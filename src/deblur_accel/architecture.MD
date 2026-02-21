# Deblur Accelerator Architecture

## 1) What This Accelerator Does

The accelerator performs frequency-domain deblurring on one square grayscale tile:

`blurred image -> FFT -> Wiener filter generation -> complex multiply -> IFFT -> deblurred image`

In this project flow, Python prepares the input tile and PSF-derived frequency response `H(u,v)`, and hardware (`deblur.sv`) executes the core streaming chain.

## 2) How To Run

1. Put input images in `src/deblur_accel/image/`.
2. Run the pipeline from `src/deblur_accel/`:

```bash
python PY_deblur_pipeline.py \
  --images-dir image \
  --outdir results \
  --max-n 256 \
  --lanes 4 \
  --k-values 1,2,4,8,16,32,64,128,256,512,1024 \
  --h-scale 32767 \
  --psf-kernel 9 \
  --psf-sigma 2.0 \
  --deblur-psf-sigmas 2.0 \
  --data-w 24 \
  --frac-w 15 \
  --mult-frac 15 \
  --mult-saturate 1 \
  --pre-ifft-shifts 12 \
  --fft-to-ifft-maps 0
```

3. Output is written to `src/deblur_accel/results/<image>_N*_L*/`.

## 2.1) Parameter Explanation

Command-line parameters in `PY_deblur_pipeline.py`:

- `--images-dir`
  - Input folder containing source images.
  - Default: `image`

- `--outdir`
  - Output folder for result case directories.
  - Default: `results`

- `--max-n`
  - Maximum width of the image by pixel, power-of-two.
  - The script resizes input to nearest allowed power-of-two up to this limit.
  - Default: `256`

- `--img-n`
  - The width of the image in pixel
  - Must be power-of-two and `>= 2`.
  - If provided, this overrides auto-size logic from image dimensions.
  - Default: unset (auto)

- `--fft-logn`
  - The number of layer needed for doing FFT
  - Must satisfy `2^FFT_LOGN == IMG_N`.
  - Default: unset (auto from `IMG_N`)

- `--lanes`
  - The number of fft compute module will exist for the accelerator
  - Higher lanes can improve throughput at higher area cost.
  - Must be power-of-two and divide selected `N`.
  - Default: `4`

- `--k-values`
  - Comma-separated Wiener `K` candidates (regularization strength).
  - Script sweeps these and picks best RMSE.
  - Larger `K` = more conservative deblurring (less noise amplification).
  - Default: `1,2,4,8,16,32,64,128,256,512,1024`

- `--h-scale`
  - Quantization scale used to convert floating-point `H(u,v)` into fixed-point integers.
  - Default targets Q1.15-style scaling (`32767`).
  - Default: `32767`

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

- `--fft-to-ifft-maps`
  - Comma-separated mapping modes (`FFT_TO_IFFT_MAP` values) from FFT bin order to IFFT feed order.
  - Usually keep as `0` unless explicitly validating alternate mappings.
  - Default: `0`

- `--improve-tolerance`
  - Reporting threshold for `IMPROVED` status.
  - `IMPROVED` if `rmse_deblur <= rmse_blur * (1 + tolerance)`.
  - Default: `0.10` (10% tolerance)

- `--selection-metric`
  - Candidate-selection rule for PSF/K sweep.
  - `sharpness`: no-reference selection (variance of Laplacian), best for realistic use without ground truth.
  - `rmse`: reference-based selection (only meaningful in synthetic tests where original is known).
  - Default: `sharpness`

## 3) Top-Level Hardware I/O (`deblur.sv`)

Top module: `src/deblur_accel/deblur.sv`

### Clock/Reset
- `clk`
- `rst_n`

### Image Stream Input (spatial domain)
- `img_valid` / `img_ready`
- `img_re[15:0]` (real grayscale sample in fixed-point)

### Wiener Input Stream (`H(u,v)`)
- `h_valid` / `h_ready`
- `h_re[DATA_W-1:0]`, `h_im[DATA_W-1:0]` (complex frequency response bins)
- `k_cfg[DATA_W-1:0]` (Wiener regularization constant)

### Output Stream (reconstructed spatial domain)
- `out_valid` / `out_ready`
- `out_re[DATA_W-1:0]`
- `out_im[DATA_W-1:0]`
- `out_last`

### Status
- `done`
- `fft_perf_done`
- `fft_perf_cycles[63:0]`

## 4) Expected I/O Behavior

### Input expectations
- Image tile is square, power-of-two sized (pipeline currently uses up to `N=256`).
- `img_*` stream provides exactly `N*N` samples.
- `h_*` stream provides exactly `N*N` complex bins.
- Data is fixed-point signed integer format (Q-format controlled by script params).

### Handshake behavior
- All streams use valid/ready protocol.
- A transfer occurs when `valid && ready` on the same cycle.
- The testbench feeds `H` first, then image samples, then collects `N*N` output samples.

### Output expectations
- `out_*` returns `N*N` complex samples corresponding to reconstructed image domain.
- Main useful content is in `out_re`; `out_im` should stay small (residual numerical error).
- Python converts reconstructed fixed-point values back to 8-bit grayscale PNG.

### Quality behavior
- Deblur quality depends on PSF model match, `K`, fixed-point widths, and scaling settings.
- `metrics.txt` records RMSE and selected settings (`best_k`, `best_shift`, map, etc.).

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
