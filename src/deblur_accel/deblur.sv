/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off UNUSEDSIGNAL */
`default_nettype none

// Top-level deblur accelerator:
// image -> FFT2D -> Wiener filter generation -> complex multiply -> IFFT2D.
// One IMG_N x IMG_N frame is buffered and processed per run.
module deblur #(
    // Tile side length.
    parameter int IMG_N      = 4,
    // log2(IMG_N), forwarded to FFT.
    parameter int FFT_LOGN   = 2,
    // Number of parallel 1D FFT lanes in fft2d_core.
    parameter int FFT_LANES  = 1,
    // Internal datapath width for filter/mult/IFFT.
    parameter int DATA_W     = 10,
    // Fractional bits for filter/IFFT fixed-point domain.
    parameter int FRAC_W     = 8,
    // Fractional bits used by complex multiplier normalization.
    parameter int MULT_FRAC  = 8,
    // 1 enables output saturation in complex multiplier.
    parameter int MULT_SAT   = 1,
    // Optional gain shift before IFFT input.
    parameter int PRE_IFFT_SHIFT = 0,
    // FFT output index mapping mode into natural-frequency storage.
    parameter int FFT_TO_IFFT_MAP = 0
) (
    // Clock/reset.
    input  logic                         clk,
    input  logic                         rst_n,

    // Image input stream (real-valued grayscale samples).
    input  logic                         img_valid,
    output logic                         img_ready,
    input  logic signed [15:0]           img_re,

    // Frequency-response input stream for Wiener block.
    input  logic                         h_valid,
    output logic                         h_ready,
    input  logic signed [DATA_W-1:0]     h_re,
    input  logic signed [DATA_W-1:0]     h_im,
    input  logic        [DATA_W-1:0]     k_cfg,

    // Deblurred output stream.
    output logic                         out_valid,
    input  logic                         out_ready,
    output logic signed [DATA_W-1:0]     out_re,
    output logic signed [DATA_W-1:0]     out_im,
    output logic                         out_last,

    // Status/performance.
    output logic                         done,
    output logic                         fft_perf_done,
    output logic [63:0]                  fft_perf_cycles
);

    localparam int TOT      = IMG_N * IMG_N;
    localparam int FFT_W    = 16;
    localparam int FFT_AW   = (TOT <= 1) ? 1 : $clog2(TOT);
    localparam int IMG_LOGN = (IMG_N <= 1) ? 1 : $clog2(IMG_N);
    typedef logic signed [DATA_W-1:0] data_t;

    typedef enum logic [2:0] {
        // Accept/capture image and H in parallel; capture FFT outputs.
        ST_PRELOAD   = 3'd0,
        // Image/H done, but still waiting remaining FFT bins.
        ST_WAIT_FFT  = 3'd1,
        // Feed Y(u,v)*G(u,v) into IFFT.
        ST_FEED_IFFT = 3'd2,
        // Drain IFFT output stream.
        ST_WAIT_OUT  = 3'd3,
        // Completion pulse state.
        ST_DONE      = 3'd4
    } state_t;

    state_t st;

    // Frame memories:
    // g_* stores Wiener output G(u,v); y_* stores FFT(image) bins.
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

    // Bit-reverse helper over one IMG_N dimension.
    function automatic int bitrev_dim(input int value);
        int b;
        int r;
        begin
            r = 0;
            for (b = 0; b < IMG_LOGN; b++) begin
                r = (r << 1) | ((value >> b) & 1);
            end
            bitrev_dim = r;
        end
    endfunction

    // Convert FFT stream order to selected storage order for IFFT feeding.
    function automatic logic [FFT_AW-1:0] fft_raw_to_nat(input logic [FFT_AW-1:0] raw_idx);
        int row;
        int col;
        int rr;
        int cc;
        int idx_i;
        begin
            row = raw_idx / IMG_N;
            col = raw_idx % IMG_N;
            unique case (FFT_TO_IFFT_MAP)
                0: begin
                    rr = bitrev_dim(row);
                    cc = bitrev_dim(col);
                    idx_i = rr * IMG_N + cc;
                end
                1: begin
                    idx_i = raw_idx;
                end
                2: begin
                    rr = bitrev_dim(col);
                    cc = bitrev_dim(row);
                    idx_i = rr * IMG_N + cc;
                end
                3: begin
                    idx_i = col * IMG_N + row;
                end
                default: begin
                    rr = bitrev_dim(row);
                    cc = bitrev_dim(col);
                    idx_i = rr * IMG_N + cc;
                end
            endcase
            fft_raw_to_nat = FFT_AW'(idx_i);
        end
    endfunction

    // Convert 16-bit FFT output into configured DATA_W safely.
    function automatic logic signed [DATA_W-1:0] fft_to_data(
        input logic signed [FFT_W-1:0] x
    );
        begin
            if (DATA_W >= FFT_W) begin
                // Signed assignment performs sign-extension when widening.
                fft_to_data = data_t'(x);
            end else begin
                // Narrowing path: arithmetic down-shift before truncation.
                fft_to_data = data_t'($signed(x) >>> (FFT_W - DATA_W));
            end
        end
    endfunction

    // Left-shift with saturation to keep pre-IFFT scaling bounded.
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

    // Handshake coupling for FFT preload path.
    assign fft_in_valid = (st == ST_PRELOAD) && !img_loaded && img_valid;
    assign img_ready    = (st == ST_PRELOAD) && !img_loaded && fft_in_ready;
    assign fft_in_re    = img_re;
    assign fft_in_im    = '0;

    // Handshake coupling for Wiener preload path.
    assign filt_valid_in = (st == ST_PRELOAD) && !h_loaded && h_valid;
    assign h_ready       = (st == ST_PRELOAD) && !h_loaded && filt_ready_in;
    assign filt_ready_out = 1'b1;

    always_comb begin
        // Read aligned Y and G bins for one complex multiply per cycle.
        mult_in_valid = (st == ST_FEED_IFFT) && !feed_done;
        mult_a_re     = y_re_mem[feed_cnt];
        mult_a_im     = y_im_mem[feed_cnt];
        mult_b_re     = feed_done ? '0 : g_re_mem[feed_cnt];
        mult_b_im     = feed_done ? '0 : g_im_mem[feed_cnt];
    end

    // Optional pre-IFFT gain compensation (power-of-two scaling).
    assign ifft_pre_re = sat_shift_left_pow2(mult_y_re, PRE_IFFT_SHIFT);
    assign ifft_pre_im = sat_shift_left_pow2(mult_y_im, PRE_IFFT_SHIFT);

    assign ifft_in_valid = mult_out_valid;
    assign mult_out_ready = ifft_in_ready;
    assign ifft_in_re = ifft_pre_re;
    assign ifft_in_im = ifft_pre_im;

    // Keep consuming FFT output while preloading and wait phase.
    assign fft_out_ready = (st == ST_PRELOAD) || (st == ST_WAIT_FFT);
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

    // Main control/FSM and full-frame bookkeeping.
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            st         <= ST_PRELOAD;
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

            // Capture full G(u,v) frame from Wiener stream.
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

            // Capture FFT frame and reorder indices as configured.
            if (fft_out_valid && fft_out_ready && !fft_loaded) begin
                y_re_mem[fft_raw_to_nat(y_wr_cnt)] <= fft_to_data(fft_out_re);
                y_im_mem[fft_raw_to_nat(y_wr_cnt)] <= fft_to_data(fft_out_im);
                if (y_wr_cnt == FFT_AW'(TOT - 1)) begin
                    y_wr_cnt <= y_wr_cnt;
                    fft_loaded <= 1'b1;
                end else begin
                    y_wr_cnt <= y_wr_cnt + 1'b1;
                end
            end

            // Count outgoing IFFT samples until full tile is drained.
            if (ifft_out_valid && ifft_out_ready && !out_done) begin
                if (out_cnt == FFT_AW'(TOT - 1)) begin
                    out_cnt <= out_cnt;
                    out_done <= 1'b1;
                end else begin
                    out_cnt <= out_cnt + 1'b1;
                end
            end

            case (st)
                ST_PRELOAD: begin
                    // Accept both preload streams; transition once both loaded.
                    if (h_valid && h_ready && !h_loaded) begin
                        if (h_in_cnt == FFT_AW'(TOT - 1)) begin
                            h_in_cnt <= h_in_cnt;
                        end else begin
                            h_in_cnt <= h_in_cnt + 1'b1;
                        end
                    end
                    if (h_loaded) begin
                        h_in_cnt <= h_in_cnt;
                    end
                    if (img_valid && img_ready && !img_loaded) begin
                        if (img_in_cnt == FFT_AW'(TOT - 1)) begin
                            img_in_cnt <= img_in_cnt;
                            img_loaded <= 1'b1;
                        end else begin
                            img_in_cnt <= img_in_cnt + 1'b1;
                        end
                    end
                    if (h_loaded && img_loaded) begin
                        if (fft_loaded) begin
                            st <= ST_FEED_IFFT;
                        end else begin
                            st <= ST_WAIT_FFT;
                        end
                    end
                end

                ST_WAIT_FFT: begin
                    // Wait for remaining FFT bins if preload finished early.
                    if (fft_loaded) begin
                        st <= ST_FEED_IFFT;
                    end
                end

                ST_FEED_IFFT: begin
                    // Feed all bins through complex multiply into IFFT.
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
                    // Wait until all IFFT outputs are accepted downstream.
                    if (out_done) begin
                        st <= ST_DONE;
                        done <= 1'b1;
                    end
                end

                ST_DONE: begin
                    // One-cycle completion indication.
                    done <= 1'b1;
                end

                default: begin
                    st <= ST_DONE;
                end
            endcase
        end
    end

endmodule

/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on WIDTHEXPAND */
`default_nettype wire
