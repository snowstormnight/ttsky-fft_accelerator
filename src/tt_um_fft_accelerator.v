/*
 * Copyright (c) 2024 snows
 * SPDX-License-Identifier: Apache-2.0
 */

/* verilator lint_off DECLFILENAME */
/* verilator lint_off TIMESCALEMOD */
/* verilator lint_off UNUSEDSIGNAL */
`default_nettype none

module tt_um_ttsky_deblur_accel (
    input  wire [7:0] ui_in,    // Dedicated inputs
    output wire [7:0] uo_out,   // Dedicated outputs
    input  wire [7:0] uio_in,   // IOs: Input path
    output wire [7:0] uio_out,  // IOs: Output path
    output wire [7:0] uio_oe,   // IOs: Enable path (active high: 0=input, 1=output)
    input  wire       ena,      // always 1 when the design is powered, so you can ignore it
    input  wire       clk,      // clock
    input  wire       rst_n     // reset_n - low to reset
);

  localparam int TT_DATA_W = 10;

  // Tiny Tapeout pin-map for a minimal bring-up interface.
  // Controls:
  //   ui_in[0] -> img_valid
  //   ui_in[1] -> h_valid
  //   ui_in[2] -> out_ready
  // Data:
  //   img_re from uio_in (sign-extended 8-bit)
  //   h_re   from ui_in  (sign-extended 8-bit)
  //   h_im   from uio_in (sign-extended 8-bit)
  //   k_cfg  from {ui_in[MSBs], uio_in}
  wire                    img_valid  = ui_in[0];
  wire                    h_valid    = ui_in[1];
  wire                    out_ready  = ui_in[2];
  wire signed [15:0]      img_re     = {{8{uio_in[7]}}, uio_in};
  wire signed [TT_DATA_W-1:0] h_re   = {{(TT_DATA_W-8){ui_in[7]}}, ui_in};
  wire signed [TT_DATA_W-1:0] h_im   = {{(TT_DATA_W-8){uio_in[7]}}, uio_in};
  wire        [TT_DATA_W-1:0] k_cfg  = {ui_in[7 -: (TT_DATA_W-8)], uio_in};

  wire                    img_ready;
  wire                    h_ready;
  wire                    out_valid;
  wire signed [TT_DATA_W-1:0] out_re;
  wire signed [TT_DATA_W-1:0] out_im;
  wire                    out_last;
  wire                    done;
  wire                    fft_perf_done;
  wire [63:0]             fft_perf_cycles;

  // Keep uio pins as inputs in this wrapper revision.
  assign uio_oe  = 8'b0000_0000;
  assign uio_out = 8'b0000_0000;

  // Export status/debug signals on dedicated outputs.
  assign uo_out[0] = img_ready;
  assign uo_out[1] = h_ready;
  assign uo_out[2] = out_valid;
  assign uo_out[3] = out_last;
  assign uo_out[4] = done;
  assign uo_out[5] = fft_perf_done;
  assign uo_out[6] = out_re[0];
  assign uo_out[7] = out_im[0];

  // Reduced configuration for Tiny Tapeout-oriented integration test.
  deblur #(
      .IMG_N(4),
      .FFT_LOGN(2),
      .FFT_LANES(1),
      .DATA_W(TT_DATA_W),
      .FRAC_W(8),
      .MULT_FRAC(8),
      .MULT_SAT(1),
      .PRE_IFFT_SHIFT(0),
      .FFT_TO_IFFT_MAP(0)
  ) u_deblur (
      .clk(clk),
      .rst_n(rst_n),
      .img_valid(img_valid),
      .img_ready(img_ready),
      .img_re(img_re),
      .h_valid(h_valid),
      .h_ready(h_ready),
      .h_re(h_re),
      .h_im(h_im),
      .k_cfg(k_cfg),
      .out_valid(out_valid),
      .out_ready(out_ready),
      .out_re(out_re),
      .out_im(out_im),
      .out_last(out_last),
      .done(done),
      .fft_perf_done(fft_perf_done),
      .fft_perf_cycles(fft_perf_cycles)
  );

  // List all unused inputs to prevent warnings
  wire _unused = &{ena, fft_perf_cycles[63:0], 1'b0};

endmodule

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on TIMESCALEMOD */
/* verilator lint_on DECLFILENAME */
