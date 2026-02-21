/* verilator lint_off DECLFILENAME */
/* verilator lint_off WIDTHEXPAND */
/* verilator lint_off UNUSEDPARAM */
`timescale 1ns/1ps

// Wiener-only filter generator:
//   G(u,v) = H*(u,v) / (|H(u,v)|^2 + K)
//
// Fixed-point model:
// - H_real/H_imag are signed Q(DATA_WIDTH-FRAC_W).FRAC_W
// - K is unsigned Q(DATA_WIDTH-FRAC_W).FRAC_W
// - G_real/G_imag are signed Q(DATA_WIDTH-FRAC_W).FRAC_W
//
// Internal math keeps |H|^2 in Q(2*FRAC_W), then computes exact fixed-point divide:
//   G_int = (H_int << (2*FRAC_W)) / (|H|^2_int + (K_int << FRAC_W))
module FilterGen #(
    parameter int DATA_WIDTH     = 24,
    parameter int FRAC_W         = 15,
    parameter int LUT_ADDR_WIDTH = 8,  // reserved for interface compatibility
    parameter int K_SHIFT        = 0
) (
    input  logic                           clk,
    input  logic                           rst_n,
    input  logic                           valid_in,
    output logic                           ready_in,
    input  logic signed [DATA_WIDTH-1:0]   H_real,
    input  logic signed [DATA_WIDTH-1:0]   H_imag,
    input  logic        [DATA_WIDTH-1:0]   K,
    output logic                           valid_out,
    input  logic                           ready_out,
    output logic signed [DATA_WIDTH-1:0]   G_real,
    output logic signed [DATA_WIDTH-1:0]   G_imag
);

    localparam int MUL_W   = 2 * DATA_WIDTH;
    localparam int DEN_W   = MUL_W + FRAC_W + 2;
    localparam int NUM_W   = DATA_WIDTH + (2 * FRAC_W) + 2;
    localparam int QUOT_W  = (NUM_W > DEN_W) ? (NUM_W + 1) : (DEN_W + 1);

    assign ready_in = ~valid_out | ready_out;

    logic signed [MUL_W-1:0] h_real_sq;
    logic signed [MUL_W-1:0] h_imag_sq;
    logic signed [DEN_W-1:0] mag2_q2f;
    logic signed [DEN_W-1:0] denom_q2f;
    logic signed [DEN_W-1:0] denom_safe;

    logic signed [NUM_W-1:0] num_real;
    logic signed [NUM_W-1:0] num_imag;

    logic signed [QUOT_W-1:0] q_real;
    logic signed [QUOT_W-1:0] q_imag;

    function automatic logic signed [DATA_WIDTH-1:0] sat_to_data(
        input logic signed [QUOT_W-1:0] x
    );
        logic signed [QUOT_W-1:0] max_ext;
        logic signed [QUOT_W-1:0] min_ext;
        begin
            max_ext = $signed({{(QUOT_W-DATA_WIDTH){1'b0}}, {1'b0, {(DATA_WIDTH-1){1'b1}}}});
            min_ext = $signed({{(QUOT_W-DATA_WIDTH){1'b1}}, {1'b1, {(DATA_WIDTH-1){1'b0}}}});
            if (x > max_ext) begin
                sat_to_data = {1'b0, {(DATA_WIDTH-1){1'b1}}};
            end else if (x < min_ext) begin
                sat_to_data = {1'b1, {(DATA_WIDTH-1){1'b0}}};
            end else begin
                sat_to_data = x[DATA_WIDTH-1:0];
            end
        end
    endfunction

    always_comb begin
        h_real_sq = $signed(H_real) * $signed(H_real);
        h_imag_sq = $signed(H_imag) * $signed(H_imag);

        // |H|^2 in Q(2*FRAC_W)
        mag2_q2f = $signed({{(DEN_W-MUL_W){h_real_sq[MUL_W-1]}}, h_real_sq})
                 + $signed({{(DEN_W-MUL_W){h_imag_sq[MUL_W-1]}}, h_imag_sq});

        // K in Q(FRAC_W) -> align to Q(2*FRAC_W)
        denom_q2f = mag2_q2f + ($signed({{(DEN_W-DATA_WIDTH){1'b0}}, K}) <<< (FRAC_W + K_SHIFT));

        if (denom_q2f <= 0) begin
            denom_safe = 1;
        end else begin
            denom_safe = denom_q2f;
        end

        // Numerators aligned for output Q(FRAC_W): shift by 2*FRAC_W before divide.
        num_real = $signed(H_real) <<< (2 * FRAC_W);
        num_imag = $signed(-H_imag) <<< (2 * FRAC_W);

        q_real = $signed(num_real) / $signed(denom_safe);
        q_imag = $signed(num_imag) / $signed(denom_safe);
    end

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            valid_out <= 1'b0;
            G_real    <= '0;
            G_imag    <= '0;
        end else begin
            if (ready_in) begin
                valid_out <= valid_in;
                if (valid_in) begin
                    G_real <= sat_to_data(q_real);
                    G_imag <= sat_to_data(q_imag);
                end
            end
        end
    end

endmodule

/* verilator lint_on UNUSEDPARAM */
/* verilator lint_on WIDTHEXPAND */
/* verilator lint_on DECLFILENAME */
