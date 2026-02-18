# FFT Accelerator Architecture

## 1. Goal and Scope
This folder implements a hardware-efficient 2D FFT accelerator in SystemVerilog using a **single reused 1D FFT datapath** style (TinyTapeout-friendly direction), plus a Verilator C++ driver and Python automation script.

Main verification flow:
- Image (grayscale) -> Q1.15 -> hardware 2D FFT
- Hardware FFT output -> NumPy IFFT -> reconstructed image
- Compare reconstructed vs original with max error and RMSE

## 2. File Organization
- `SV_fft2d_core.sv`: top-level 2D FFT core (row FFT pass + column FFT pass)
- `SV_fft1d_core.sv`: reusable 1D FFT engine (radix-2 DIF, iterative)
- `SV_fft_controller.sv`: butterfly schedule generator for 1D FFT
- `SV_butterfly.sv`: one radix-2 DIF butterfly with time-shared multiplier
- `SV_twiddle_rom.sv`: static twiddle ROM (N=32 default reference)
- `CPP_2d_fft_tb.cpp`: Verilated executable testbench for streaming in/out text files
- `PY_test_image_generation.py`: automation (build + run + compare + PNG outputs)
- `cmd.txt`: command shortcuts

Note: for N != 32, Python auto-generates `build/SV_twiddle_rom_auto.sv` and compiles that ROM.

## 3. Hardware Hierarchy
`fft2d_core`
- Contains two local memories (`mem0`, `mem1`) sized N*N complex samples
- Instantiates **one** `fft1d_core`
- Schedules:
  1. load full tile into `mem0`
  2. row-wise 1D FFT: `mem0 -> fft1d -> mem1`
  3. column-wise 1D FFT: `mem1 -> fft1d -> mem0`
  4. drain final bins from `mem0`

`fft1d_core`
- Contains in-place RAM for N complex samples
- Instantiates:
  - `fft_controller` (index/twiddle scheduling)
  - `twiddle_rom`
  - `butterfly`
- Modes:
  - load N samples
  - run all butterflies iteratively
  - stream out N bins

`butterfly`
- Computes one radix-2 DIF butterfly
- Uses one multiplier reused across 4 micro-steps
- Applies per-stage `>>> 1` scaling

## 4. Top I/O of `fft2d_core`
Inputs:
- `clk`, `rst_n`
- `in_valid`, `in_re[15:0]`, `in_im[15:0]`
- `out_ready`

Outputs:
- `in_ready`
- `out_valid`, `out_re[15:0]`, `out_im[15:0]`
- `perf_done` (1-cycle pulse at block completion)
- `perf_cycles[63:0]` (latched block cycle count)

Data format:
- signed Q1.15 complex samples on input/output ports

## 5. Expected I/O Behavior Over Time
For one NxN block:

1. **Load phase**
- `in_ready=1`, `out_valid=0`
- Producer sends exactly N*N samples row-major
- One sample accepted each cycle when `in_valid && in_ready`

2. **Compute phase**
- `in_ready=0`, `out_valid=0`
- Core internally runs row pass then column pass
- No external data transfer

3. **Drain phase**
- `in_ready=0`, `out_valid=1`
- Consumer accepts exactly N*N complex bins with `out_ready`

Then returns to load phase for next tile.

Ordering:
- Input: natural row-major
- Output: bit-reversed on both dimensions (DIF behavior)
- Software can reorder using bit-reversal on rows and columns

## 6. Performance Counter Meaning
`perf_cycles` measures elapsed cycles from:
- first accepted sample of a block
through
- final accepted output sample of the same block

This gives full block latency including load, compute, and drain.

## 7. Python/C++ Pipeline
Python script (`PY_test_image_generation.py`) does:
1. Read all images from `image/`
2. Choose nearest power-of-2 square size, capped by `--max-n` (default 256)
3. Resize image to NxN grayscale
4. Convert to Q1.15 and write `input_tile.txt` (one integer per line)
5. Build Verilator model for N (if needed)
6. Run C++ binary (`build/Vfft2d_core`) to generate `dut_fft_out.txt`
7. Load hardware FFT output, reorder if `--order bitrev2d`
8. Reconstruct via `ifft2(F_hw * (N*N))`
9. Save:
   - `original.png`
   - `reconstructed.png`
   - `difference.png`
   - `metrics.txt` (max error, rmse, pass, perf_cycles)

## 8. How To Run
From `src/fft`:

1. Put one or more grayscale images in `image/`
2. Run:
```bash
python3 PY_test_image_generation.py --images-dir image --outdir results --order bitrev2d --max-n 256
```

Outputs are under `results/<image_name>_N<resolved_size>/`.

## 9. Constraints and Tradeoff Summary
- This design is area-efficient by reusing one 1D FFT core and one butterfly datapath.
- Throughput is lower than wide-parallel architectures.
- This tradeoff is intentional for TinyTapeout-style feasibility.
