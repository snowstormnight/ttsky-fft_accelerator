/* verilator lint_off DECLFILENAME */
`default_nettype none

// -----------------------------------------------------------------------------
// Module: fft2d_core
// -----------------------------------------------------------------------------
// Purpose:
// - Compute one NxN complex 2D FFT.
// - Parallelism is parameterized by LANES (number of 1D FFT cores reused in
//   parallel for row/column batches).
//
// Key tradeoff:
// - LANES=1  : smallest area, highest latency.
// - LANES>1  : higher area, lower latency (row/column batches run in parallel).
//
// External interface remains narrow streaming (one complex sample per cycle):
// - Input : row-major spatial tile entries.
// - Output: row-major frequency bins, bit-reversed on both dimensions.
// -----------------------------------------------------------------------------
module fft2d_core #(
    parameter int N      = 32,
    parameter int LOGN   = 5,
    parameter int LANES  = 1
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

  localparam int TOT = N * N;
  localparam int AW  = $clog2(TOT);
  typedef logic [LOGN-1:0] idx_t;

  typedef enum logic [2:0] {
    ST_LOAD, ST_ROW_FEED, ST_ROW_RECV, ST_COL_FEED, ST_COL_RECV, ST_DRAIN
  } state_t;
  state_t state;

  // Working memories:
  // mem0: input + final output, mem1: row-pass intermediate.
  logic signed [15:0] mem0_re [0:TOT-1];
  logic signed [15:0] mem0_im [0:TOT-1];
  logic signed [15:0] mem1_re [0:TOT-1];
  logic signed [15:0] mem1_im [0:TOT-1];

  // batch_base selects current row/col batch start.
  // feed_cnt/recv_cnt are intra-vector positions for one 1D FFT call.
  idx_t batch_base, feed_cnt, recv_cnt;
  logic [AW-1:0] out_ptr, load_ptr;

  function automatic logic [AW-1:0] idx2d(input idx_t r, input idx_t c);
    idx2d = {r, c};
  endfunction

  // One set of stream signals per lane/core.
  logic [LANES-1:0] f_in_valid, f_in_ready, f_out_valid, f_out_ready;
  logic signed [15:0] f_in_re  [0:LANES-1];
  logic signed [15:0] f_in_im  [0:LANES-1];
  logic signed [15:0] f_out_re [0:LANES-1];
  logic signed [15:0] f_out_im [0:LANES-1];

  // Feed/receive progress for all lanes in lockstep.
  logic feed_fire, recv_fire;

  logic perf_active;
  logic [63:0] perf_counter;

  genvar g;
  generate
    for (g = 0; g < LANES; g++) begin : g_fft
      fft1d_core #(.N(N), .LOGN(LOGN)) u_fft1d (
          .clk(clk),
          .rst_n(rst_n),
          .in_valid(f_in_valid[g]),
          .in_ready(f_in_ready[g]),
          .in_re(f_in_re[g]),
          .in_im(f_in_im[g]),
          .out_valid(f_out_valid[g]),
          .out_ready(f_out_ready[g]),
          .out_re(f_out_re[g]),
          .out_im(f_out_im[g])
      );
    end
  endgenerate

  assign in_ready  = (state == ST_LOAD);
  assign out_valid = (state == ST_DRAIN);

  always_comb begin
    out_re = '0;
    out_im = '0;

    // Default all lane streams idle.
    for (int l = 0; l < LANES; l++) begin
      f_in_valid[l]  = 1'b0;
      f_in_re[l]     = '0;
      f_in_im[l]     = '0;
      f_out_ready[l] = 1'b0;
    end

    // Feed/recv advance events require all lanes to be ready/valid.
    feed_fire = 1'b0;
    recv_fire = 1'b0;

    unique case (state)
      ST_ROW_FEED: begin
        feed_fire = 1'b1;
        for (int l = 0; l < LANES; l++) begin
          f_in_valid[l] = 1'b1;
          f_in_re[l]    = mem0_re[idx2d(batch_base + idx_t'(l), feed_cnt)];
          f_in_im[l]    = mem0_im[idx2d(batch_base + idx_t'(l), feed_cnt)];
          feed_fire    &= f_in_ready[l];
        end
      end

      ST_ROW_RECV: begin
        recv_fire = 1'b1;
        for (int l = 0; l < LANES; l++) begin
          f_out_ready[l] = 1'b1;
          recv_fire     &= f_out_valid[l];
        end
      end

      ST_COL_FEED: begin
        feed_fire = 1'b1;
        for (int l = 0; l < LANES; l++) begin
          f_in_valid[l] = 1'b1;
          f_in_re[l]    = mem1_re[idx2d(feed_cnt, batch_base + idx_t'(l))];
          f_in_im[l]    = mem1_im[idx2d(feed_cnt, batch_base + idx_t'(l))];
          feed_fire    &= f_in_ready[l];
        end
      end

      ST_COL_RECV: begin
        recv_fire = 1'b1;
        for (int l = 0; l < LANES; l++) begin
          f_out_ready[l] = 1'b1;
          recv_fire     &= f_out_valid[l];
        end
      end

      ST_DRAIN: begin
        out_re = mem0_re[out_ptr];
        out_im = mem0_im[out_ptr];
      end

      default: begin end
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state        <= ST_LOAD;
      batch_base   <= '0;
      feed_cnt     <= '0;
      recv_cnt     <= '0;
      out_ptr      <= '0;
      load_ptr     <= '0;
      perf_done    <= 1'b0;
      perf_cycles  <= '0;
      perf_active  <= 1'b0;
      perf_counter <= '0;
    end else begin
      perf_done <= 1'b0;

      // Count block latency from first accepted input to final accepted output.
      if (!perf_active && state == ST_LOAD && in_valid && in_ready && load_ptr == '0) begin
        perf_active  <= 1'b1;
        perf_counter <= 64'd1;
      end else if (perf_active) begin
        perf_counter <= perf_counter + 64'd1;
      end

      unique case (state)
        ST_LOAD: begin
          if (in_valid && in_ready) begin
            mem0_re[load_ptr] <= in_re;
            mem0_im[load_ptr] <= in_im;
            if (load_ptr == AW'(TOT - 1)) begin
              load_ptr   <= '0;
              batch_base <= '0;
              feed_cnt   <= '0;
              recv_cnt   <= '0;
              state      <= ST_ROW_FEED;
            end else begin
              load_ptr <= load_ptr + 1'b1;
            end
          end
        end

        ST_ROW_FEED: begin
          if (feed_fire) begin
            if (feed_cnt == idx_t'(N - 1)) begin
              feed_cnt <= '0;
              recv_cnt <= '0;
              state    <= ST_ROW_RECV;
            end else begin
              feed_cnt <= feed_cnt + 1'b1;
            end
          end
        end

        ST_ROW_RECV: begin
          if (recv_fire) begin
            for (int l = 0; l < LANES; l++) begin
              idx_t row_l = batch_base + idx_t'(l);
              mem1_re[idx2d(row_l, recv_cnt)] <= f_out_re[l];
              mem1_im[idx2d(row_l, recv_cnt)] <= f_out_im[l];
            end

            if (recv_cnt == idx_t'(N - 1)) begin
              recv_cnt <= '0;
              if (batch_base == idx_t'(N - LANES)) begin
                batch_base <= '0;
                feed_cnt   <= '0;
                state      <= ST_COL_FEED;
              end else begin
                batch_base <= batch_base + idx_t'(LANES);
                feed_cnt   <= '0;
                state      <= ST_ROW_FEED;
              end
            end else begin
              recv_cnt <= recv_cnt + 1'b1;
            end
          end
        end

        ST_COL_FEED: begin
          if (feed_fire) begin
            if (feed_cnt == idx_t'(N - 1)) begin
              feed_cnt <= '0;
              recv_cnt <= '0;
              state    <= ST_COL_RECV;
            end else begin
              feed_cnt <= feed_cnt + 1'b1;
            end
          end
        end

        ST_COL_RECV: begin
          if (recv_fire) begin
            for (int l = 0; l < LANES; l++) begin
              idx_t col_l = batch_base + idx_t'(l);
              mem0_re[idx2d(recv_cnt, col_l)] <= f_out_re[l];
              mem0_im[idx2d(recv_cnt, col_l)] <= f_out_im[l];
            end

            if (recv_cnt == idx_t'(N - 1)) begin
              recv_cnt <= '0;
              if (batch_base == idx_t'(N - LANES)) begin
                out_ptr <= '0;
                state   <= ST_DRAIN;
              end else begin
                batch_base <= batch_base + idx_t'(LANES);
                feed_cnt   <= '0;
                state      <= ST_COL_FEED;
              end
            end else begin
              recv_cnt <= recv_cnt + 1'b1;
            end
          end
        end

        ST_DRAIN: begin
          if (out_valid && out_ready) begin
            if (out_ptr == AW'(TOT - 1)) begin
              if (perf_active) begin
                perf_done    <= 1'b1;
                perf_cycles  <= perf_counter;
                perf_active  <= 1'b0;
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

/* verilator lint_on DECLFILENAME */
`default_nettype wire
