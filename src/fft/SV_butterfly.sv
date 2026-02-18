`default_nettype none

// -----------------------------------------------------------------------------
// Module: butterfly
// -----------------------------------------------------------------------------
// Purpose:
// - Compute one radix-2 DIF butterfly operation on complex Q1.15 inputs.
// - Architecture is area-first: one 16x16 multiplier is reused over time.
//
// Math implemented:
//   sum  = (a + b) / 2
//   diff = (a - b) / 2
//   out_a = sum
//   out_b = diff * W
// where W is twiddle factor (cos/sin) in Q1.15.
//
// Scaling policy:
// - "divide by 2" per stage via arithmetic right shift by 1.
// - This avoids overflow growth through FFT stages.
//
// Timing model:
// - start pulse accepted in S_IDLE.
// - module runs 4 internal multiply steps (S_M1..S_M4).
// - done pulses for one cycle when outputs become valid.
//
// Notes on synthesizability:
// - no real/dynamic types; all fixed-width logic.
// - no latches (all combinational defaults are assigned).
// -----------------------------------------------------------------------------
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

  // Short alias for Q1.15 signed sample type.
  typedef logic signed [15:0] q15_t;

  // Micro-FSM controlling multiplier reuse:
  // S_M1..S_M4 each compute one real product required for complex multiply.
  typedef enum logic [2:0] {S_IDLE, S_M1, S_M2, S_M3, S_M4} state_t;
  state_t state;

  // Registered precompute terms from a and b.
  // sum_* drives out_a directly; diff_* is multiplied by twiddle.
  logic signed [15:0] sum_re_r, sum_im_r, diff_re_r, diff_im_r;
  // Registered twiddle to keep data stable during multicycle operation.
  logic signed [15:0] w_re_r, w_im_r;
  // Registers holding first 3 multiplier results while last one is computed.
  logic signed [31:0] m1_r, m2_r, m3_r;
  // Shared multiplier inputs selected by state.
  logic signed [15:0] mul_a, mul_b;
  // Shared multiplier output.
  logic signed [31:0] mul_p;

  function automatic logic signed [15:0] sat16(input logic signed [31:0] x);
    // Clamp wide intermediate back to signed 16-bit Q1.15 range.
    if (x > 32'sd32767) sat16 = 16'sd32767;
    else if (x < -32'sd32768) sat16 = -16'sd32768;
    else sat16 = x[15:0];
  endfunction

  always_comb begin
    // Reuse one real multiplier across 4 products:
    // 1) dr*wr, 2) di*wi, 3) dr*wi, 4) di*wr
    // Later assembled into:
    // re = dr*wr - di*wi
    // im = dr*wi + di*wr
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

  // Single hardware multiplier.
  assign mul_p = mul_a * mul_b;
  // busy is high whenever operation is in flight.
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
      // done is a pulse, so clear every cycle unless set in S_M4.
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
            // Move into multiply sequence.
            state     <= S_M1;
          end
        end
        // Capture product #1 (dr*wr).
        S_M1: begin
          m1_r  <= mul_p;
          state <= S_M2;
        end
        // Capture product #2 (di*wi).
        S_M2: begin
          m2_r  <= mul_p;
          state <= S_M3;
        end
        // Capture product #3 (dr*wi).
        S_M3: begin
          m3_r  <= mul_p;
          state <= S_M4;
        end
        // Product #4 (di*wr) is available on mul_p now.
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
