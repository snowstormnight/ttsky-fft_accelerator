`default_nettype none

// Q1.15 twiddles for W16^k = exp(-j*2*pi*k/16), k = 0..7.
module twiddle_rom #(
    parameter int LOGN = 4
) (
    input  logic [LOGN-1:0]         idx,
    output logic signed [15:0]      w_re,
    output logic signed [15:0]      w_im
);

  always_comb begin
    unique case (idx)
      4'd0: begin w_re = 16'sd32767;  w_im = 16'sd0;      end
      4'd1: begin w_re = 16'sd30274;  w_im = -16'sd12540; end
      4'd2: begin w_re = 16'sd23170;  w_im = -16'sd23170; end
      4'd3: begin w_re = 16'sd12540;  w_im = -16'sd30274; end
      4'd4: begin w_re = 16'sd0;      w_im = -16'sd32768; end
      4'd5: begin w_re = -16'sd12540; w_im = -16'sd30274; end
      4'd6: begin w_re = -16'sd23170; w_im = -16'sd23170; end
      4'd7: begin w_re = -16'sd30274; w_im = -16'sd12540; end
      default: begin w_re = 16'sd0;   w_im = 16'sd0;      end
    endcase
  end

endmodule

`default_nettype wire
