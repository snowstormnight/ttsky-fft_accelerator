`timescale 1ns/1ps

// Wiener-only filter generator:
// W(u,v) = H*(u,v) / (|H(u,v)|^2 + K)
module FilterGen #(
    parameter int DATA_WIDTH     = 16,
    parameter int LUT_ADDR_WIDTH = 8,
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

    localparam int WIDE_WIDTH = 2 * DATA_WIDTH;
    localparam int LUT_DEPTH  = (1 << LUT_ADDR_WIDTH);

    // Output register is one entry deep; this keeps handshake robust with small area.
    assign ready_in = ~valid_out | ready_out;

    logic signed [WIDE_WIDTH-1:0] h_real_sq;
    logic signed [WIDE_WIDTH-1:0] h_imag_sq;
    logic signed [WIDE_WIDTH-1:0] mag2;
    logic signed [WIDE_WIDTH-1:0] denom;
    logic        [LUT_ADDR_WIDTH-1:0] lut_addr;
    logic signed [DATA_WIDTH-1:0] recip;
    logic signed [WIDE_WIDTH-1:0] g_real_wide;
    logic signed [WIDE_WIDTH-1:0] g_imag_wide;

    // Small reciprocal LUT avoids a divider and keeps area down.
    logic [DATA_WIDTH-1:0] recip_lut [0:LUT_DEPTH-1];
    initial begin : init_recip_lut
        int i;
        int recip_q;
        int num_q;
        int den_i;
        int max_pos;
        num_q   = (1 << (DATA_WIDTH - 1));      // Q1.15 scale for DATA_WIDTH=16
        max_pos = (1 << (DATA_WIDTH - 1)) - 1;  // max signed value
        for (i = 0; i < LUT_DEPTH; i++) begin
            den_i = i + 1;
            // Rounded fixed-point reciprocal: round((1/den_i) * 2^(DATA_WIDTH-1)).
            recip_q = (num_q + (den_i >> 1)) / den_i;
            // Saturate to signed positive range (avoid 32768 -> negative wrap in 16-bit signed).
            if (recip_q > max_pos) recip_q = max_pos;
            recip_lut[i] = recip_q[DATA_WIDTH-1:0];
        end
    end

    always_comb begin
        h_real_sq = H_real * H_real;
        h_imag_sq = H_imag * H_imag;
        mag2      = h_real_sq + h_imag_sq;
        denom     = mag2 + ($signed({{DATA_WIDTH{1'b0}}, K}) <<< K_SHIFT);
        
        // Use denominator magnitude directly as LUT index, clamped to table range.
        // This keeps small denominators accurate (e.g., denom=5 -> lut[5] ~= 0.2 in Q1.15).
        if (denom <= 0) begin
            lut_addr = '0;
        end else if (denom >= LUT_DEPTH) begin
            lut_addr = LUT_DEPTH-1;
        end else begin
            lut_addr = denom[LUT_ADDR_WIDTH-1:0];
        end
        recip     = $signed(recip_lut[lut_addr]);

        g_real_wide = H_real * recip;
        g_imag_wide = (-H_imag) * recip;
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
                    // Keep scaling cheap: truncate to output width.
                    G_real <= g_real_wide[DATA_WIDTH-1:0];
                    G_imag <= g_imag_wide[DATA_WIDTH-1:0];
                end
            end
        end
    end

endmodule
