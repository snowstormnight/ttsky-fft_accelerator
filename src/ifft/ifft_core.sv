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
    localparam int LOGN   = $clog2(N);
    localparam int CORE_W = DATA_W + LOGN + 2;      // internal growth guard bits
    localparam int MUL_W  = CORE_W + DATA_W;        // v(core) * w(data)
    localparam int LONG_W = MUL_W + 4;

    localparam real PI = 3.14159265358979323846;

    initial begin
        if ((1 << LOGN) != N) begin
            $error("ifft_core: N (%0d) must be a power of 2", N);
        end
    end

    typedef enum logic [1:0] {
        ST_LOAD = 2'd0,
        ST_CALC = 2'd1,
        ST_OUT  = 2'd2
    } state_t;

    state_t state;

    logic signed [CORE_W-1:0] mem_re [0:N-1];
    logic signed [CORE_W-1:0] mem_im [0:N-1];

    // Twiddle ROM: W_N^k = exp(+j*2*pi*k/N) for IFFT.
    logic signed [DATA_W-1:0] tw_re [0:(N/2)-1];
    logic signed [DATA_W-1:0] tw_im [0:(N/2)-1];

    int load_cnt;
    int out_idx;

    int stage;
    int m;
    int half;
    int k_base;
    int j_idx;

    localparam logic signed [CORE_W-1:0] CORE_MAX = {1'b0, {(CORE_W-1){1'b1}}};
    localparam logic signed [CORE_W-1:0] CORE_MIN = {1'b1, {(CORE_W-1){1'b0}}};
    localparam logic signed [DATA_W-1:0] DATA_MAX = {1'b0, {(DATA_W-1){1'b1}}};
    localparam logic signed [DATA_W-1:0] DATA_MIN = {1'b1, {(DATA_W-1){1'b0}}};

    function automatic int bit_reverse(input int value);
        int b;
        int r;
        begin
            r = 0;
            for (b = 0; b < LOGN; b++) begin
                r = (r << 1) | ((value >> b) & 1);
            end
            return r;
        end
    endfunction

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

    function automatic logic signed [DATA_W-1:0] scale_ifft(
        input logic signed [CORE_W-1:0] x
    );
        logic signed [LONG_W-1:0] shifted;
        begin
            shifted    = $signed(x) >>> LOGN; // divide by N for IFFT normalization
            scale_ifft = sat_to_data(shifted);
        end
    endfunction

    integer ti;
    real angle;
    initial begin
        for (ti = 0; ti < N/2; ti++) begin
            angle   = (2.0 * PI * ti) / N;
            tw_re[ti] = quant_real_to_fixed($cos(angle));
            tw_im[ti] = quant_real_to_fixed($sin(angle));
        end
    end

    always_comb begin
        in_ready  = (state == ST_LOAD);
        out_valid = (state == ST_OUT);
        out_last  = (state == ST_OUT) && (out_idx == (N-1));

        if (state == ST_OUT) begin
            out_re = scale_ifft(mem_re[out_idx]);
            out_im = scale_ifft(mem_im[out_idx]);
        end else begin
            out_re = '0;
            out_im = '0;
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin
        int wr_idx;
        int idx_a;
        int idx_b;
        int tw_idx;

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
            frame_done <= 1'b0;

            stage  <= 0;
            m      <= 2;
            half   <= 1;
            k_base <= 0;
            j_idx  <= 0;
        end else begin
            frame_done <= 1'b0;

            case (state)
                ST_LOAD: begin
                    if (in_valid && in_ready) begin
                        // Store in bit-reversed order so iterative DIT returns natural-order output.
                        wr_idx = bit_reverse(load_cnt);
                        mem_re[wr_idx] <= $signed(in_re);
                        mem_im[wr_idx] <= $signed(in_im);

                        if (load_cnt == N-1) begin
                            state    <= ST_CALC;
                            load_cnt <= 0;

                            stage  <= 0;
                            m      <= 2;
                            half   <= 1;
                            k_base <= 0;
                            j_idx  <= 0;
                        end else begin
                            load_cnt <= load_cnt + 1;
                        end
                    end
                end

                ST_CALC: begin
                    idx_a  = k_base + j_idx;
                    idx_b  = idx_a + half;
                    tw_idx = j_idx << (LOGN - stage - 1);

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
                        if (k_base + m < N) begin
                            k_base <= k_base + m;
                        end else begin
                            if (stage == LOGN - 1) begin
                                state   <= ST_OUT;
                                out_idx <= 0;
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
