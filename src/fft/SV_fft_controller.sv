`default_nettype none

// Iterative radix-2 DIF schedule for one in-place butterfly datapath.
// Generates (idx_a, idx_b, tw_idx) for each butterfly, then waits for bfly_done
// before advancing to the next pair.
//
// Terminology:
// - stage: FFT stage index (0 ... LOGN-1)
// - base : start index of the current butterfly group
// - j    : butterfly index inside the group
module fft_controller #(
    parameter int N    = 32,
    parameter int LOGN = 5
) (
    input  logic                  clk,
    input  logic                  rst_n,
    input  logic                  start,
    input  logic                  bfly_done,
    output logic                  bfly_start,
    output logic [LOGN-1:0]       idx_a,
    output logic [LOGN-1:0]       idx_b,
    output logic [LOGN-1:0]       tw_idx,
    output logic                  busy,
    output logic                  done
);

  localparam int STAGE_W = (LOGN <= 2) ? 1 : $clog2(LOGN);
  typedef logic [LOGN:0] count_t;
  typedef logic [STAGE_W:0] stage_ext_t;
  localparam count_t N_VAL = count_t'(N);
  localparam stage_ext_t LOGN_VAL = stage_ext_t'(LOGN);

  // running: controller active for current FFT block
  // waiting: a butterfly has been issued; wait for bfly_done
  logic running, waiting;
  logic [STAGE_W-1:0] stage;
  logic [LOGN-1:0] j, base;
  logic [LOGN:0] span, half;

  always_comb begin
    // DIF schedule parameters for current stage:
    // span = distance between groups, half = butterfly partner offset.
    span   = N_VAL >> stage;
    half   = span >> 1;
    // In-group butterfly: (base+j) with (base+j+half).
    idx_a  = base + j;
    idx_b  = base + j + half[LOGN-1:0];
    // Twiddle index pattern for DIF.
    tw_idx = j << stage;
  end

  assign busy = running;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      running     <= 1'b0;
      waiting     <= 1'b0;
      stage       <= '0;
      j           <= '0;
      base        <= '0;
      bfly_start  <= 1'b0;
      done        <= 1'b0;
    end else begin
      bfly_start <= 1'b0;
      done       <= 1'b0;

      if (start && !running) begin
        running <= 1'b1;
        waiting <= 1'b0;
        stage   <= '0;
        j       <= '0;
        base    <= '0;
      end else if (running) begin
        if (!waiting) begin
          // Fire one butterfly job.
          bfly_start <= 1'b1;
          waiting    <= 1'b1;
        end else if (bfly_done) begin
          // Butterfly done: walk j, then base, then stage.
          waiting <= 1'b0;
          if (({1'b0, j} + 1'b1) < half) begin
            j <= j + 1'b1;
          end else begin
            j <= '0;
            if (({1'b0, base} + span) < N_VAL) begin
              base <= base + span[LOGN-1:0];
            end else begin
              base <= '0;
              if (({1'b0, stage} + 1'b1) < LOGN_VAL) begin
                stage <= stage + 1'b1;
              end else begin
                running <= 1'b0;
                done    <= 1'b1;
              end
            end
          end
        end
      end
    end
  end

endmodule

`default_nettype wire
