`default_nettype none

// Auto-generated Q1.15 twiddle ROM for W256^k, k=0..127.
module twiddle_rom #(
    parameter int LOGN = 8
) (
    input  logic [LOGN-1:0]         idx,
    output logic signed [15:0]      w_re,
    output logic signed [15:0]      w_im
);

  always_comb begin
    unique case (idx)
      8'd0: begin w_re = 16'sd32767; w_im = 16'sd0; end
      8'd1: begin w_re = 16'sd32758; w_im = -16'sd804; end
      8'd2: begin w_re = 16'sd32729; w_im = -16'sd1608; end
      8'd3: begin w_re = 16'sd32679; w_im = -16'sd2411; end
      8'd4: begin w_re = 16'sd32610; w_im = -16'sd3212; end
      8'd5: begin w_re = 16'sd32522; w_im = -16'sd4011; end
      8'd6: begin w_re = 16'sd32413; w_im = -16'sd4808; end
      8'd7: begin w_re = 16'sd32286; w_im = -16'sd5602; end
      8'd8: begin w_re = 16'sd32138; w_im = -16'sd6393; end
      8'd9: begin w_re = 16'sd31972; w_im = -16'sd7180; end
      8'd10: begin w_re = 16'sd31786; w_im = -16'sd7962; end
      8'd11: begin w_re = 16'sd31581; w_im = -16'sd8740; end
      8'd12: begin w_re = 16'sd31357; w_im = -16'sd9512; end
      8'd13: begin w_re = 16'sd31114; w_im = -16'sd10279; end
      8'd14: begin w_re = 16'sd30853; w_im = -16'sd11039; end
      8'd15: begin w_re = 16'sd30572; w_im = -16'sd11793; end
      8'd16: begin w_re = 16'sd30274; w_im = -16'sd12540; end
      8'd17: begin w_re = 16'sd29957; w_im = -16'sd13279; end
      8'd18: begin w_re = 16'sd29622; w_im = -16'sd14010; end
      8'd19: begin w_re = 16'sd29269; w_im = -16'sd14733; end
      8'd20: begin w_re = 16'sd28899; w_im = -16'sd15447; end
      8'd21: begin w_re = 16'sd28511; w_im = -16'sd16151; end
      8'd22: begin w_re = 16'sd28106; w_im = -16'sd16846; end
      8'd23: begin w_re = 16'sd27684; w_im = -16'sd17531; end
      8'd24: begin w_re = 16'sd27246; w_im = -16'sd18205; end
      8'd25: begin w_re = 16'sd26791; w_im = -16'sd18868; end
      8'd26: begin w_re = 16'sd26320; w_im = -16'sd19520; end
      8'd27: begin w_re = 16'sd25833; w_im = -16'sd20160; end
      8'd28: begin w_re = 16'sd25330; w_im = -16'sd20788; end
      8'd29: begin w_re = 16'sd24812; w_im = -16'sd21403; end
      8'd30: begin w_re = 16'sd24279; w_im = -16'sd22006; end
      8'd31: begin w_re = 16'sd23732; w_im = -16'sd22595; end
      8'd32: begin w_re = 16'sd23170; w_im = -16'sd23170; end
      8'd33: begin w_re = 16'sd22595; w_im = -16'sd23732; end
      8'd34: begin w_re = 16'sd22006; w_im = -16'sd24279; end
      8'd35: begin w_re = 16'sd21403; w_im = -16'sd24812; end
      8'd36: begin w_re = 16'sd20788; w_im = -16'sd25330; end
      8'd37: begin w_re = 16'sd20160; w_im = -16'sd25833; end
      8'd38: begin w_re = 16'sd19520; w_im = -16'sd26320; end
      8'd39: begin w_re = 16'sd18868; w_im = -16'sd26791; end
      8'd40: begin w_re = 16'sd18205; w_im = -16'sd27246; end
      8'd41: begin w_re = 16'sd17531; w_im = -16'sd27684; end
      8'd42: begin w_re = 16'sd16846; w_im = -16'sd28106; end
      8'd43: begin w_re = 16'sd16151; w_im = -16'sd28511; end
      8'd44: begin w_re = 16'sd15447; w_im = -16'sd28899; end
      8'd45: begin w_re = 16'sd14733; w_im = -16'sd29269; end
      8'd46: begin w_re = 16'sd14010; w_im = -16'sd29622; end
      8'd47: begin w_re = 16'sd13279; w_im = -16'sd29957; end
      8'd48: begin w_re = 16'sd12540; w_im = -16'sd30274; end
      8'd49: begin w_re = 16'sd11793; w_im = -16'sd30572; end
      8'd50: begin w_re = 16'sd11039; w_im = -16'sd30853; end
      8'd51: begin w_re = 16'sd10279; w_im = -16'sd31114; end
      8'd52: begin w_re = 16'sd9512; w_im = -16'sd31357; end
      8'd53: begin w_re = 16'sd8740; w_im = -16'sd31581; end
      8'd54: begin w_re = 16'sd7962; w_im = -16'sd31786; end
      8'd55: begin w_re = 16'sd7180; w_im = -16'sd31972; end
      8'd56: begin w_re = 16'sd6393; w_im = -16'sd32138; end
      8'd57: begin w_re = 16'sd5602; w_im = -16'sd32286; end
      8'd58: begin w_re = 16'sd4808; w_im = -16'sd32413; end
      8'd59: begin w_re = 16'sd4011; w_im = -16'sd32522; end
      8'd60: begin w_re = 16'sd3212; w_im = -16'sd32610; end
      8'd61: begin w_re = 16'sd2411; w_im = -16'sd32679; end
      8'd62: begin w_re = 16'sd1608; w_im = -16'sd32729; end
      8'd63: begin w_re = 16'sd804; w_im = -16'sd32758; end
      8'd64: begin w_re = 16'sd0; w_im = -16'sd32768; end
      8'd65: begin w_re = -16'sd804; w_im = -16'sd32758; end
      8'd66: begin w_re = -16'sd1608; w_im = -16'sd32729; end
      8'd67: begin w_re = -16'sd2411; w_im = -16'sd32679; end
      8'd68: begin w_re = -16'sd3212; w_im = -16'sd32610; end
      8'd69: begin w_re = -16'sd4011; w_im = -16'sd32522; end
      8'd70: begin w_re = -16'sd4808; w_im = -16'sd32413; end
      8'd71: begin w_re = -16'sd5602; w_im = -16'sd32286; end
      8'd72: begin w_re = -16'sd6393; w_im = -16'sd32138; end
      8'd73: begin w_re = -16'sd7180; w_im = -16'sd31972; end
      8'd74: begin w_re = -16'sd7962; w_im = -16'sd31786; end
      8'd75: begin w_re = -16'sd8740; w_im = -16'sd31581; end
      8'd76: begin w_re = -16'sd9512; w_im = -16'sd31357; end
      8'd77: begin w_re = -16'sd10279; w_im = -16'sd31114; end
      8'd78: begin w_re = -16'sd11039; w_im = -16'sd30853; end
      8'd79: begin w_re = -16'sd11793; w_im = -16'sd30572; end
      8'd80: begin w_re = -16'sd12540; w_im = -16'sd30274; end
      8'd81: begin w_re = -16'sd13279; w_im = -16'sd29957; end
      8'd82: begin w_re = -16'sd14010; w_im = -16'sd29622; end
      8'd83: begin w_re = -16'sd14733; w_im = -16'sd29269; end
      8'd84: begin w_re = -16'sd15447; w_im = -16'sd28899; end
      8'd85: begin w_re = -16'sd16151; w_im = -16'sd28511; end
      8'd86: begin w_re = -16'sd16846; w_im = -16'sd28106; end
      8'd87: begin w_re = -16'sd17531; w_im = -16'sd27684; end
      8'd88: begin w_re = -16'sd18205; w_im = -16'sd27246; end
      8'd89: begin w_re = -16'sd18868; w_im = -16'sd26791; end
      8'd90: begin w_re = -16'sd19520; w_im = -16'sd26320; end
      8'd91: begin w_re = -16'sd20160; w_im = -16'sd25833; end
      8'd92: begin w_re = -16'sd20788; w_im = -16'sd25330; end
      8'd93: begin w_re = -16'sd21403; w_im = -16'sd24812; end
      8'd94: begin w_re = -16'sd22006; w_im = -16'sd24279; end
      8'd95: begin w_re = -16'sd22595; w_im = -16'sd23732; end
      8'd96: begin w_re = -16'sd23170; w_im = -16'sd23170; end
      8'd97: begin w_re = -16'sd23732; w_im = -16'sd22595; end
      8'd98: begin w_re = -16'sd24279; w_im = -16'sd22006; end
      8'd99: begin w_re = -16'sd24812; w_im = -16'sd21403; end
      8'd100: begin w_re = -16'sd25330; w_im = -16'sd20788; end
      8'd101: begin w_re = -16'sd25833; w_im = -16'sd20160; end
      8'd102: begin w_re = -16'sd26320; w_im = -16'sd19520; end
      8'd103: begin w_re = -16'sd26791; w_im = -16'sd18868; end
      8'd104: begin w_re = -16'sd27246; w_im = -16'sd18205; end
      8'd105: begin w_re = -16'sd27684; w_im = -16'sd17531; end
      8'd106: begin w_re = -16'sd28106; w_im = -16'sd16846; end
      8'd107: begin w_re = -16'sd28511; w_im = -16'sd16151; end
      8'd108: begin w_re = -16'sd28899; w_im = -16'sd15447; end
      8'd109: begin w_re = -16'sd29269; w_im = -16'sd14733; end
      8'd110: begin w_re = -16'sd29622; w_im = -16'sd14010; end
      8'd111: begin w_re = -16'sd29957; w_im = -16'sd13279; end
      8'd112: begin w_re = -16'sd30274; w_im = -16'sd12540; end
      8'd113: begin w_re = -16'sd30572; w_im = -16'sd11793; end
      8'd114: begin w_re = -16'sd30853; w_im = -16'sd11039; end
      8'd115: begin w_re = -16'sd31114; w_im = -16'sd10279; end
      8'd116: begin w_re = -16'sd31357; w_im = -16'sd9512; end
      8'd117: begin w_re = -16'sd31581; w_im = -16'sd8740; end
      8'd118: begin w_re = -16'sd31786; w_im = -16'sd7962; end
      8'd119: begin w_re = -16'sd31972; w_im = -16'sd7180; end
      8'd120: begin w_re = -16'sd32138; w_im = -16'sd6393; end
      8'd121: begin w_re = -16'sd32286; w_im = -16'sd5602; end
      8'd122: begin w_re = -16'sd32413; w_im = -16'sd4808; end
      8'd123: begin w_re = -16'sd32522; w_im = -16'sd4011; end
      8'd124: begin w_re = -16'sd32610; w_im = -16'sd3212; end
      8'd125: begin w_re = -16'sd32679; w_im = -16'sd2411; end
      8'd126: begin w_re = -16'sd32729; w_im = -16'sd1608; end
      8'd127: begin w_re = -16'sd32758; w_im = -16'sd804; end
      default: begin w_re = 16'sd0; w_im = 16'sd0; end
    endcase
  end

endmodule

`default_nettype wire
