`default_nettype none
`timescale 1ns/1ps

module tb();

  initial begin
    $dumpfile("tb.fst");
    $dumpvars(0, tb);
    #1;
  end

  // Inputs to DUT (driven by cocotb)
  reg  signed [15:0] a_re;
  reg  signed [15:0] a_im;
  reg  signed [15:0] b_re;
  reg  signed [15:0] b_im;

  // Outputs from DUT (checked by cocotb)
  wire signed [15:0] y_re;
  wire signed [15:0] y_im;

  complex_mult #(.W(16)) dut (
    .a_re(a_re),
    .a_im(a_im),
    .b_re(b_re),
    .b_im(b_im),
    .y_re(y_re),
    .y_im(y_im)
  );

endmodule
