`default_nettype none

// 1D FFT core (N=16 default), radix-2 DIF, in-place iterative architecture.
// Input order: natural. Output order: bit-reversed.
module fft1d_core #(
    parameter int N    = 16,
    parameter int LOGN = 4
) (
    input  logic                    clk,
    input  logic                    rst_n,
    input  logic                    in_valid,
    output logic                    in_ready,
    input  logic signed [15:0]      in_re,
    input  logic signed [15:0]      in_im,
    output logic                    out_valid,
    input  logic                    out_ready,
    output logic signed [15:0]      out_re,
    output logic signed [15:0]      out_im
);

  typedef logic [LOGN-1:0] idx_t;
  localparam idx_t LAST_IDX = idx_t'(N - 1);

  typedef enum logic [1:0] {ST_LOAD, ST_RUN, ST_OUT} state_t;
  state_t state;

  logic signed [15:0] mem_re [0:N-1];
  logic signed [15:0] mem_im [0:N-1];
  logic [LOGN-1:0] load_ptr, out_ptr;

  logic ctrl_start, ctrl_done;
  logic ctrl_busy_unused, bfly_busy_unused;
  logic bfly_start, bfly_done;
  logic [LOGN-1:0] idx_a, idx_b, tw_idx;
  logic signed [15:0] w_re, w_im;
  logic signed [15:0] bfly_a_re, bfly_a_im, bfly_b_re, bfly_b_im;

  fft_controller #(.N(N), .LOGN(LOGN)) u_ctrl (
      .clk(clk),
      .rst_n(rst_n),
      .start(ctrl_start),
      .bfly_done(bfly_done),
      .bfly_start(bfly_start),
      .idx_a(idx_a),
      .idx_b(idx_b),
      .tw_idx(tw_idx),
      .busy(ctrl_busy_unused),
      .done(ctrl_done)
  );

  twiddle_rom #(.LOGN(LOGN)) u_rom (
      .idx(tw_idx),
      .w_re(w_re),
      .w_im(w_im)
  );

  butterfly u_bfly (
      .clk(clk),
      .rst_n(rst_n),
      .start(bfly_start),
      .a_re(mem_re[idx_a]),
      .a_im(mem_im[idx_a]),
      .b_re(mem_re[idx_b]),
      .b_im(mem_im[idx_b]),
      .w_re(w_re),
      .w_im(w_im),
      .busy(bfly_busy_unused),
      .done(bfly_done),
      .out_a_re(bfly_a_re),
      .out_a_im(bfly_a_im),
      .out_b_re(bfly_b_re),
      .out_b_im(bfly_b_im)
  );

  wire _unused = &{ctrl_busy_unused, bfly_busy_unused, 1'b0};

  assign in_ready  = (state == ST_LOAD);
  assign out_valid = (state == ST_OUT);

  always_comb begin
    out_re = '0;
    out_im = '0;
    if (state == ST_OUT) begin
      out_re = mem_re[out_ptr];
      out_im = mem_im[out_ptr];
    end
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      state      <= ST_LOAD;
      load_ptr   <= '0;
      out_ptr    <= '0;
      ctrl_start <= 1'b0;
    end else begin
      ctrl_start <= 1'b0;

      if (bfly_done) begin
        mem_re[idx_a] <= bfly_a_re;
        mem_im[idx_a] <= bfly_a_im;
        mem_re[idx_b] <= bfly_b_re;
        mem_im[idx_b] <= bfly_b_im;
      end

      unique case (state)
        ST_LOAD: begin
          if (in_valid && in_ready) begin
            mem_re[load_ptr] <= in_re;
            mem_im[load_ptr] <= in_im;
            if (load_ptr == LAST_IDX) begin
              load_ptr   <= '0;
              ctrl_start <= 1'b1;
              state      <= ST_RUN;
            end else begin
              load_ptr <= load_ptr + 1'b1;
            end
          end
        end
        ST_RUN: begin
          if (ctrl_done) begin
            out_ptr <= '0;
            state   <= ST_OUT;
          end
        end
        ST_OUT: begin
          if (out_valid && out_ready) begin
            if (out_ptr == LAST_IDX) begin
              out_ptr <= '0;
              state   <= ST_LOAD;
            end else begin
              out_ptr <= out_ptr + 1'b1;
            end
          end
        end
        default: state <= ST_LOAD;
      endcase
    end
  end

endmodule

`default_nettype wire
