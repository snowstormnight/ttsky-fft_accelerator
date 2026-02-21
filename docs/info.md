## How it works

`tt_um_ttsky_deblur_accel` is a Tiny Tapeout wrapper around a grayscale image deblurring accelerator.

Internal processing chain:

1. `fft2d_core` computes the 2D FFT of the input image tile.
2. `FilterGen` computes Wiener coefficients from streamed `H(u,v)` and `K`.
3. `complex_mult` applies frequency-domain filtering: `Yf(u,v) = Y(u,v) * G(u,v)`.
4. `ifft_core` computes the inverse transform to reconstruct the deblurred spatial-domain tile.

Top-level internal module hierarchy:

- `tt_um_ttsky_deblur_accel`
  - `deblur`
    - `fft2d_core`
      - `fft1d_core`
        - `fft_controller`
        - `butterfly`
        - `twiddle_rom`
    - `FilterGen`
    - `complex_mult`
    - `ifft_core`

Current Tiny Tapeout wrapper configuration (inside wrapper):

- `IMG_N = 32`
- `FFT_LOGN = 5`
- `FFT_LANES = 1`
- `DATA_W = 16`
- `FRAC_W = 12`
- `MULT_FRAC = 12`
- `MULT_SAT = 1`
- `PRE_IFFT_SHIFT = 0`
- `FFT_TO_IFFT_MAP = 0`

Interface behavior:

- Image and filter model are streamed in using valid/ready handshakes.
- Output is streamed out using valid/ready handshake.
- Wrapper exports status flags (`img_ready`, `h_ready`, `out_valid`, `out_last`, `done`, `fft_perf_done`) and LSB debug bits of output data.

## How to test

The wrapper uses a minimal bring-up pin map for Tiny Tapeout IO limits.

### Pin map

Inputs (`ui_in`):

- `ui_in[0]`: `img_valid`
- `ui_in[1]`: `h_valid`
- `ui_in[2]`: `out_ready`
- `ui_in[7:0]`: also used as signed 8-bit payload for `h_re` (sign-extended to 16-bit) and contributes to `k_cfg`

Bidir inputs (`uio_in`, configured as input in this wrapper):

- `uio_in[7:0]`: signed 8-bit payload (sign-extended) used for `img_re` and `h_im`, and contributes to `k_cfg`

Outputs (`uo_out`):

- `uo_out[0]`: `img_ready`
- `uo_out[1]`: `h_ready`
- `uo_out[2]`: `out_valid`
- `uo_out[3]`: `out_last`
- `uo_out[4]`: `done`
- `uo_out[5]`: `fft_perf_done`
- `uo_out[6]`: `out_re[0]` (LSB debug)
- `uo_out[7]`: `out_im[0]` (LSB debug)

Bidirectional output behavior:

- `uio_oe = 8'b00000000` (all bidir pins used as input)
- `uio_out = 8'b00000000`

### Basic bring-up sequence

1. Hold `rst_n = 0` for reset cycles, then set `rst_n = 1`.
2. Stream `H` bins with `h_valid=1` while observing `h_ready`.
3. Stream image samples with `img_valid=1` while observing `img_ready`.
4. Set `out_ready=1` and read outputs when `out_valid=1`.
5. Detect frame completion with `out_last` / `done`.

Notes:

- This wrapper is intentionally IO-limited and uses compact 8-bit payload mapping for bring-up/debug.
- Full-resolution frame loading and high-quality result reconstruction are intended in the full Verilator/software flow, while this TT wrapper demonstrates integration and control signaling on TT pins.

## External hardware

No external hardware is required for digital simulation and Tiny Tapeout integration checks.

Optional for lab demonstration:

- A USB logic analyzer or FPGA host can drive pin streams (`ui_in`, `uio_in`) and capture `uo_out` status.
