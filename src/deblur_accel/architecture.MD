# Deblur Accelerator Architecture

## 1) What This Accelerator Does

The accelerator is a frequency-domain deblurring pipeline for one square grayscale tile:

`blurred image -> FFT -> Wiener filter generation -> complex multiply -> IFFT -> deblurred image`

Current hardware chain:
- `FFT2D` (butterfly datapath + memory + controller) converts pixels to frequency bins `Y(u,v)`.
- `FilterGen` computes Wiener gain `G(u,v)` from `H(u,v)` and `K`.
- `complex_mult` applies filtering: `Yf(u,v) = Y(u,v) * G(u,v)`.
- `IFFT` reconstructs spatial-domain output from `Yf(u,v)`.

Current integration mode:
- Software provides `H(u,v)` (computed from PSF) and streams it to hardware.

Deployment target:
- Replace software-provided `H(u,v)` with a precomputed PSF/`H(u,v)` bank (on-chip/off-chip).
- Select bank entry by control logic (or a lightweight blur estimator).
- Keep Wiener + multiply + FFT/IFFT pipeline fully hardware-driven during inference.

## 2) How To Run

1. Put input images in `src/deblur_accel/image/`.
2. Run the pipeline from `src/deblur_accel/`:

```bash
python PY_deblur_pipeline.py \
  --images-dir image \
  --outdir results \
  --max-n 256 \
  --img-n 256 \
  --fft-logn 8 \
  --lanes 4 \
  --k-values 16,64,256,1024,4096,8192,16384 \
  --h-scale 32767 \
  --psf-kernel 9 \
  --psf-sigma 2.0 \
  --deblur-psf-sigmas 1.2,1.6,2.0,2.4,2.8,3.2 \
  --data-w 24 \
  --frac-w 15 \
  --mult-frac 15 \
  --mult-saturate 1 \
  --pre-ifft-shifts 12 \
  --fft-to-ifft-maps 0 \
  --selection-metric sharpness \
  --improve-tolerance 0.10
```

3. Output is written to `src/deblur_accel/results/<image>_N*_L*/`.

## 2.1) Parameter Explanation

Command-line parameters in `PY_deblur_pipeline.py`, grouped by what they control:

### Paths And Files

- `--images-dir`
  - Input folder containing source images.
  - Default: `image`

- `--outdir`
  - Output folder for result case directories.
  - Default: `results`


### Image Size / FFT Topology

- `--max-n`
  - Maximum allowed tile size `N` used by auto-sizing path.
  - The script chooses a power-of-two size from image dimensions, then clamps by `max-n`.
  - Default: `256`

- `--img-n`
  - Directly sets hardware tile size `N` (`IMG_N`).
  - Must be power-of-two and `>= 2`.
  - If provided, this overrides auto-size logic from image dimensions.
  - Default: unset (auto)

- `--fft-logn`
  - Directly sets `FFT_LOGN` (number of radix-2 FFT stages in 1D FFT path).
  - Must satisfy `2^FFT_LOGN == IMG_N`.
  - Default: unset (auto from `IMG_N`)

- `--lanes`
  - Number of parallel FFT lanes (`FFT_LANES`).
  - Higher lanes can improve throughput at higher area cost.
  - Must be power-of-two and divide selected `N`.
  - Default: `4`

- `--fft-to-ifft-maps`
  - Comma-separated mapping modes (`FFT_TO_IFFT_MAP` values) from FFT bin order to IFFT feed order.
  - Usually keep as `0` unless explicitly validating alternate mappings.
  - Default: `0`

---

### Blur / Deblur Model (PSF + Wiener)

- `--psf-kernel`
  - Gaussian kernel size used to synthesize blur PSF.
  - Odd values are preferred.
  - Default: `9`

- `--psf-sigma`
  - Sigma used to generate the *blurred input* PSF.
  - This controls blur severity of synthetic input.
  - Default: `2.0`

- `--deblur-psf-sigmas`
  - Comma-separated sigma candidates used for *deblur filter model* PSF.
  - Lets deblur model differ from blur model (model-mismatch search).
  - Empty means use only `--psf-sigma`.
  - Default: `1.2,1.6,2.0,2.4,2.8,3.2`

- `--k-values`
  - Comma-separated Wiener `K` candidates (regularization strength).
  - Script sweeps these and picks best candidate by `--selection-metric`.
  - Larger `K` = more conservative deblurring (less noise amplification).
  - Default: `16,64,256,1024,4096,8192,16384`

- `--h-scale`
  - Quantization scale used to convert floating-point `H(u,v)` into fixed-point integers.
  - Default targets Q1.15-style scaling (`32767`).
  - Default: `32767`

---

### Fixed-Point / Numeric Hardware Format

- `--data-w`
  - Main fixed-point data width (`DATA_W` parameter) for Wiener/mult/IFFT path.
  - Wider values reduce clipping/quantization risk but increase hardware cost.
  - Default: `24`

- `--frac-w`
  - Fractional bit count (`FRAC_W` parameter) for fixed-point values in Wiener/IFFT settings.
  - Controls precision split between integer/fraction bits.
  - Default: `15`

- `--mult-frac`
  - Fractional bit count (`MULT_FRAC` parameter) used inside `complex_mult`.
  - Should usually match `--frac-w` for consistent scaling.
  - Default: `15`

- `--mult-saturate`
  - Saturation mode (`MULT_SAT` parameter) for complex multiplier output.
  - `1`: saturate on overflow, `0`: wrap.
  - Default: `1`

- `--pre-ifft-shifts`
  - Comma-separated left-shift candidates (`PRE_IFFT_SHIFT` values) applied before IFFT input.
  - Compensates normalization/scaling mismatch; swept and best selected.
  - Larger values increase amplitude but can clip.
  - Default: `12`

---

### Candidate Selection / Reporting

- `--selection-metric`
  - Candidate-selection rule for PSF/K sweep.
  - `sharpness`: no-reference selection (variance of Laplacian), best for realistic use without ground truth.
  - `rmse`: reference-based selection (only meaningful in synthetic tests where original is known).
  - Default: `sharpness`

- `--improve-tolerance`
  - Reporting threshold for `IMPROVED` status.
  - `IMPROVED` if `rmse_deblur <= rmse_blur * (1 + tolerance)`.
  - Default: `0.10` (10% tolerance)

## 3) Top-Level Hardware I/O (`deblur.sv`)

Top module: `src/deblur_accel/deblur.sv`

Signal-by-signal function:

- `clk`: Main synchronous clock for all internal blocks (`FFT2D`, `FilterGen`, `complex_mult`, `IFFT`).
- `rst_n`: Active-low reset; `0` resets FSM/counters/state, `1` enables normal operation.
- `img_valid`: Upstream says current `img_re` sample is valid.
- `img_ready`: Deblur top can accept `img_re`; input transfer happens on `img_valid && img_ready`.
- `img_re[15:0]`: Spatial-domain real input sample (signed fixed-point); expected `IMG_N*IMG_N` samples per frame.
- `h_valid`: Upstream says current `h_re/h_im` bin is valid.
- `h_ready`: Deblur top can accept one `H(u,v)` bin; transfer happens on `h_valid && h_ready`.
- `h_re[DATA_W-1:0]`: Real part of blur transfer bin `H(u,v)` (signed fixed-point).
- `h_im[DATA_W-1:0]`: Imaginary part of blur transfer bin `H(u,v)` (signed fixed-point).
- `k_cfg[DATA_W-1:0]`: Wiener regularization constant `K`, used by `FilterGen` in `G(u,v)=conj(H)/( |H|^2 + K )`.
- `out_valid`: Deblur top says `out_re/out_im` are valid.
- `out_ready`: Downstream can accept output; output transfer happens on `out_valid && out_ready`.
- `out_re[DATA_W-1:0]`: Reconstructed spatial-domain real output sample (primary image output).
- `out_im[DATA_W-1:0]`: Reconstructed spatial-domain imaginary residual (ideally near zero).
- `out_last`: Asserted on final output sample of the frame.
- `done`: Top-level frame completion indication.
- `fft_perf_done`: FFT performance counter ready flag.
- `fft_perf_cycles[63:0]`: FFT cycle count for the frame.

### Expected Stream Order (Current Integration)

1. Feed complete `H` stream: `IMG_N * IMG_N` complex bins (`h_re/h_im`).
2. Feed complete image stream: `IMG_N * IMG_N` real samples (`img_re`).
3. Read complete output stream: `IMG_N * IMG_N` complex samples (`out_re/out_im`).

## 4) Expected I/O Behavior

Detailed transaction behavior for one frame (`N = IMG_N`, total samples `TOT = N*N`):

### Phase 0: Reset / Idle
- Hold `rst_n = 0` for reset cycles.
- Drive `rst_n = 1` to start normal operation.
- Internal state starts at `ST_LOAD_H`.

### Phase 1: Load Wiener Model (`H`)  
Goal: accept exactly `TOT` complex bins.

- Producer drives:
  - `h_valid = 1`
  - `h_re`, `h_im` = current `H(u,v)` bin
- Transfer condition: `h_valid && h_ready`.
- On each successful handshake, internal `H` write counter increments by 1.
- Continue until exactly `TOT` handshakes complete.
- During this phase, image stream is not consumed as the top is in `ST_LOAD_H`.

### Phase 2: Load Blurred Image Samples  
Goal: accept exactly `TOT` real samples.

- Producer drives:
  - `img_valid = 1`
  - `img_re` = current spatial sample
- Transfer condition: `img_valid && img_ready`.
- On each successful handshake, internal image input counter increments by 1.
- Continue until exactly `TOT` handshakes complete.
- Samples are streamed in row-major frame order from the software testbench.

### Phase 3: Internal Processing  
Goal: hardware computes frame result.

- Top runs internal chain:
  - FFT on loaded image
  - Wiener `G(u,v)` generation from loaded `H` and `k_cfg`
  - Complex multiply
  - IFFT reconstruction
- No extra external data input required in this phase.

### Phase 4: Drain Output Samples  
Goal: output exactly `TOT` complex reconstructed samples.

- Consumer drives `out_ready` (usually held high by testbench).
- Top drives:
  - `out_valid`
  - `out_re`, `out_im`
  - `out_last` on final sample
- Transfer condition: `out_valid && out_ready`.
- On each successful handshake, output counter increments by 1.
- After `TOT` successful handshakes:
  - frame output is complete
  - `done` asserts for completion indication.

### Handshake Rules (All Streams)
- A sample/bin is transferred only on the rising clock edge where `valid && ready == 1`.
- If `valid=1` and `ready=0`, producer must hold the same data stable until accepted.
- If `ready=1` and `valid=0`, no transfer occurs.

### Data Format Expectations
- `img_re`: signed fixed-point real sample (input image domain).
- `h_re/h_im`: signed fixed-point complex frequency response bins.
- `k_cfg`: fixed-point Wiener regularization constant.
- `out_re/out_im`: signed fixed-point complex reconstructed output.

### Output Use
- `out_re` is the primary reconstructed image value.
- `out_im` is residual imaginary component (should be small in a stable run).
- Python post-processing converts `out_re` back to 8-bit grayscale for `deblurred.png`.

### Performance / Quality Notes
- `fft_perf_done` / `fft_perf_cycles` report FFT block cycle count.
- Deblur quality depends on PSF-model match, `K`, mapping/shift settings, and fixed-point precision.
- `metrics.txt` records selected settings and quality metrics per frame.

## 5) Module Hierarchy

Top integration:
- `deblur.sv`
  - `fft2d_core` (`src/fft/SV_fft2d_core.sv`)
    - `fft1d_core` (`src/fft/SV_fft1d_core.sv`)
      - `fft_controller` (`src/fft/SV_fft_controller.sv`)
      - `butterfly` (`src/fft/SV_butterfly.sv`)
      - `twiddle_rom` (auto-generated at build time)
  - `FilterGen` (`src/wiener_filter/wiener_filter.v`)
  - `complex_mult` (`src/mult/complex_mult.sv`)
  - `ifft_core` (`src/ifft/ifft_core.sv`)

Software orchestration:
- `PY_deblur_pipeline.py`
  - generates blur and `H(u,v)`
  - compiles/runs Verilator model
  - reconstructs output image
  - writes `original.png`, `blurred.png`, `deblurred.png`, `metrics.txt`

Single C++ testbench for top:
- `CPP_deblur_stage_tb.cpp` (instantiates `Vdeblur` only)
