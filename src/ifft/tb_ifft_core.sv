`timescale 1ns/1ps

module tb_ifft_core;
    localparam int N       = 16;
    localparam int DATA_W  = 16;
    localparam int FRAC_W  = 12;
    localparam int NUM_FRAMES = 3;
    localparam int TOL_LSB = 3;

    localparam real PI = 3.14159265358979323846;

    logic clk;
    logic rst_n;

    logic in_valid;
    logic in_ready;
    logic signed [DATA_W-1:0] in_re;
    logic signed [DATA_W-1:0] in_im;

    logic out_valid;
    logic out_ready;
    logic signed [DATA_W-1:0] out_re;
    logic signed [DATA_W-1:0] out_im;
    logic out_last;
    logic frame_done;

    logic signed [DATA_W-1:0] freq_re [0:N-1];
    logic signed [DATA_W-1:0] freq_im [0:N-1];
    logic signed [DATA_W-1:0] exp_re  [0:N-1];
    logic signed [DATA_W-1:0] exp_im  [0:N-1];

    int err_count;

    ifft_core #(
        .N(N),
        .DATA_W(DATA_W),
        .FRAC_W(FRAC_W)
    ) dut (
        .clk(clk),
        .rst_n(rst_n),
        .in_valid(in_valid),
        .in_ready(in_ready),
        .in_re(in_re),
        .in_im(in_im),
        .out_valid(out_valid),
        .out_ready(out_ready),
        .out_re(out_re),
        .out_im(out_im),
        .out_last(out_last),
        .frame_done(frame_done)
    );

    always #5 clk = ~clk;

    function automatic int abs_i(input int x);
        if (x < 0) return -x;
        else       return x;
    endfunction

    function automatic logic signed [DATA_W-1:0] real_to_fixed(input real x);
        int tmp;
        int max_v;
        int min_v;
        begin
            max_v = (1 << (DATA_W-1)) - 1;
            min_v = -(1 << (DATA_W-1));
            tmp = $rtoi(x * (1 << FRAC_W));
            if (tmp > max_v) tmp = max_v;
            if (tmp < min_v) tmp = min_v;
            real_to_fixed = tmp[DATA_W-1:0];
        end
    endfunction

    function automatic real fixed_to_real(input logic signed [DATA_W-1:0] x);
        fixed_to_real = $itor(x) / (1 << FRAC_W);
    endfunction

    task automatic build_random_frame_and_reference;
        int k;
        int n;
        real xre;
        real xim;
        real c;
        real s;
        real angle;
        real sum_re;
        real sum_im;
        begin
            for (k = 0; k < N; k++) begin
                // Keep values small to avoid saturation in fixed-point pipeline.
                freq_re[k] = real_to_fixed(($itor($urandom_range(-800, 800))) / 4096.0);
                freq_im[k] = real_to_fixed(($itor($urandom_range(-800, 800))) / 4096.0);
            end

            for (n = 0; n < N; n++) begin
                sum_re = 0.0;
                sum_im = 0.0;
                for (k = 0; k < N; k++) begin
                    xre = fixed_to_real(freq_re[k]);
                    xim = fixed_to_real(freq_im[k]);
                    angle = (2.0 * PI * k * n) / N;
                    c = $cos(angle);
                    s = $sin(angle);
                    sum_re += (xre * c - xim * s);
                    sum_im += (xre * s + xim * c);
                end
                exp_re[n] = real_to_fixed(sum_re / N);
                exp_im[n] = real_to_fixed(sum_im / N);
            end
        end
    endtask

    task automatic send_frame;
        int k;
        begin
            for (k = 0; k < N; k++) begin
                @(posedge clk);
                in_valid <= 1'b1;
                in_re    <= freq_re[k];
                in_im    <= freq_im[k];
                while (!in_ready) begin
                    @(posedge clk);
                end
            end
            @(posedge clk);
            in_valid <= 1'b0;
            in_re    <= '0;
            in_im    <= '0;
        end
    endtask

    task automatic collect_and_check_output(input int frame_id);
        int n;
        int dre;
        int dim;
        begin
            n = 0;
            while (n < N) begin
                @(posedge clk);
                if (out_valid && out_ready) begin
                    dre = $signed(out_re) - $signed(exp_re[n]);
                    dim = $signed(out_im) - $signed(exp_im[n]);

                    if ((abs_i(dre) > TOL_LSB) || (abs_i(dim) > TOL_LSB)) begin
                        $display("[ERR] frame=%0d n=%0d got=(%0d,%0d) exp=(%0d,%0d) diff=(%0d,%0d)",
                                 frame_id, n, $signed(out_re), $signed(out_im),
                                 $signed(exp_re[n]), $signed(exp_im[n]), dre, dim);
                        err_count = err_count + 1;
                    end
                    n = n + 1;
                end
            end
        end
    endtask

    initial begin
        int f;

        clk       = 1'b0;
        rst_n     = 1'b0;
        in_valid  = 1'b0;
        in_re     = '0;
        in_im     = '0;
        out_ready = 1'b1;
        err_count = 0;

        repeat (5) @(posedge clk);
        rst_n = 1'b1;

        for (f = 0; f < NUM_FRAMES; f++) begin
            build_random_frame_and_reference();
            send_frame();
            collect_and_check_output(f);
            $display("[INFO] Completed frame %0d", f);
        end

        if (err_count == 0) begin
            $display("[PASS] All %0d frames passed", NUM_FRAMES);
        end else begin
            $display("[FAIL] Total mismatches = %0d", err_count);
        end

        #20;
        $finish;
    end

endmodule

