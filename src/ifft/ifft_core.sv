// -----------------------------------------------------------------------------
// Module: ifft_core
// -----------------------------------------------------------------------------
// Streaming 2D IFFT core for square tiles where N = DIM*DIM and DIM is power-of-2.
//
// Processing flow:
// 1) ST_LOAD:
//    - Accept N frequency-domain bins.
//    - Store each row in bit-reversed order for DIT traversal.
// 2) ST_CALC_PASS1:
//    - Run in-place 1D IFFT on each row (complex butterflies, twiddle multiply).
// 3) ST_TRANSPOSE:
//    - Transpose pass-1 result into buf_* and apply bit-reversed row indexing
//      for second pass DIT traversal.
// 4) ST_CALC_PASS2:
//    - Run in-place 1D IFFT on transposed rows (equivalent to column pass).
// 5) ST_OUT:
//    - Stream out scaled spatial samples with final transpose addressing.
//
// Fixed-point behavior:
// - Internal width CORE_W has guard bits for growth.
// - Twiddles are DATA_W fixed-point values with FRAC_W fractional bits.
// - Final output scales by 1/N (implemented as arithmetic right-shift by LOGN).
// - Saturation is applied when narrowing to CORE_W and DATA_W.
// -----------------------------------------------------------------------------
module ifft_core #(
    parameter int N       = 64,
    parameter int DATA_W  = 16,
    parameter int FRAC_W  = 12
) (
    input  logic                         clk,
    input  logic                         rst_n,

    // Input stream: one complex bin per valid handshake.
    input  logic                         in_valid,
    output logic                         in_ready,
    input  logic signed [DATA_W-1:0]     in_re,
    input  logic signed [DATA_W-1:0]     in_im,

    // Output stream: one complex sample per valid handshake.
    output logic                         out_valid,
    input  logic                         out_ready,
    output logic signed [DATA_W-1:0]     out_re,
    output logic signed [DATA_W-1:0]     out_im,
    output logic                         out_last,

    output logic                         frame_done
);
    // LOGN: log2(total points), LOGDIM: log2(side dimension), DIM: side length.
    localparam int LOGN   = $clog2(N);
    localparam int LOGDIM = LOGN / 2;
    localparam int DIM    = (1 << LOGDIM);

    // Internal arithmetic widths sized for safe intermediate headroom.
    localparam int CORE_W = DATA_W + LOGN + 2;      // internal growth guard bits
    localparam int MUL_W  = CORE_W + DATA_W;        // v(core) * w(data)
    localparam int LONG_W = MUL_W + 4;

    localparam real PI = 3.14159265358979323846;

    // Synthesis-time parameter checks.
    initial begin
        if ((1 << LOGN) != N) begin
            $error("ifft_core: N (%0d) must be a power of 2", N);
        end
        if ((LOGN % 2) != 0) begin
            $error("ifft_core: N (%0d) must be an even-log square (DIM^2)", N);
        end
        if ((DIM * DIM) != N) begin
            $error("ifft_core: N (%0d) must satisfy DIM*DIM == N", N);
        end
    end

    typedef enum logic [2:0] {
        // Load full frame from input stream.
        ST_LOAD      = 3'd0,
        // First 1D IFFT pass over rows.
        ST_CALC_PASS1= 3'd1,
        // Transpose intermediate frame for second pass.
        ST_TRANSPOSE = 3'd2,
        // Second 1D IFFT pass (transposed rows => original columns).
        ST_CALC_PASS2= 3'd3,
        // Stream final spatial-domain frame to output.
        ST_OUT       = 3'd4
    } state_t;

    state_t state;

    // Main working memories for in-place passes.
    logic signed [CORE_W-1:0] mem_re [0:N-1];
    logic signed [CORE_W-1:0] mem_im [0:N-1];

    // Transpose/intermediate buffer between pass 1 and pass 2.
    logic signed [CORE_W-1:0] buf_re [0:N-1];
    logic signed [CORE_W-1:0] buf_im [0:N-1];

    // Twiddle ROM for 1D IFFT of size DIM: W_DIM^k = exp(+j*2*pi*k/DIM).
    logic signed [DATA_W-1:0] tw_re [0:(DIM/2)-1];
    logic signed [DATA_W-1:0] tw_im [0:(DIM/2)-1];

    int load_cnt;
    int out_idx;
    int trans_cnt;

    int row_idx;
    int stage;
    int m;
    int half;
    int k_base;
    int j_idx;

    localparam logic signed [CORE_W-1:0] CORE_MAX = {1'b0, {(CORE_W-1){1'b1}}};
    localparam logic signed [CORE_W-1:0] CORE_MIN = {1'b1, {(CORE_W-1){1'b0}}};
    localparam logic signed [DATA_W-1:0] DATA_MAX = {1'b0, {(DATA_W-1){1'b1}}};
    localparam logic signed [DATA_W-1:0] DATA_MIN = {1'b1, {(DATA_W-1){1'b0}}};

    // Bit-reverse helper over one DIM-sized axis.
    function automatic int bit_reverse_dim(input int value);
        int b;
        int r;
        begin
            r = 0;
            for (b = 0; b < LOGDIM; b++) begin
                r = (r << 1) | ((value >> b) & 1);
            end
            return r;
        end
    endfunction

    // Saturate wide intermediates into CORE_W internal range.
    function automatic logic signed [CORE_W-1:0] sat_to_core(
        input logic signed [LONG_W-1:0] x
    );
        logic signed [LONG_W-1:0] max_ext;
        logic signed [LONG_W-1:0] min_ext;
        begin
            max_ext = $signed({{(LONG_W-CORE_W){1'b0}}, CORE_MAX});
            min_ext = $signed({{(LONG_W-CORE_W){1'b1}}, CORE_MIN});
            if (x > max_ext) begin
                sat_to_core = CORE_MAX;
            end else if (x < min_ext) begin
                sat_to_core = CORE_MIN;
            end else begin
                sat_to_core = x[CORE_W-1:0];
            end
        end
    endfunction

    // Saturate wide intermediates into DATA_W output range.
    function automatic logic signed [DATA_W-1:0] sat_to_data(
        input logic signed [LONG_W-1:0] x
    );
        logic signed [LONG_W-1:0] max_ext;
        logic signed [LONG_W-1:0] min_ext;
        begin
            max_ext = $signed({{(LONG_W-DATA_W){1'b0}}, DATA_MAX});
            min_ext = $signed({{(LONG_W-DATA_W){1'b1}}, DATA_MIN});
            if (x > max_ext) begin
                sat_to_data = DATA_MAX;
            end else if (x < min_ext) begin
                sat_to_data = DATA_MIN;
            end else begin
                sat_to_data = x[DATA_W-1:0];
            end
        end
    endfunction

    // Quantize real-valued constants (twiddles) into DATA_W fixed-point.
    function automatic logic signed [DATA_W-1:0] quant_real_to_fixed(input real x);
        int tmp;
        begin
            tmp = $rtoi(x * (1 << FRAC_W));
            if (tmp > DATA_MAX) begin
                quant_real_to_fixed = DATA_MAX;
            end else if (tmp < DATA_MIN) begin
                quant_real_to_fixed = DATA_MIN;
            end else begin
                quant_real_to_fixed = tmp[DATA_W-1:0];
            end
        end
    endfunction

    // Final 1/N scaling for IFFT output with saturation to DATA_W.
    function automatic logic signed [DATA_W-1:0] scale_ifft(
        input logic signed [CORE_W-1:0] x
    );
        logic signed [LONG_W-1:0] shifted;
        begin
            shifted    = $signed(x) >>> LOGN; // divide by DIM*DIM (=N)
            scale_ifft = sat_to_data(shifted);
        end
    endfunction

    // Twiddle ROM initialization for one DIM-point inverse FFT kernel.
    integer ti;
    real angle;
    initial begin
        for (ti = 0; ti < DIM/2; ti++) begin
            angle   = (2.0 * PI * ti) / DIM;
            tw_re[ti] = quant_real_to_fixed($cos(angle));
            tw_im[ti] = quant_real_to_fixed($sin(angle));
        end
    end

    // Streaming interface behavior and output addressing.
    always_comb begin
        int out_row;
        int out_col;
        int out_mem_idx;

        out_row = 0;
        out_col = 0;
        out_mem_idx = 0;

        in_ready  = (state == ST_LOAD);
        out_valid = (state == ST_OUT);
        out_last  = (state == ST_OUT) && (out_idx == (N-1));

        if (state == ST_OUT) begin
            out_row = out_idx / DIM;
            out_col = out_idx % DIM;
            out_mem_idx = (out_col * DIM) + out_row;
            out_re = scale_ifft(buf_re[out_mem_idx]);
            out_im = scale_ifft(buf_im[out_mem_idx]);
        end else begin
            out_re = '0;
            out_im = '0;
        end
    end

    // Main sequential engine for 2-pass in-place 2D IFFT.
    always_ff @(posedge clk or negedge rst_n) begin
        int wr_row;
        int wr_col;
        int wr_idx;

        int row_off;
        int idx_a;
        int idx_b;
        int tw_idx;

        int tr_row;
        int tr_col;
        int tr_idx;

        logic signed [CORE_W-1:0] u_re;
        logic signed [CORE_W-1:0] u_im;
        logic signed [CORE_W-1:0] v_re;
        logic signed [CORE_W-1:0] v_im;
        logic signed [DATA_W-1:0] w_re;
        logic signed [DATA_W-1:0] w_im;

        logic signed [MUL_W-1:0] p1;
        logic signed [MUL_W-1:0] p2;
        logic signed [MUL_W-1:0] p3;
        logic signed [MUL_W-1:0] p4;

        logic signed [LONG_W-1:0] t_re_full;
        logic signed [LONG_W-1:0] t_im_full;
        logic signed [CORE_W-1:0] t_re;
        logic signed [CORE_W-1:0] t_im;

        logic signed [LONG_W-1:0] sum_re;
        logic signed [LONG_W-1:0] sum_im;
        logic signed [LONG_W-1:0] diff_re;
        logic signed [LONG_W-1:0] diff_im;

        if (!rst_n) begin
            state      <= ST_LOAD;
            load_cnt   <= 0;
            out_idx    <= 0;
            trans_cnt  <= 0;
            frame_done <= 1'b0;

            row_idx <= 0;
            stage   <= 0;
            m       <= 2;
            half    <= 1;
            k_base  <= 0;
            j_idx   <= 0;
        end else begin
            frame_done <= 1'b0;

            case (state)
                ST_LOAD: begin
                    if (in_valid && in_ready) begin
                        // For pass-1 DIT, store each row at bit-reversed column index.
                        wr_row = load_cnt / DIM;
                        wr_col = bit_reverse_dim(load_cnt % DIM);
                        wr_idx = (wr_row * DIM) + wr_col;

                        mem_re[wr_idx] <= $signed(in_re);
                        mem_im[wr_idx] <= $signed(in_im);

                        if (load_cnt == N-1) begin
                            state    <= ST_CALC_PASS1;
                            load_cnt <= 0;

                            row_idx <= 0;
                            stage   <= 0;
                            m       <= 2;
                            half    <= 1;
                            k_base  <= 0;
                            j_idx   <= 0;
                        end else begin
                            load_cnt <= load_cnt + 1;
                        end
                    end
                end

                ST_CALC_PASS1: begin
                    // In-place row-wise DIT butterfly update on mem_*.
                    row_off = row_idx * DIM;
                    idx_a   = row_off + k_base + j_idx;
                    idx_b   = idx_a + half;
                    tw_idx  = j_idx << (LOGDIM - stage - 1);

                    u_re = mem_re[idx_a];
                    u_im = mem_im[idx_a];
                    v_re = mem_re[idx_b];
                    v_im = mem_im[idx_b];
                    w_re = tw_re[tw_idx];
                    w_im = tw_im[tw_idx];

                    p1 = v_re * w_re;
                    p2 = v_im * w_im;
                    p3 = v_re * w_im;
                    p4 = v_im * w_re;

                    t_re_full = $signed(p1) - $signed(p2);
                    t_im_full = $signed(p3) + $signed(p4);

                    t_re = sat_to_core($signed(t_re_full >>> FRAC_W));
                    t_im = sat_to_core($signed(t_im_full >>> FRAC_W));

                    sum_re  = $signed(u_re) + $signed(t_re);
                    sum_im  = $signed(u_im) + $signed(t_im);
                    diff_re = $signed(u_re) - $signed(t_re);
                    diff_im = $signed(u_im) - $signed(t_im);

                    mem_re[idx_a] <= sat_to_core(sum_re);
                    mem_im[idx_a] <= sat_to_core(sum_im);
                    mem_re[idx_b] <= sat_to_core(diff_re);
                    mem_im[idx_b] <= sat_to_core(diff_im);

                    if (j_idx + 1 < half) begin
                        j_idx <= j_idx + 1;
                    end else begin
                        j_idx <= 0;
                        if (k_base + m < DIM) begin
                            k_base <= k_base + m;
                        end else begin
                            if (stage == LOGDIM - 1) begin
                                if (row_idx == DIM - 1) begin
                                    state    <= ST_TRANSPOSE;
                                    trans_cnt <= 0;
                                end else begin
                                    row_idx <= row_idx + 1;
                                    stage   <= 0;
                                    m       <= 2;
                                    half    <= 1;
                                    k_base  <= 0;
                                    j_idx   <= 0;
                                end
                            end else begin
                                stage  <= stage + 1;
                                m      <= (m << 1);
                                half   <= (half << 1);
                                k_base <= 0;
                                j_idx  <= 0;
                            end
                        end
                    end
                end

                ST_TRANSPOSE: begin
                    // Transpose and bit-reverse row index to prepare pass-2 DIT layout.
                    tr_row = trans_cnt / DIM;
                    tr_col = trans_cnt % DIM;
                    tr_idx = (tr_col * DIM) + bit_reverse_dim(tr_row);

                    buf_re[tr_idx] <= mem_re[trans_cnt];
                    buf_im[tr_idx] <= mem_im[trans_cnt];

                    if (trans_cnt == N-1) begin
                        state     <= ST_CALC_PASS2;
                        trans_cnt <= 0;

                        row_idx <= 0;
                        stage   <= 0;
                        m       <= 2;
                        half    <= 1;
                        k_base  <= 0;
                        j_idx   <= 0;
                    end else begin
                        trans_cnt <= trans_cnt + 1;
                    end
                end

                ST_CALC_PASS2: begin
                    // In-place second pass on buf_* (equivalent to column IFFT).
                    row_off = row_idx * DIM;
                    idx_a   = row_off + k_base + j_idx;
                    idx_b   = idx_a + half;
                    tw_idx  = j_idx << (LOGDIM - stage - 1);

                    u_re = buf_re[idx_a];
                    u_im = buf_im[idx_a];
                    v_re = buf_re[idx_b];
                    v_im = buf_im[idx_b];
                    w_re = tw_re[tw_idx];
                    w_im = tw_im[tw_idx];

                    p1 = v_re * w_re;
                    p2 = v_im * w_im;
                    p3 = v_re * w_im;
                    p4 = v_im * w_re;

                    t_re_full = $signed(p1) - $signed(p2);
                    t_im_full = $signed(p3) + $signed(p4);

                    t_re = sat_to_core($signed(t_re_full >>> FRAC_W));
                    t_im = sat_to_core($signed(t_im_full >>> FRAC_W));

                    sum_re  = $signed(u_re) + $signed(t_re);
                    sum_im  = $signed(u_im) + $signed(t_im);
                    diff_re = $signed(u_re) - $signed(t_re);
                    diff_im = $signed(u_im) - $signed(t_im);

                    buf_re[idx_a] <= sat_to_core(sum_re);
                    buf_im[idx_a] <= sat_to_core(sum_im);
                    buf_re[idx_b] <= sat_to_core(diff_re);
                    buf_im[idx_b] <= sat_to_core(diff_im);

                    if (j_idx + 1 < half) begin
                        j_idx <= j_idx + 1;
                    end else begin
                        j_idx <= 0;
                        if (k_base + m < DIM) begin
                            k_base <= k_base + m;
                        end else begin
                            if (stage == LOGDIM - 1) begin
                                if (row_idx == DIM - 1) begin
                                    state   <= ST_OUT;
                                    out_idx <= 0;
                                end else begin
                                    row_idx <= row_idx + 1;
                                    stage   <= 0;
                                    m       <= 2;
                                    half    <= 1;
                                    k_base  <= 0;
                                    j_idx   <= 0;
                                end
                            end else begin
                                stage  <= stage + 1;
                                m      <= (m << 1);
                                half   <= (half << 1);
                                k_base <= 0;
                                j_idx  <= 0;
                            end
                        end
                    end
                end

                ST_OUT: begin
                    // Output one sample per accepted handshake; pulse frame_done on last.
                    if (out_valid && out_ready) begin
                        if (out_idx == N-1) begin
                            state      <= ST_LOAD;
                            out_idx    <= 0;
                            frame_done <= 1'b1;
                        end else begin
                            out_idx <= out_idx + 1;
                        end
                    end
                end

                default: begin
                    state <= ST_LOAD;
                end
            endcase
        end
    end

endmodule
