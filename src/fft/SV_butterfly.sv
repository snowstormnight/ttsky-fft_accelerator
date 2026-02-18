`default_nettype none

// Radix-2 DIF butterfly with per-stage scaling (right shift by 1).
// Uses one time-shared 16x16 multiplier over 4 cycles.
// Outputs:
//   out_a = (a + b)/2
//   out_b = ((a - b)/2) * W
//
// This module is intentionally serialized for area efficiency:
// one real multiplier is reused over four micro-steps (S_M1..S_M4).
module butterfly (
    input  logic                    clk,
    input  logic                    rst_n,
    input  logic                    start,
    input  logic signed [15:0]      a_re,
    input  logic signed [15:0]      a_im,
    input  logic signed [15:0]      b_re,
    input  logic signed [15:0]      b_im,
    input  logic signed [15:0]      w_re,
    input  logic signed [15:0]      w_im,
    output logic                    busy,
    output logic                    done,
    output logic signed [15:0]      out_a_re,
    output logic signed [15:0]      out_a_im,
    output logic signed [15:0]      out_b_re,
    output logic signed [15:0]      out_b_im
);

  typedef logic signed [15:0] q15_t;
  // Micro-FSM for time-sharing the single multiplier.
  typedef enum logic [2:0] {S_IDLE, S_M1, S_M2, S_M3, S_M4} state_t;
  state_t state;

  logic signed [15:0] sum_re_r, sum_im_r, diff_re_r, diff_im_r;
  logic signed [15:0] w_re_r, w_im_r;
  logic signed [31:0] m1_r, m2_r, m3_r;
  logic signed [15:0] mul_a, mul_b;
  logic signed [31:0] mul_p;

  function automatic logic signed [15:0] sat16(input logic signed [31:0] x);
    // Clamp wide intermediate back to signed 16-bit Q1.15 range.
    if (x > 32'sd32767) sat16 = 16'sd32767;
    else if (x < -32'sd32768) sat16 = -16'sd32768;
    else sat16 = x[15:0];
  endfunction

  always_comb begin
    // Reuse one real multiplier across 4 products:
    // 1) diff_re*w_re, 2) diff_im*w_im, 3) diff_re*w_im, 4) diff_im*w_re
    mul_a = '0;
    mul_b = '0;
    unique case (state)
      S_M1: begin mul_a = diff_re_r; mul_b = w_re_r; end
      S_M2: begin mul_a = diff_im_r; mul_b = w_im_r; end
      S_M3: begin mul_a = diff_re_r; mul_b = w_im_r; end
      S_M4: begin mul_a = diff_im_r; mul_b = w_re_r; end
      default: begin end
    endcase
  end

  assign mul_p = mul_a * mul_b;
  assign busy  = (state != S_IDLE);

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state     <= S_IDLE;
      done      <= 1'b0;
      out_a_re  <= '0;
      out_a_im  <= '0;
      out_b_re  <= '0;
      out_b_im  <= '0;
      sum_re_r  <= '0;
      sum_im_r  <= '0;
      diff_re_r <= '0;
      diff_im_r <= '0;
      w_re_r    <= '0;
      w_im_r    <= '0;
      m1_r      <= '0;
      m2_r      <= '0;
      m3_r      <= '0;
    end else begin
      done <= 1'b0;
      unique case (state)
        S_IDLE: begin
          if (start) begin
            // Stage scaling: divide by 2 each stage to control growth.
            sum_re_r  <= q15_t'(($signed({a_re[15], a_re}) + $signed({b_re[15], b_re})) >>> 1);
            sum_im_r  <= q15_t'(($signed({a_im[15], a_im}) + $signed({b_im[15], b_im})) >>> 1);
            diff_re_r <= q15_t'(($signed({a_re[15], a_re}) - $signed({b_re[15], b_re})) >>> 1);
            diff_im_r <= q15_t'(($signed({a_im[15], a_im}) - $signed({b_im[15], b_im})) >>> 1);
            w_re_r    <= w_re;
            w_im_r    <= w_im;
            state     <= S_M1;
          end
        end
        S_M1: begin
          m1_r  <= mul_p;
          state <= S_M2;
        end
        S_M2: begin
          m2_r  <= mul_p;
          state <= S_M3;
        end
        S_M3: begin
          m3_r  <= mul_p;
          state <= S_M4;
        end
        S_M4: begin
          // Complex multiply assembly:
          // (dr+jdi)*(wr+jwi) = (dr*wr - di*wi) + j(dr*wi + di*wr)
          out_a_re <= sum_re_r;
          out_a_im <= sum_im_r;
          out_b_re <= sat16((m1_r - m2_r) >>> 15);
          out_b_im <= sat16((m3_r + mul_p) >>> 15);
          done     <= 1'b1;
          state    <= S_IDLE;
        end
        default: state <= S_IDLE;
      endcase
    end
  end

endmodule

`default_nettype wire
