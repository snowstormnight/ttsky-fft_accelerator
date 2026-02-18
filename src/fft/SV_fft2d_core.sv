`default_nettype none

// -----------------------------------------------------------------------------
// Module: fft2d_core
// -----------------------------------------------------------------------------
// Purpose:
// - Compute one NxN complex 2D FFT by reusing a single 1D FFT core.
// - This is area-oriented (iterative), not throughput-maximal.
//
// Processing sequence per image tile:
// 1) ST_LOAD     : accept N*N input samples (row-major) into mem0.
// 2) ST_ROW_*    : run 1D FFT on each row:  mem0 -> fft1d -> mem1.
// 3) ST_COL_*    : run 1D FFT on each col:  mem1 -> fft1d -> mem0.
// 4) ST_DRAIN    : stream N*N output bins from mem0.
//
// External protocol:
// - in_ready  high only during ST_LOAD.
// - out_valid high only during ST_DRAIN.
// - one sample per cycle when valid && ready handshake succeeds.
//
// Ordering:
// - Input expected in natural raster row-major order.
// - Output is bit-reversed on both dimensions (due to DIF 1D kernel).
// - Software can reorder back to natural.
//
// Performance counters:
// - perf_done   pulses for one cycle at end of block.
// - perf_cycles latches number of cycles from first accepted input sample
//   to final accepted output sample for that block.
// -----------------------------------------------------------------------------
module fft2d_core #(
    parameter int N    = 32,
    parameter int LOGN = 5
) (
    input  logic                    clk,
    input  logic                    rst_n,
    input  logic                    in_valid,
    output logic                    in_ready,
    input  logic signed [15:0]      in_re,
    input  logic signed [15:0]      in_im,
    output logic                    out_valid,
    input  logic                    out_ready,
    output logic signed [15:0]      out_re,
    output logic signed [15:0]      out_im,
    output logic                    perf_done,
    output logic [63:0]             perf_cycles
);

  // Total samples in one 2D tile and address width for flattened storage.
  localparam int TOT = N * N;
  localparam int AW  = $clog2(TOT);

  // Block controller states for whole 2D operation.
  typedef enum logic [2:0] {
    ST_LOAD, ST_ROW_FEED, ST_ROW_RECV, ST_COL_FEED, ST_COL_RECV, ST_DRAIN
  } state_t;
  state_t state;

  // Two local memories:
  // - mem0: input and final output workspace
  // - mem1: intermediate row-FFT workspace
  logic signed [15:0] mem0_re [0:TOT-1];
  logic signed [15:0] mem0_im [0:TOT-1];
  logic signed [15:0] mem1_re [0:TOT-1];
  logic signed [15:0] mem1_im [0:TOT-1];

  // row_idx : which row is being processed in row pass.
  // col_idx : which column is being processed in column pass.
  // feed_cnt: position currently fed into 1D core.
  // recv_cnt: position currently received from 1D core.
  logic [LOGN-1:0] row_idx, col_idx, feed_cnt, recv_cnt;
  // load_ptr: flattened write pointer while loading input tile.
  // out_ptr : flattened read pointer while draining final output tile.
  logic [AW-1:0] out_ptr;
  logic [AW-1:0] load_ptr;

  // Convert 2D index (row, col) -> 1D row-major address.
  function automatic logic [AW-1:0] idx2d(input logic [LOGN-1:0] r, input logic [LOGN-1:0] c);
    idx2d = {r, c};
  endfunction

  // Internal streaming signals connected to reused fft1d_core.
  logic                    f_in_valid, f_in_ready, f_out_valid, f_out_ready;
  logic signed [15:0]      f_in_re, f_in_im, f_out_re, f_out_im;
  // perf_active gates counting; perf_counter accumulates cycle count.
  logic                    perf_active;
  logic [63:0]             perf_counter;

  fft1d_core #(.N(N), .LOGN(LOGN)) u_fft1d (
      .clk(clk),
      .rst_n(rst_n),
      .in_valid(f_in_valid),
      .in_ready(f_in_ready),
      .in_re(f_in_re),
      .in_im(f_in_im),
      .out_valid(f_out_valid),
      .out_ready(f_out_ready),
      .out_re(f_out_re),
      .out_im(f_out_im)
  );

  // External block-level handshake:
  // - ready during full-tile load
  // - valid during full-tile drain
  assign in_ready  = (state == ST_LOAD);
  assign out_valid = (state == ST_DRAIN);

  always_comb begin
    // Safe defaults avoid latch inference.
    f_in_valid  = 1'b0;
    f_in_re     = '0;
    f_in_im     = '0;
    f_out_ready = 1'b0;
    out_re      = '0;
    out_im      = '0;

    unique case (state)
      // Feed one row into 1D FFT.
      ST_ROW_FEED: begin
        f_in_valid = 1'b1;
        f_in_re    = mem0_re[idx2d(row_idx, feed_cnt)];
        f_in_im    = mem0_im[idx2d(row_idx, feed_cnt)];
      end
      // Receive one row from 1D FFT.
      ST_ROW_RECV: f_out_ready = 1'b1;
      // Feed one column into 1D FFT.
      ST_COL_FEED: begin
        f_in_valid = 1'b1;
        f_in_re    = mem1_re[idx2d(feed_cnt, col_idx)];
        f_in_im    = mem1_im[idx2d(feed_cnt, col_idx)];
      end
      // Receive one column from 1D FFT.
      ST_COL_RECV: f_out_ready = 1'b1;
      // Drain final 2D FFT bins out.
      ST_DRAIN: begin
        out_re = mem0_re[out_ptr];
        out_im = mem0_im[out_ptr];
      end
      default: begin end
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state     <= ST_LOAD;
      load_ptr  <= '0;
      row_idx   <= '0;
      col_idx   <= '0;
      feed_cnt  <= '0;
      recv_cnt  <= '0;
      out_ptr   <= '0;
      perf_done <= 1'b0;
      perf_cycles <= '0;
      perf_active <= 1'b0;
      perf_counter <= '0;
    end else begin
      // perf_done is one-cycle pulse; clear by default.
      perf_done <= 1'b0;
      // Start counting on first accepted input sample of a block.
      if (!perf_active && state == ST_LOAD && in_valid && in_ready && load_ptr == '0) begin
        perf_active <= 1'b1;
        perf_counter <= 64'd1;
      end else if (perf_active) begin
        perf_counter <= perf_counter + 64'd1;
      end

      unique case (state)
        // Load full tile into mem0.
        ST_LOAD: begin
          if (in_valid && in_ready) begin
            mem0_re[load_ptr] <= in_re;
            mem0_im[load_ptr] <= in_im;
            if (load_ptr == AW'(TOT - 1)) begin
              load_ptr <= '0;
              row_idx  <= '0;
              feed_cnt <= '0;
              recv_cnt <= '0;
              state    <= ST_ROW_FEED;
            end else begin
              load_ptr <= load_ptr + 1'b1;
            end
          end
        end

        // Feed one row to 1D core (N samples).
        ST_ROW_FEED: begin
          if (f_in_valid && f_in_ready) begin
            if (feed_cnt == LOGN'(N - 1)) begin
              feed_cnt <= '0;
              recv_cnt <= '0;
              state    <= ST_ROW_RECV;
            end else begin
              feed_cnt <= feed_cnt + 1'b1;
            end
          end
        end

        // Receive one transformed row and store into mem1.
        ST_ROW_RECV: begin
          if (f_out_valid && f_out_ready) begin
            mem1_re[idx2d(row_idx, recv_cnt)] <= f_out_re;
            mem1_im[idx2d(row_idx, recv_cnt)] <= f_out_im;
            if (recv_cnt == LOGN'(N - 1)) begin
              recv_cnt <= '0;
              if (row_idx == LOGN'(N - 1)) begin
                row_idx  <= '0;
                col_idx  <= '0;
                feed_cnt <= '0;
                state    <= ST_COL_FEED;
              end else begin
                row_idx  <= row_idx + 1'b1;
                feed_cnt <= '0;
                state    <= ST_ROW_FEED;
              end
            end else begin
              recv_cnt <= recv_cnt + 1'b1;
            end
          end
        end

        // Feed one column from mem1 into 1D core.
        ST_COL_FEED: begin
          if (f_in_valid && f_in_ready) begin
            if (feed_cnt == LOGN'(N - 1)) begin
              feed_cnt <= '0;
              recv_cnt <= '0;
              state    <= ST_COL_RECV;
            end else begin
              feed_cnt <= feed_cnt + 1'b1;
            end
          end
        end

        // Receive one transformed column and write final bins into mem0.
        ST_COL_RECV: begin
          if (f_out_valid && f_out_ready) begin
            mem0_re[idx2d(recv_cnt, col_idx)] <= f_out_re;
            mem0_im[idx2d(recv_cnt, col_idx)] <= f_out_im;
            if (recv_cnt == LOGN'(N - 1)) begin
              recv_cnt <= '0;
              if (col_idx == LOGN'(N - 1)) begin
                out_ptr <= '0;
                state   <= ST_DRAIN;
              end else begin
                col_idx  <= col_idx + 1'b1;
                feed_cnt <= '0;
                state    <= ST_COL_FEED;
              end
            end else begin
              recv_cnt <= recv_cnt + 1'b1;
            end
          end
        end

        // Drain final output tile.
        ST_DRAIN: begin
          if (out_valid && out_ready) begin
            if (out_ptr == AW'(TOT - 1)) begin
              // End of block: latch cycle count for this image tile.
              if (perf_active) begin
                perf_done <= 1'b1;
                perf_cycles <= perf_counter;
                perf_active <= 1'b0;
              end
              out_ptr <= '0;
              state   <= ST_LOAD;
            end else begin
              out_ptr <= out_ptr + 1'b1;
            end
          end
        end
        default: state <= ST_LOAD;
      endcase
    end
  end

endmodule

`default_nettype wire
