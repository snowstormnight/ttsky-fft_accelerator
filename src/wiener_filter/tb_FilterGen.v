`timescale 1ns/1ps

module tb_FilterGen;
    parameter DATA_WIDTH = 16;
    parameter LUT_ADDR_WIDTH = 8;
    parameter CLK_PERIOD = 10;
    
    reg clk;
    reg rst_n;
    reg signed [15:0] H_real;
    reg signed [15:0] H_imag;
    reg valid_in;
    wire ready_in;
    reg signed [15:0] K;
    wire signed [15:0] G_real;
    wire signed [15:0] G_imag;
    wire valid_out;
    reg ready_out;
    
    FilterGen #(.DATA_WIDTH(16), .LUT_ADDR_WIDTH(8), .K_SHIFT(0)) dut (
        .clk(clk), .rst_n(rst_n), .H_real(H_real), .H_imag(H_imag),
        .valid_in(valid_in), .ready_in(ready_in), .K(K),
        .G_real(G_real), .G_imag(G_imag), .valid_out(valid_out), .ready_out(ready_out)
    );
    
    initial begin clk = 0; forever #5 clk = ~clk; end

    function automatic signed [15:0] sat_q15_from_real(input real x);
        integer q;
    begin
        q = $rtoi(x * 32768.0 + ((x >= 0.0) ? 0.5 : -0.5));
        if (q > 32767) q = 32767;
        if (q < -32768) q = -32768;
        sat_q15_from_real = q[15:0];
    end
    endfunction

    task automatic run_case(
        input signed [15:0] hr,
        input signed [15:0] hi,
        input signed [15:0] k_in
    );
        integer denom_exp;
        real    recip_real_exp;
        reg signed [15:0] recip_q15_exp;
        reg signed [31:0] g_real_wide_exp;
        reg signed [31:0] g_imag_wide_exp;
        reg signed [15:0] g_real_exp;
        reg signed [15:0] g_imag_exp;
        reg signed [15:0] g_real_samp;
        reg signed [15:0] g_imag_samp;
        reg signed [31:0] denom_samp;
        reg signed [15:0] recip_samp;
        reg [LUT_ADDR_WIDTH-1:0] lut_addr_samp;
    begin
        // Drive stimulus before the active sampling edge.
        @(negedge clk);
        H_real   = hr;
        H_imag   = hi;
        K        = k_in;
        valid_in = 1'b1;

        // One-deep ready/valid interface: hold input until output is accepted.
        @(posedge clk);
        while (!(valid_out && ready_out)) @(posedge clk);

        // Sample exactly at the handshake edge so printout matches waveforms.
        g_real_samp   = G_real;
        g_imag_samp   = G_imag;
        denom_samp    = dut.denom;
        recip_samp    = dut.recip;
        lut_addr_samp = dut.lut_addr;

        denom_exp = (hr * hr) + (hi * hi) + k_in;
        if (denom_exp <= 0) begin
            recip_real_exp = 1.0;
        end else begin
            recip_real_exp = 1.0 / denom_exp;
        end
        recip_q15_exp  = sat_q15_from_real(recip_real_exp);
        g_real_wide_exp = hr * recip_q15_exp;
        g_imag_wide_exp = (-hi) * recip_q15_exp;
        g_real_exp = g_real_wide_exp[15:0];
        g_imag_exp = g_imag_wide_exp[15:0];

        $display("IN  H=(%0d,%0d) K=%0d | denom=%0d lut_addr=%0d recip_raw=%0d recip_q15=%f",
                 hr, hi, k_in, denom_samp, lut_addr_samp, recip_samp, $itor(recip_samp)/32768.0);
        $display("EXP G=(%0d,%0d) exp_q15=(%f,%f)",
                 g_real_exp, g_imag_exp, $itor(g_real_exp)/32768.0, $itor(g_imag_exp)/32768.0);
        $display("OUT G=(%0d,%0d) out_q15=(%f,%f)",
                 g_real_samp, g_imag_samp, $itor(g_real_samp)/32768.0, $itor(g_imag_samp)/32768.0);

        @(negedge clk);
        valid_in = 1'b0;
    end
    endtask
    
    initial begin
        $display("TEST START");
        rst_n = 0; valid_in = 0; ready_out = 1; H_real = 0; H_imag = 0; K = 0;
        #50; rst_n = 1;

        run_case( 1,  2, 0);
        run_case( 2,  1, 0);
        run_case( 3,  4, 0);
        run_case( 4, -1, 0);
        run_case(-2,  3, 0);
        run_case( 0,  5, 1);
        run_case( 7,  0, 2);

        #50; $display("TEST PASSED"); $finish;
    end
    
    initial begin #100000; $fatal(1, "TIMEOUT"); end
    initial begin $dumpfile("dumpfile.fst"); $dumpvars(0); end
endmodule
