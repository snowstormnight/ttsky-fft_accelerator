/* verilator lint_off DECLFILENAME */
`default_nettype none

// -----------------------------------------------------------------------------
// Module: twiddle_rom
// -----------------------------------------------------------------------------
// Purpose:
// - Lookup twiddle coefficients for FFT butterflies in Q1.15 fixed-point.
//
// Definition:
// - W_N^k = exp(-j*2*pi*k/N) = cos(2*pi*k/N) - j*sin(2*pi*k/N)
// - w_re = Re{W_N^k}, w_im = Im{W_N^k}
//
// Implementation notes:
// - Combinational case ROM (synthesizable).
// - This static file contains values for N=32 schedule (k=0..15).
// - For other N (e.g., 256), the Python flow auto-generates
//   build/SV_twiddle_rom_auto.sv and compiles that instead.
// -----------------------------------------------------------------------------
module twiddle_rom #(
    parameter int LOGN = 5
) (
    input  logic [LOGN-1:0]         idx,
    output logic signed [15:0]      w_re,
    output logic signed [15:0]      w_im
);

  always_comb begin
    // idx selects twiddle k.
    unique case (idx)
      5'd0:  begin w_re = 16'sd32767;  w_im = 16'sd0;      end
      5'd1:  begin w_re = 16'sd32138;  w_im = -16'sd6393;  end
      5'd2:  begin w_re = 16'sd30274;  w_im = -16'sd12540; end
      5'd3:  begin w_re = 16'sd27246;  w_im = -16'sd18205; end
      5'd4:  begin w_re = 16'sd23170;  w_im = -16'sd23170; end
      5'd5:  begin w_re = 16'sd18205;  w_im = -16'sd27246; end
      5'd6:  begin w_re = 16'sd12540;  w_im = -16'sd30274; end
      5'd7:  begin w_re = 16'sd6393;   w_im = -16'sd32138; end
      5'd8:  begin w_re = 16'sd0;      w_im = -16'sd32768; end
      5'd9:  begin w_re = -16'sd6393;  w_im = -16'sd32138; end
      5'd10: begin w_re = -16'sd12540; w_im = -16'sd30274; end
      5'd11: begin w_re = -16'sd18205; w_im = -16'sd27246; end
      5'd12: begin w_re = -16'sd23170; w_im = -16'sd23170; end
      5'd13: begin w_re = -16'sd27246; w_im = -16'sd18205; end
      5'd14: begin w_re = -16'sd30274; w_im = -16'sd12540; end
      5'd15: begin w_re = -16'sd32138; w_im = -16'sd6393;  end
      default: begin w_re = 16'sd0;   w_im = 16'sd0;      end
    endcase
  end

endmodule

/* verilator lint_on DECLFILENAME */
`default_nettype wire
