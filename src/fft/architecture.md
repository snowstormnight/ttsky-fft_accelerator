# FFT Accelerator Architecture

## 1. Goal and Scope
This folder implements a parameterized 2D FFT accelerator in SystemVerilog using **LANES parallel 1D FFT datapaths** (LANES=1 gives the original area-efficient mode), plus a Verilator C++ driver and Python automation script.

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
- Instantiates `LANES` copies of `fft1d_core`
- Schedules:
  1. load full tile into `mem0`
  2. row-wise 1D FFT in batches of `LANES` rows: `mem0 -> fft1d[] -> mem1`
  3. column-wise 1D FFT in batches of `LANES` cols: `mem1 -> fft1d[] -> mem0`
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

## 4. External I/O Protocol 
Top-level ports:

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

One full block (`N x N`) works exactly like this:

1. **Feed input block**
- Wait until `in_ready=1`.
- Drive samples in row-major order (natural order):
  `(r=0,c=0) ... (r=0,c=N-1) ... (r=N-1,c=N-1)`.
- For grayscale images: `in_re=pixel_q15`, `in_im=0`.
- Keep `in_valid=1` while streaming.
- Advance to next input sample only on handshake: `in_valid && in_ready`.
- Exactly `N*N` input handshakes are required.

2. **Wait for internal compute**
- After the input block is fully accepted, set `in_valid=0`.
- During compute: `in_ready=0`, `out_valid=0`.
- Core runs row FFT batches then column FFT batches internally.
- No external transfer in this phase.

3. **Collect output block**
- Set `out_ready=1` (or backpressure if needed).
- When internal compute is complete, the core sets `out_valid=1` to indicate output data is ready to be drained.
- When `out_valid && out_ready`, capture one complex output bin.
- Exactly `N*N` output handshakes are produced.
- Output stream order is row-major over a **bit-reversed-2D** spectrum.
- Software can reorder to natural order with row and column bit-reversal.

4. **Block done / next block**
- On final output handshake, `perf_done` pulses for one cycle.
- `perf_cycles` holds full block latency (first accepted input -> last accepted output).
- Core then returns to load mode (`in_ready=1`) for the next block.

## 5. Performance Counter Meaning
`perf_cycles` measures elapsed cycles from:
- first accepted sample of a block
through
- final accepted output sample of the same block

This gives full block latency including load, compute, and drain.

## 6. Python/C++ Pipeline
Python script (`PY_test_image_generation.py`) does:
1. Read all images from `image/`
2. Choose nearest power-of-2 square size, capped by `--max-n` (default 256)
3. Resize image to NxN grayscale
4. Convert to Q1.15 and write `input_tile.txt` (one integer per line)
5. Build Verilator model for `(N, LANES)` (if needed)
6. Run C++ binary (`build/Vfft2d_core`) to generate `dut_fft_out.txt`
7. Load hardware FFT output, reorder if `--order bitrev2d`
8. Reconstruct via `ifft2(F_hw * (N*N))`
9. Save:
   - `original.png`
   - `reconstructed.png`
   - `difference.png`
   - `metrics.txt` (max error, rmse, pass, perf_cycles)

## 7. How To Run
From `src/fft`:

1. Put one or more grayscale images in `image/`
2. Run (example):
```bash
python3 PY_test_image_generation.py --images-dir image --outdir results --order bitrev2d --max-n 256 --lanes 4
```

Outputs are under `results/<image_name>_N<resolved_size>/`.

## 8. Constraints and Tradeoff Summary
- `LANES=1` is the smallest-area baseline.
- Higher `LANES` increases area and lowers latency (PPA tradeoff).
- External I/O is still 1 complex sample per cycle; speedup comes from lower internal compute cycles.

## 9. Why This FFT Accelerator Can Beat a CPU
This design is stronger than a CPU implementation in several hardware-specific ways:

1. **Deterministic throughput/latency**
- The FFT runs as a fixed hardware schedule (FSM + streaming handshakes), not a software loop affected by OS scheduling, cache misses, or branch behavior.
- For a fixed `(N, LANES)` and fixed handshakes, cycle count is repeatable (`perf_cycles`).

2. **Specialized data path for FFT math**
- The architecture directly maps radix-2 FFT operations into dedicated fixed-point hardware (butterfly + twiddle ROM + local memories).
- No instruction fetch/decode overhead for each arithmetic step.

3. **Parameterized parallelism**
- `LANES` scales parallel 1D FFT engines in hardware.
- `LANES=1` minimizes area; higher `LANES` lowers compute cycles significantly (measured in this project).
- This gives an explicit area/performance tradeoff a general CPU core cannot match as cleanly.

4. **Efficient streaming integration**
- The block uses a clean producer/consumer interface (`in_valid/in_ready`, `out_valid/out_ready`), which fits larger accelerator pipelines well.
- This is useful for FFT -> multiply -> IFFT chains without CPU-centric data marshaling at every operation.

5. **Fixed-point efficiency**
- Q1.15 arithmetic is compact and hardware-friendly, reducing logic cost and switching activity compared with floating-point software paths.

Important tradeoff:
- CPUs remain better for maximum flexibility and very small one-off problem sizes.
- This accelerator is better when repeated FFT workloads justify specialized hardware and deterministic behavior.
