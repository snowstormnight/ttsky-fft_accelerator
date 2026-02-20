// complex_mult_elem.sv
// Element-wise complex multiplier with optional parallel lanes and 0/1 pipeline stage.
// Intended for frequency-domain multiply: Y(u,v) * G(u,v) across synchronized bin streams.
//
// Fixed-point format: signed Q(IN_W-FRAC).(FRAC)
// Output format: signed Q(OUT_W-FRAC).(FRAC) (same FRAC scaling)
//
// Spec alignment: element-wise complex multiply, parallel lanes, pipelining :contentReference[oaicite:1]{index=1}

module complex_mult #(
  parameter int unsigned IN_W         = 16,  // input word width
  parameter int unsigned OUT_W        = 16,  // output word width
  parameter int unsigned FRAC         = 8,   // fractional bits
  parameter int unsigned LANES        = 1,   // complex multiplies per cycle
  parameter bit          USE_GAUSS_3M = 1'b1,// 1: 3-mult Gauss (area) 0: 4-mult standard
  parameter bit          ROUND        = 1'b1,// 1: symmetric round-to-nearest (ties away from 0), 0: truncate
  parameter bit          SATURATE     = 1'b0,// 1: saturate to OUT_W, 0: wrap
  parameter bit          PIPE1        = 1'b1 // 1: one register stage with ready/valid, 0: purely combinational
) (
  input  logic clk,
  input  logic rst_n,

  // streaming handshake
  input  logic in_valid,
  output logic in_ready,

  input  logic out_ready,
  output logic out_valid,

  // Flattened lane buses: lane i is bits [i*IN_W +: IN_W]
  input  logic signed [LANES*IN_W-1:0] a_re,
  input  logic signed [LANES*IN_W-1:0] a_im,
  input  logic signed [LANES*IN_W-1:0] b_re,
  input  logic signed [LANES*IN_W-1:0] b_im,

  output logic signed [LANES*OUT_W-1:0] y_re,
  output logic signed [LANES*OUT_W-1:0] y_im
);

  // ----------------------------
  // Local widths (safe headroom)
  // ----------------------------
  localparam int unsigned SUM_W  = IN_W + 1;            // for (a_re + a_im), etc.
  localparam int unsigned MUL_W  = 2*IN_W;              // product width
  localparam int unsigned ACC_W  = MUL_W + 2;           // headroom for adds/subs

  // ----------------------------
  // Helpers
  // ----------------------------
  function automatic logic signed [OUT_W-1:0] sat_to_out_w(input logic signed [ACC_W-1:0] x);
    logic signed [ACC_W-1:0] maxv, minv;
    begin
      // max =  2^(OUT_W-1)-1, min = -2^(OUT_W-1)
      maxv =  $signed({1'b0, {(OUT_W-1){1'b1}}}); // fits in ACC_W by sign extension below
      minv = -$signed({1'b1, {(OUT_W-1){1'b0}}});

      // Sign-extend max/min to ACC_W
      maxv = $signed({{(ACC_W-OUT_W){1'b0}}, maxv[OUT_W-1:0]});
      minv = $signed({{(ACC_W-OUT_W){1'b1}}, minv[OUT_W-1:0]});

      if (x > maxv)      sat_to_out_w = $signed({1'b0, {(OUT_W-1){1'b1}}});
      else if (x < minv) sat_to_out_w = $signed({1'b1, {(OUT_W-1){1'b0}}});
      else               sat_to_out_w = x[OUT_W-1:0];
    end
  endfunction

  function automatic logic signed [ACC_W-1:0] round_and_shift(
    input logic signed [ACC_W-1:0] x_full
  );
    logic signed [ACC_W-1:0] x_bias;
    logic signed [ACC_W-1:0] bias;
    begin
      if (FRAC == 0) begin
        round_and_shift = x_full;
      end else begin
        if (ROUND) begin
          // symmetric rounding: +0.5 for >=0, -0.5 for <0
          bias   = $signed(1) <<< (FRAC-1);
          x_bias = (x_full >= 0) ? (x_full + bias) : (x_full - bias);
        end else begin
          x_bias = x_full;
        end
        round_and_shift = x_bias >>> FRAC; // arithmetic shift keeps sign
      end
    end
  endfunction

  // ----------------------------
  // Combinational lane compute
  // ----------------------------
  logic signed [LANES*OUT_W-1:0] y_re_c, y_im_c;

  genvar i;
  generate
    for (i = 0; i < LANES; i++) begin : g_lane
      // Slice lane inputs
      logic signed [IN_W-1:0]  ar, ai, br, bi;
      logic signed [SUM_W-1:0] ar_p_ai, br_p_bi;

      logic signed [MUL_W-1:0] m1, m2, m3, m4;
      logic signed [ACC_W-1:0] real_full, imag_full;
      logic signed [ACC_W-1:0] real_s,    imag_s;

      always_comb begin
        ar = a_re[i*IN_W +: IN_W];
        ai = a_im[i*IN_W +: IN_W];
        br = b_re[i*IN_W +: IN_W];
        bi = b_im[i*IN_W +: IN_W];

        // default
        m1 = '0; m2 = '0; m3 = '0; m4 = '0;
        real_full = '0;
        imag_full = '0;

        if (USE_GAUSS_3M) begin
          // Gauss 3-mult:
          // k1 = ar*br
          // k2 = ai*bi
          // k3 = (ar+ai)*(br+bi)
          // real = k1 - k2
          // imag = k3 - k1 - k2
          ar_p_ai = $signed(ar) + $signed(ai);
          br_p_bi = $signed(br) + $signed(bi);

          m1 = $signed(ar)     * $signed(br);     // k1
          m2 = $signed(ai)     * $signed(bi);     // k2
          m3 = $signed(ar_p_ai)* $signed(br_p_bi);// k3

          real_full = $signed({{(ACC_W-MUL_W){m1[MUL_W-1]}}, m1})
                    - $signed({{(ACC_W-MUL_W){m2[MUL_W-1]}}, m2});

          imag_full = $signed({{(ACC_W-MUL_W){m3[MUL_W-1]}}, m3})
                    - $signed({{(ACC_W-MUL_W){m1[MUL_W-1]}}, m1})
                    - $signed({{(ACC_W-MUL_W){m2[MUL_W-1]}}, m2});
        end else begin
          // Standard 4-mult:
          // real = ar*br - ai*bi
          // imag = ar*bi + ai*br
          m1 = $signed(ar) * $signed(br);
          m2 = $signed(ai) * $signed(bi);
          m3 = $signed(ar) * $signed(bi);
          m4 = $signed(ai) * $signed(br);

          real_full = $signed({{(ACC_W-MUL_W){m1[MUL_W-1]}}, m1})
                    - $signed({{(ACC_W-MUL_W){m2[MUL_W-1]}}, m2});

          imag_full = $signed({{(ACC_W-MUL_W){m3[MUL_W-1]}}, m3})
                    + $signed({{(ACC_W-MUL_W){m4[MUL_W-1]}}, m4});
        end

        // scale back to FRAC
        real_s = round_and_shift(real_full);
        imag_s = round_and_shift(imag_full);

        // saturate/wrap to OUT_W
        if (SATURATE) begin
          y_re_c[i*OUT_W +: OUT_W] = sat_to_out_w(real_s);
          y_im_c[i*OUT_W +: OUT_W] = sat_to_out_w(imag_s);
        end else begin
          y_re_c[i*OUT_W +: OUT_W] = real_s[OUT_W-1:0];
          y_im_c[i*OUT_W +: OUT_W] = imag_s[OUT_W-1:0];
        end
      end
    end
  endgenerate

  // ----------------------------
  // Handshake / optional pipeline
  // ----------------------------
  if (PIPE1) begin : g_pipe1
    logic signed [LANES*OUT_W-1:0] y_re_r, y_im_r;
    logic                         v_r;

    // single-stage ready/valid register
    assign in_ready  = (~v_r) | out_ready;
    assign out_valid = v_r;
    assign y_re      = y_re_r;
    assign y_im      = y_im_r;

    always_ff @(posedge clk or negedge rst_n) begin
      if (!rst_n) begin
        v_r   <= 1'b0;
        y_re_r <= '0;
        y_im_r <= '0;
      end else begin
        if (in_ready) begin
          v_r <= in_valid;
          if (in_valid) begin
            y_re_r <= y_re_c;
            y_im_r <= y_im_c;
          end
        end
      end
    end
  end else begin : g_comb
    // Combinational pass-through (no internal storage).
    // Backpressure propagates directly.
    assign in_ready  = out_ready;
    assign out_valid = in_valid;
    assign y_re      = y_re_c;
    assign y_im      = y_im_c;
  end

endmodule