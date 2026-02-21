`default_nettype none
`timescale 1ns/1ps

module deblur #(
    parameter int IMG_N      = 256,
    parameter int FFT_LOGN   = 8,
    parameter int FFT_LANES  = 4,
    parameter int DATA_W     = 24,
    parameter int FRAC_W     = 15,
    parameter int MULT_FRAC  = 15,
    parameter int MULT_SAT   = 1,
    parameter int PRE_IFFT_SHIFT = 0,
    parameter int FFT_TO_IFFT_MAP = 0
) (
    input  logic                         clk,
    input  logic                         rst_n,

    input  logic                         img_valid,
    output logic                         img_ready,
    input  logic signed [15:0]           img_re,

    input  logic                         h_valid,
    output logic                         h_ready,
    input  logic signed [DATA_W-1:0]     h_re,
    input  logic signed [DATA_W-1:0]     h_im,
    input  logic        [DATA_W-1:0]     k_cfg,

    output logic                         out_valid,
    input  logic                         out_ready,
    output logic signed [DATA_W-1:0]     out_re,
    output logic signed [DATA_W-1:0]     out_im,
    output logic                         out_last,

    output logic                         done,
    output logic                         fft_perf_done,
    output logic [63:0]                  fft_perf_cycles
);

    localparam int TOT      = IMG_N * IMG_N;
    localparam int FFT_W    = 16;
    localparam int FFT_AW   = (TOT <= 1) ? 1 : $clog2(TOT);
    localparam int IMG_LOGN = (IMG_N <= 1) ? 1 : $clog2(IMG_N);

    typedef enum logic [2:0] {
        ST_LOAD_H    = 3'd0,
        ST_LOAD_IMG  = 3'd1,
        ST_WAIT_FFT  = 3'd2,
        ST_FEED_IFFT = 3'd3,
        ST_WAIT_OUT  = 3'd4,
        ST_DONE      = 3'd5
    } state_t;

    state_t st;

    logic signed [DATA_W-1:0] g_re_mem [0:TOT-1];
    logic signed [DATA_W-1:0] g_im_mem [0:TOT-1];
    logic signed [DATA_W-1:0] y_re_mem [0:TOT-1];
    logic signed [DATA_W-1:0] y_im_mem [0:TOT-1];

    logic [FFT_AW-1:0] h_in_cnt;
    logic [FFT_AW-1:0] g_wr_cnt;
    logic [FFT_AW-1:0] img_in_cnt;
    logic [FFT_AW-1:0] y_wr_cnt;
    logic [FFT_AW-1:0] feed_cnt;
    logic [FFT_AW-1:0] out_cnt;

    logic h_loaded;
    logic img_loaded;
    logic fft_loaded;
    logic feed_done;
    logic out_done;

    logic fft_in_valid;
    logic fft_in_ready;
    logic signed [FFT_W-1:0] fft_in_re;
    logic signed [FFT_W-1:0] fft_in_im;
    logic fft_out_valid;
    logic fft_out_ready;
    logic signed [FFT_W-1:0] fft_out_re;
    logic signed [FFT_W-1:0] fft_out_im;

    logic filt_valid_in;
    logic filt_ready_in;
    logic filt_valid_out;
    logic filt_ready_out;
    logic signed [DATA_W-1:0] filt_g_re;
    logic signed [DATA_W-1:0] filt_g_im;

    logic mult_in_valid;
    logic mult_in_ready;
    logic mult_out_valid;
    logic mult_out_ready;
    logic signed [DATA_W-1:0] mult_a_re;
    logic signed [DATA_W-1:0] mult_a_im;
    logic signed [DATA_W-1:0] mult_b_re;
    logic signed [DATA_W-1:0] mult_b_im;
    logic signed [DATA_W-1:0] mult_y_re;
    logic signed [DATA_W-1:0] mult_y_im;
    logic signed [DATA_W-1:0] ifft_pre_re;
    logic signed [DATA_W-1:0] ifft_pre_im;

    logic ifft_in_valid;
    logic ifft_in_ready;
    logic signed [DATA_W-1:0] ifft_in_re;
    logic signed [DATA_W-1:0] ifft_in_im;
    logic ifft_out_valid;
    logic ifft_out_ready;
    logic signed [DATA_W-1:0] ifft_out_re;
    logic signed [DATA_W-1:0] ifft_out_im;
    logic ifft_out_last;
    logic ifft_frame_done;

    function automatic int bitrev_dim(input int value);
        int b;
        int r;
        begin
            r = 0;
            for (b = 0; b < IMG_LOGN; b++) begin
                r = (r << 1) | ((value >> b) & 1);
            end
            return r;
        end
    endfunction

    function automatic int nat_to_fft_raw(input int nat_idx);
        int row;
        int col;
        int rr;
        int cc;
        begin
            row = nat_idx / IMG_N;
            col = nat_idx % IMG_N;
            unique case (FFT_TO_IFFT_MAP)
                0: begin
                    rr = bitrev_dim(row);
                    cc = bitrev_dim(col);
                    nat_to_fft_raw = rr * IMG_N + cc;
                end
                1: begin
                    nat_to_fft_raw = nat_idx;
                end
                2: begin
                    rr = bitrev_dim(col);
                    cc = bitrev_dim(row);
                    nat_to_fft_raw = rr * IMG_N + cc;
                end
                3: begin
                    nat_to_fft_raw = col * IMG_N + row;
                end
                default: begin
                    rr = bitrev_dim(row);
                    cc = bitrev_dim(col);
                    nat_to_fft_raw = rr * IMG_N + cc;
                end
            endcase
        end
    endfunction

    function automatic logic signed [DATA_W-1:0] sat_shift_left_pow2(
        input logic signed [DATA_W-1:0] x,
        input int unsigned sh
    );
        localparam int EXT_W = DATA_W + 32;
        logic signed [EXT_W-1:0] x_ext;
        logic signed [EXT_W-1:0] y_ext;
        logic signed [EXT_W-1:0] max_ext;
        logic signed [EXT_W-1:0] min_ext;
        begin
            x_ext = $signed({{(EXT_W-DATA_W){x[DATA_W-1]}}, x});
            y_ext = x_ext <<< sh;
            max_ext = $signed({{(EXT_W-DATA_W){1'b0}}, {1'b0, {(DATA_W-1){1'b1}}}});
            min_ext = $signed({{(EXT_W-DATA_W){1'b1}}, {1'b1, {(DATA_W-1){1'b0}}}});
            if (y_ext > max_ext) begin
                sat_shift_left_pow2 = {1'b0, {(DATA_W-1){1'b1}}};
            end else if (y_ext < min_ext) begin
                sat_shift_left_pow2 = {1'b1, {(DATA_W-1){1'b0}}};
            end else begin
                sat_shift_left_pow2 = y_ext[DATA_W-1:0];
            end
        end
    endfunction

    assign fft_in_valid = (st == ST_LOAD_IMG) && img_valid;
    assign img_ready    = (st == ST_LOAD_IMG) && fft_in_ready;
    assign fft_in_re    = img_re;
    assign fft_in_im    = '0;

    assign filt_valid_in = (st == ST_LOAD_H) && h_valid;
    assign h_ready       = (st == ST_LOAD_H) && filt_ready_in;
    assign filt_ready_out = (st == ST_LOAD_H);

    always_comb begin
        int raw_idx;
        raw_idx = 0;
        if (!feed_done) begin
            raw_idx = nat_to_fft_raw(feed_cnt);
        end

        mult_in_valid = (st == ST_FEED_IFFT) && !feed_done;
        mult_a_re     = y_re_mem[raw_idx];
        mult_a_im     = y_im_mem[raw_idx];
        mult_b_re     = feed_done ? '0 : g_re_mem[feed_cnt];
        mult_b_im     = feed_done ? '0 : g_im_mem[feed_cnt];
    end

    assign ifft_pre_re = sat_shift_left_pow2(mult_y_re, PRE_IFFT_SHIFT);
    assign ifft_pre_im = sat_shift_left_pow2(mult_y_im, PRE_IFFT_SHIFT);

    assign ifft_in_valid = mult_out_valid;
    assign mult_out_ready = ifft_in_ready;
    assign ifft_in_re = ifft_pre_re;
    assign ifft_in_im = ifft_pre_im;

    assign fft_out_ready = (st == ST_LOAD_IMG) || (st == ST_WAIT_FFT);
    assign ifft_out_ready = out_ready;

    assign out_valid = ifft_out_valid;
    assign out_re    = ifft_out_re;
    assign out_im    = ifft_out_im;
    assign out_last  = ifft_out_last;

    fft2d_core #(
        .N(IMG_N),
        .LOGN(FFT_LOGN),
        .LANES(FFT_LANES)
    ) u_fft (
        .clk(clk),
        .rst_n(rst_n),
        .in_valid(fft_in_valid),
        .in_ready(fft_in_ready),
        .in_re(fft_in_re),
        .in_im(fft_in_im),
        .out_valid(fft_out_valid),
        .out_ready(fft_out_ready),
        .out_re(fft_out_re),
        .out_im(fft_out_im),
        .perf_done(fft_perf_done),
        .perf_cycles(fft_perf_cycles)
    );

    FilterGen #(
        .DATA_WIDTH(DATA_W),
        .FRAC_W(FRAC_W),
        .LUT_ADDR_WIDTH(8),
        .K_SHIFT(0)
    ) u_filter (
        .clk(clk),
        .rst_n(rst_n),
        .valid_in(filt_valid_in),
        .ready_in(filt_ready_in),
        .H_real(h_re),
        .H_imag(h_im),
        .K(k_cfg),
        .valid_out(filt_valid_out),
        .ready_out(filt_ready_out),
        .G_real(filt_g_re),
        .G_imag(filt_g_im)
    );

    complex_mult #(
        .IN_W(DATA_W),
        .OUT_W(DATA_W),
        .FRAC(MULT_FRAC),
        .LANES(1),
        .USE_GAUSS_3M(1'b1),
        .ROUND(1'b1),
        .SATURATE(MULT_SAT != 0),
        .PIPE1(1'b1)
    ) u_mult (
        .clk(clk),
        .rst_n(rst_n),
        .in_valid(mult_in_valid),
        .in_ready(mult_in_ready),
        .out_ready(mult_out_ready),
        .out_valid(mult_out_valid),
        .a_re(mult_a_re),
        .a_im(mult_a_im),
        .b_re(mult_b_re),
        .b_im(mult_b_im),
        .y_re(mult_y_re),
        .y_im(mult_y_im)
    );

    ifft_core #(
        .N(TOT),
        .DATA_W(DATA_W),
        .FRAC_W(FRAC_W)
    ) u_ifft (
        .clk(clk),
        .rst_n(rst_n),
        .in_valid(ifft_in_valid),
        .in_ready(ifft_in_ready),
        .in_re(ifft_in_re),
        .in_im(ifft_in_im),
        .out_valid(ifft_out_valid),
        .out_ready(ifft_out_ready),
        .out_re(ifft_out_re),
        .out_im(ifft_out_im),
        .out_last(ifft_out_last),
        .frame_done(ifft_frame_done)
    );

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            st         <= ST_LOAD_H;
            h_in_cnt   <= '0;
            g_wr_cnt   <= '0;
            img_in_cnt <= '0;
            y_wr_cnt   <= '0;
            feed_cnt   <= '0;
            out_cnt    <= '0;
            h_loaded   <= 1'b0;
            img_loaded <= 1'b0;
            fft_loaded <= 1'b0;
            feed_done  <= 1'b0;
            out_done   <= 1'b0;
            done       <= 1'b0;
        end else begin
            done <= 1'b0;

            if (filt_valid_out && filt_ready_out && !h_loaded) begin
                g_re_mem[g_wr_cnt] <= filt_g_re;
                g_im_mem[g_wr_cnt] <= filt_g_im;
                if (g_wr_cnt == FFT_AW'(TOT - 1)) begin
                    g_wr_cnt <= g_wr_cnt;
                    h_loaded <= 1'b1;
                end else begin
                    g_wr_cnt <= g_wr_cnt + 1'b1;
                end
            end

            if (fft_out_valid && fft_out_ready && !fft_loaded) begin
                y_re_mem[y_wr_cnt] <= $signed({{(DATA_W-FFT_W){fft_out_re[FFT_W-1]}}, fft_out_re});
                y_im_mem[y_wr_cnt] <= $signed({{(DATA_W-FFT_W){fft_out_im[FFT_W-1]}}, fft_out_im});
                if (y_wr_cnt == FFT_AW'(TOT - 1)) begin
                    y_wr_cnt <= y_wr_cnt;
                    fft_loaded <= 1'b1;
                end else begin
                    y_wr_cnt <= y_wr_cnt + 1'b1;
                end
            end

            if (ifft_out_valid && ifft_out_ready && !out_done) begin
                if (out_cnt == FFT_AW'(TOT - 1)) begin
                    out_cnt <= out_cnt;
                    out_done <= 1'b1;
                end else begin
                    out_cnt <= out_cnt + 1'b1;
                end
            end

            case (st)
                ST_LOAD_H: begin
                    if (h_valid && h_ready && !h_loaded) begin
                        if (h_in_cnt == FFT_AW'(TOT - 1)) begin
                            h_in_cnt <= h_in_cnt;
                        end else begin
                            h_in_cnt <= h_in_cnt + 1'b1;
                        end
                    end
                    if (h_loaded) begin
                        st <= ST_LOAD_IMG;
                    end
                end

                ST_LOAD_IMG: begin
                    if (img_valid && img_ready && !img_loaded) begin
                        if (img_in_cnt == FFT_AW'(TOT - 1)) begin
                            img_in_cnt <= img_in_cnt;
                            img_loaded <= 1'b1;
                        end else begin
                            img_in_cnt <= img_in_cnt + 1'b1;
                        end
                    end
                    if (img_loaded) begin
                        st <= ST_WAIT_FFT;
                    end
                end

                ST_WAIT_FFT: begin
                    if (fft_loaded) begin
                        st <= ST_FEED_IFFT;
                    end
                end

                ST_FEED_IFFT: begin
                    if (mult_in_valid && mult_in_ready && !feed_done) begin
                        if (feed_cnt == FFT_AW'(TOT - 1)) begin
                            feed_cnt <= feed_cnt;
                            feed_done <= 1'b1;
                        end else begin
                            feed_cnt <= feed_cnt + 1'b1;
                        end
                    end
                    if (feed_done) begin
                        st <= ST_WAIT_OUT;
                    end
                end

                ST_WAIT_OUT: begin
                    if (out_done) begin
                        st <= ST_DONE;
                        done <= 1'b1;
                    end
                end

                ST_DONE: begin
                    done <= 1'b1;
                end

                default: begin
                    st <= ST_DONE;
                end
            endcase
        end
    end

endmodule

`default_nettype wire
