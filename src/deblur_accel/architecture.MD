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
  --selection-metric rmse \
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
  - Number of 1d fft compute modules created (`FFT_LANES`).
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

### 5.1) `deblur.sv` (Top Integrator)

Path: `src/deblur_accel/deblur.sv`

Code sections:
- Lines 1-54:
  - Module header, top-level architecture intent, parameters, and all external I/O.
  - Defines system-level configuration knobs (`IMG_N`, `FFT_LANES`, width/scaling parameters, mapping mode).
- Lines 56-95:
  - Core localparams, top FSM enum (`ST_PRELOAD`, `ST_WAIT_FFT`, `ST_FEED_IFFT`, `ST_WAIT_OUT`, `ST_DONE`), frame memories, counters, and completion flags.
  - Declares frame-level storage for FFT bins (`y_*`) and Wiener outputs (`g_*`).
- Lines 96-134:
  - Interconnect signal declarations between submodules (FFT, FilterGen, multiplier, IFFT).
  - Separates ingress, processing, and egress handshake/datapath signals.
- Lines 136-147:
  - `bitrev_dim` helper function.
  - Reverses row/column bits for index remapping.
- Lines 149-184:
  - `fft_raw_to_nat` mapping function.
  - Converts FFT stream order to stored index order according to `FFT_TO_IFFT_MAP`.
- Lines 186-209:
  - `sat_shift_left_pow2` function.
  - Pre-IFFT gain shift with explicit overflow saturation.
- Lines 211-247:
  - Combinational handshake and datapath wiring.
  - Binds top I/O to submodule streams; constructs multiplier inputs from frame memories; applies optional pre-IFFT scaling.
- Lines 249-328:
  - Submodule instantiations:
  - `fft2d_core`, `FilterGen`, `complex_mult`, `ifft_core`.
  - Parameter propagation from top-level into each block.
- Lines 330-381:
  - Sequential bookkeeping for frame capture and output drain.
  - Writes `G(u,v)` memory, writes reordered FFT memory, tracks output completion.
- Lines 383-451:
  - Main frame FSM behavior.
  - Coordinates preload overlap, FFT wait, multiplier/IFFT feed, output drain, and done pulse generation.
- Lines 452-457:
  - End of module.

### 5.2) `fft2d_core` (2D FFT Engine)

Path: `src/fft/SV_fft2d_core.sv`

Code sections:
- Lines 1-36:
  - Module purpose, interface, and performance outputs.
  - Defines narrow streaming contract (one complex sample per cycle externally).
- Lines 38-57:
  - Local parameters, FSM enum, working memories (`mem0`, `mem1`), and batch counters.
  - `mem0` serves as input/final output buffer; `mem1` stores row-pass intermediate.
- Lines 59-61:
  - `idx2d` helper to flatten `(row,col)` into memory index.
- Lines 63-75:
  - Lane-level stream arrays and performance counter signals.
- Lines 76-92:
  - Generate loop instantiating `LANES` copies of `fft1d_core`.
  - This is where throughput parallelism is physically created.
- Lines 94-157:
  - Combinational stage behavior per FSM state.
  - Routes row batches into 1D FFT lanes, receives row results, routes column batches, and drives drain output.
- Lines 159-180:
  - Sequential reset and performance-cycle counting.
  - Starts counting at first accepted input sample, stops at final accepted output.
- Lines 182-197:
  - `ST_LOAD`: loads full `N*N` frame into memory.
- Lines 199-234:
  - `ST_ROW_FEED` and `ST_ROW_RECV`: first FFT pass across rows in lane batches.
- Lines 236-270:
  - `ST_COL_FEED` and `ST_COL_RECV`: second FFT pass across columns in lane batches.
- Lines 272-289:
  - `ST_DRAIN`: streams completed FFT frame out and asserts perf completion.
- Lines 293-295:
  - End of module.

### 5.3) `FilterGen` (Wiener Gain Generator)

Path: `src/wiener_filter/wiener_filter.v`

Code sections:
- Lines 1-30:
  - Module header and Wiener equation contract.
  - Defines fixed-point interpretation for `H`, `K`, and `G`.
- Lines 32-36:
  - Internal width planning for multiply/add/divide path.
  - Protects precision in denominator and quotient math.
- Lines 37:
  - Backpressure relation `ready_in = ~valid_out | ready_out`.
- Lines 39-50:
  - Internal signals for `|H|^2`, denominator, numerators, and quotients.
- Lines 51-67:
  - `sat_to_data` helper clamps quotient result to output width.
- Lines 69-92:
  - Core combinational Wiener math:
  - compute `|H|^2`, align `K`, build safe denominator, construct scaled numerators, perform signed division for `G_real/G_imag`.
- Lines 94-109:
  - Output register stage with valid/ready handshake.
  - Latches saturated `G` outputs only when downstream can accept.
- Lines 110:
  - End of module.

### 5.4) `complex_mult` (Frequency-Domain Complex Multiply)

Path: `src/mult/complex_mult.sv`

Code sections:
- Lines 1-38:
  - Module header and configurable compute style (Gauss 3-mult vs standard 4-mult), optional pipeline, and handshake.
- Lines 40-46:
  - Local width calculations (`SUM_W`, `MUL_W`, `ACC_W`) for safe intermediate arithmetic.
- Lines 47-65:
  - `sat_to_out_w` helper for saturation into output width.
- Lines 67-86:
  - `round_and_shift` helper to normalize fixed-point products back to `FRAC`.
- Lines 88-92:
  - Declares combinational lane outputs.
- Lines 93-165:
  - Per-lane combinational compute (generate loop):
  - unpack lane inputs, compute complex product (3-mult or 4-mult), scale/round, then saturate or wrap.
- Lines 167-195:
  - Optional 1-stage ready/valid output register (`PIPE1=1`).
  - Decouples combinational path and supports backpressure.
- Lines 196-202:
  - Pure combinational pass-through mode (`PIPE1=0`).
- Lines 204:
  - End of module.

### 5.5) `ifft_core` (2D IFFT Engine)

Path: `src/ifft/ifft_core.sv`

Code sections:
- Lines 1-48:
  - Module intent and streaming interfaces.
  - Declares two-pass 2D IFFT strategy for square tiles.
- Lines 49-72:
  - Geometry and width derivation plus parameter legality checks.
  - Enforces `N` power-of-two and square decomposition constraints.
- Lines 74-99:
  - FSM states and frame memories (`mem_*`, `buf_*`) plus twiddle ROM arrays.
- Lines 101-115:
  - Global counters/iterators and saturation bounds.
- Lines 117-128:
  - Bit-reversal function for per-dimension DIT ordering.
- Lines 130-166:
  - Saturation helpers (`sat_to_core`, `sat_to_data`).
- Lines 168-192:
  - Constant quantization and final `1/N` scaling helpers.
- Lines 194-203:
  - Twiddle constant initialization.
- Lines 205-229:
  - Combinational output logic:
  - drives `in_ready/out_valid/out_last`, computes transposed output addressing, applies final scale/saturation.
- Lines 231-283:
  - Sequential engine prologue:
  - reset behavior and temporary arithmetic variable declarations.
- Lines 285-309:
  - `ST_LOAD`: input frame ingest with bit-reversed write layout.
- Lines 311-380:
  - `ST_CALC_PASS1`: row-wise in-place butterfly compute and stage/group iteration.
- Lines 382-410:
  - `ST_TRANSPOSE`: transposes pass1 result into second-pass buffer with bit-reversed preparation.
- Lines 412-481:
  - `ST_CALC_PASS2`: second in-place butterfly pass over transposed data.
- Lines 483-500:
  - `ST_OUT`: output drain, frame completion pulse, and state reset.
- Lines 510-513:
  - End of module.

## 6) Connection And Control Logic (How Data Moves)

This section is a pure description of how modules are connected and what control idea each module uses.

### 6.0) Computer-Architecture Style Used

The design is a **streaming DSP accelerator** built from these architecture patterns:

- **Synchronous streaming dataflow with backpressure**
  - Every boundary uses `valid/ready`, so producers and consumers decouple cleanly.
- **Tile-level scratchpad memory**
  - Full-frame local memories (`y_*`, `g_*`, `mem0/mem1`, `mem/buf`) are used instead of cache.
  - This gives deterministic access and predictable cycle behavior.
- **FSM-orchestrated phase execution**
  - Each major block uses explicit states for load/compute/drain.
  - This is a control-centric architecture, not an out-of-order or speculative model.
- **Time-multiplexed arithmetic datapaths**
  - `butterfly` and pass engines reuse arithmetic hardware across many indices.
  - Saves area while keeping deterministic throughput.
- **Lane-parallel batching**
  - `fft2d_core` replicates multiple 1D cores (`LANES`) to increase throughput in parallel batches.
  - Similar to SIMD lane scaling but implemented as structural RTL replication.

### 6.1) Top-Level Connection (`deblur.sv`)

- `img_*` stream is connected to `fft2d_core` input.
- `h_*` + `k_cfg` stream is connected to `FilterGen` input.
- `fft2d_core` output is written into frame memory `y_*`.
- `FilterGen` output is written into frame memory `g_*`.
- During feed phase, the same index reads `y_*` and `g_*` and sends them into `complex_mult`.
- `complex_mult` output is optionally shifted/scaled, then connected to `ifft_core` input.
- `ifft_core` output is connected directly to `out_*`.

Top-level control idea:
- A frame-level FSM controls when each connection is active:
  - preload inputs,
  - wait for FFT completion,
  - feed multiplier/IFFT,
  - drain output.
- Counters track how many samples have moved through each interface.

Top-level FSM detail:
- `ST_PRELOAD`
  - Accepts image stream (`img_*`) and `H` stream (`h_*`) concurrently.
  - Simultaneously captures FFT output bins as they become available.
  - Exit condition: both image frame and `H` frame fully accepted.
- `ST_WAIT_FFT`
  - Used when preload completed before FFT drain completed.
  - Only waits for `fft_loaded` flag.
- `ST_FEED_IFFT`
  - Reads one aligned bin from `y_*` and `g_*` per accepted multiplier handshake.
  - Sends multiplier output to IFFT input.
  - Exit condition: all frame bins fed (`feed_done`).
- `ST_WAIT_OUT`
  - Drains IFFT output stream and counts accepted output samples.
  - Exit condition: full frame observed on output (`out_done`).
- `ST_DONE`
  - Completion indication state (done pulse behavior) before next frame.

### 6.2) `fft2d_core`: 2D Scheduler Around Multiple 1D Cores

Connection structure:
- `fft2d_core` instantiates `LANES` copies of `fft1d_core`.
- `mem0` and `mem1` are the shared frame memories:
  - `mem0`: initial load and final output,
  - `mem1`: row-pass intermediate.

Control logic idea:
- A 2D FSM schedules movement between memory and 1D cores:
  - `ST_LOAD`: fill `mem0`,
  - `ST_ROW_FEED/RECV`: run row pass,
  - `ST_COL_FEED/RECV`: run column pass,
  - `ST_DRAIN`: stream final FFT bins.
- `batch_base` selects which row/column group is currently assigned to the lane group.
- `feed_cnt/recv_cnt` coordinate lane lockstep so all lanes advance together.

Architecture interpretation:
- This is a **blocked two-pass transform engine** with explicit **producer/consumer phases**.
- Row and column passes are spatially separated by scratchpad storage (`mem1`), which is a classic transpose-buffer style organization.
- Lane lockstep means deterministic wavefront progression across the tile.

### 6.3) `fft1d_core`: Single-Engine 1D FFT Wrapper

Connection structure:
- `fft1d_core` connects three internal blocks:
  - `fft_controller` (schedule generator),
  - `twiddle_rom` (twiddle source),
  - `butterfly` (compute datapath).
- Local sample memory stores one 1D vector in place.

Control logic idea:
- A small FSM (`ST_LOAD`, `ST_RUN`, `ST_OUT`) governs the full vector lifecycle.
- In `ST_RUN`, `fft_controller` emits `(idx_a, idx_b, tw_idx)` for each butterfly.
- `butterfly` computes one pair; results are written back in-place.
- When controller signals done, module drains outputs via stream interface.

FSM detail:
- `ST_LOAD`: exactly `N` samples are accepted into local RAM.
- `ST_RUN`: no external stream traffic; compute is driven by controller/butterfly handshake until all stages complete.
- `ST_OUT`: exactly `N` bins are emitted; then state returns to `ST_LOAD`.

Architecture interpretation:
- This is a **microcoded-like loop engine** where schedule (controller) and datapath (butterfly) are separated.
- In-place memory update reduces memory footprint and external bandwidth demand.

### 6.4) `fft_controller`: Pair/Stage Scheduler

What it controls:
- It does not compute arithmetic.
- It only generates the address and twiddle sequence for radix-2 DIF execution.

Control logic idea:
- Nested-loop logic is implemented in registers/counters:
  - `stage` (FFT stage),
  - `base` (group base index),
  - `j` (position inside group).
- For each issued butterfly:
  - outputs `idx_a`, `idx_b`, `tw_idx`,
  - waits for `bfly_done`,
  - advances to next `(j/base/stage)` state.
- When final stage/group/pair is done, it raises `done`.

Architecture interpretation:
- This module is the **sequencer/scheduler plane** of the FFT.
- Equivalent to translating three nested software loops into a hardware control machine with one operation in flight at a time.

### 6.5) `butterfly`: Arithmetic Compute Micro-Engine

What it does:
- Performs radix-2 butterfly arithmetic on one complex pair.
- Computes:
  - sum/diff scaling,
  - twiddle complex multiply on the diff path.

Control logic idea:
- Uses a short internal FSM to reuse a small multiplier over multiple sub-steps.
- `start` begins operation; `done` pulses when outputs are ready.
- This is compute-focused logic, while ordering/scheduling comes from controller.

FSM detail:
- `S_IDLE`: latches input pair and twiddle.
- `S_M1..S_M4`: computes partial products sequentially.
- Final cycle assembles complex result, applies scaling/saturation, and asserts `done`.

Architecture interpretation:
- This is a **time-multiplexed arithmetic unit** (area-efficient MAC reuse).
- It trades cycles per butterfly for reduced multiplier count.

### 6.6) `FilterGen`: Wiener Coefficient Generator

Connection structure:
- Receives streamed `H_real/H_imag` and scalar `K`.
- Outputs streamed `G_real/G_imag`.
- Uses ready/valid handshake so it can be backpressured by downstream.

Control logic idea:
- Arithmetic is mostly combinational (`|H|^2`, denominator, divide).
- A registered output stage provides handshake-stable streaming behavior.
- Main role in pipeline is to transform blur model bins into per-bin Wiener gains.

Architecture interpretation:
- This behaves like a **streaming functional unit** with one-sample-at-a-time transform.
- Registering output is a small pipeline cut for timing and protocol stability.

### 6.7) `complex_mult`: Frequency-Domain Element-Wise Multiply

Connection structure:
- Takes aligned `Y(u,v)` and `G(u,v)` bins.
- Emits `Yf(u,v)` bins to IFFT.
- Optional internal pipeline register isolates backpressure.

Control logic idea:
- Per lane, computes complex multiplication (3-mult or 4-mult path).
- Applies fixed-point scaling/rounding/saturation policy.
- Handshake logic controls whether data is purely combinational or one-stage buffered.

Architecture interpretation:
- This is an **element-wise vector arithmetic stage** in the frequency-domain stream.
- Optional pipeline register is a classic **throughput/timing decoupling** technique.

### 6.8) `ifft_core`: 2D Inverse Transform Reconstructor

Connection structure:
- Receives filtered frequency bins from `complex_mult`.
- Performs two-pass 2D IFFT using internal memories (`mem_*`, `buf_*`).
- Streams reconstructed spatial-domain samples to `out_*`.

Control logic idea:
- Main FSM controls:
  - input load,
  - pass-1 compute,
  - transpose,
  - pass-2 compute,
  - output drain.
- Internal counters (`row/stage/group/j`) implement butterfly traversal for each pass.
- Final scaling (`1/N`) is applied at output stage.

FSM detail:
- `ST_LOAD`: captures one full frequency tile.
- `ST_CALC_PASS1`: runs row-wise inverse FFT butterflies over all rows.
- `ST_TRANSPOSE`: reorganizes memory layout for second pass.
- `ST_CALC_PASS2`: runs second set of butterflies (equivalent to column inverse FFT).
- `ST_OUT`: emits reconstructed spatial tile with final fixed-point scale.

Architecture interpretation:
- This is another **two-pass transform engine with explicit transpose stage**.
- It mirrors FFT engine principles but applies inverse twiddle/sign and final normalization.
