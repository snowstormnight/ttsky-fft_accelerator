`default_nettype none

// 2D FFT core using one reused 1D FFT core (row pass then column pass).
// Input:  N*N samples in natural raster order.
// Output: N*N complex bins in bit-reversed order on both axes.
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
    output logic signed [15:0]      out_im
);

  localparam int TOT = N * N;
  localparam int AW  = $clog2(TOT);

  typedef enum logic [2:0] {
    ST_LOAD, ST_ROW_FEED, ST_ROW_RECV, ST_COL_FEED, ST_COL_RECV, ST_DRAIN
  } state_t;
  state_t state;

  logic signed [15:0] mem0_re [0:TOT-1];
  logic signed [15:0] mem0_im [0:TOT-1];
  logic signed [15:0] mem1_re [0:TOT-1];
  logic signed [15:0] mem1_im [0:TOT-1];

  logic [LOGN-1:0] row_idx, col_idx, feed_cnt, recv_cnt;
  logic [AW-1:0] out_ptr;
  logic [AW-1:0] load_ptr;

  function automatic logic [AW-1:0] idx2d(input logic [LOGN-1:0] r, input logic [LOGN-1:0] c);
    idx2d = {r, c};
  endfunction

  logic                    f_in_valid, f_in_ready, f_out_valid, f_out_ready;
  logic signed [15:0]      f_in_re, f_in_im, f_out_re, f_out_im;

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

  assign in_ready  = (state == ST_LOAD);
  assign out_valid = (state == ST_DRAIN);

  always_comb begin
    f_in_valid  = 1'b0;
    f_in_re     = '0;
    f_in_im     = '0;
    f_out_ready = 1'b0;
    out_re      = '0;
    out_im      = '0;

    unique case (state)
      ST_ROW_FEED: begin
        f_in_valid = 1'b1;
        f_in_re    = mem0_re[idx2d(row_idx, feed_cnt)];
        f_in_im    = mem0_im[idx2d(row_idx, feed_cnt)];
      end
      ST_ROW_RECV: f_out_ready = 1'b1;
      ST_COL_FEED: begin
        f_in_valid = 1'b1;
        f_in_re    = mem1_re[idx2d(feed_cnt, col_idx)];
        f_in_im    = mem1_im[idx2d(feed_cnt, col_idx)];
      end
      ST_COL_RECV: f_out_ready = 1'b1;
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
    end else begin
      unique case (state)
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

        ST_DRAIN: begin
          if (out_valid && out_ready) begin
            if (out_ptr == AW'(TOT - 1)) begin
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
