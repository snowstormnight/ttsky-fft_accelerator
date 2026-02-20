#!/usr/bin/env python3
from __future__ import annotations

# End-to-end automation script for the FFT hardware pipeline.
#
# Per input image:
# 1) Load grayscale image from ./image (or user path).
# 2) Resize to square power-of-2 (nearest, capped by --max-n).
# 3) Convert uint8 -> signed Q1.15 and write input_tile.txt (one int per line).
# 4) Build Verilated hardware for that N if needed.
# 5) Run C++ testbench: streams tile into SV FFT, writes dut_fft_out.txt.
# 6) Read DUT complex FFT output, reorder from bit-reversed if requested.
# 7) Reconstruct image with NumPy IFFT and compare against original normalized data.
# 8) Save original/reconstructed/difference PNG plus metrics.txt.

import argparse
import math
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

import numpy as np

# Q1.15 scaling factor:
#   float value ~= int_q15 / 32768
Q = 32768.0


def load_gray(path: Path) -> np.ndarray:
    # Prefer OpenCV for speed/availability; fallback to PIL.
    try:
        import cv2  # type: ignore
        img = cv2.imread(str(path), cv2.IMREAD_GRAYSCALE)
        if img is None:
            raise RuntimeError(f"failed to read image: {path}")
        return img.astype(np.uint8)
    except Exception:
        from PIL import Image  # type: ignore
        return np.asarray(Image.open(path).convert("L"), dtype=np.uint8)


def save_png(path: Path, img_u8: np.ndarray) -> None:
    # Keep save path backend-agnostic (OpenCV first, PIL fallback).
    try:
        import cv2  # type: ignore
        cv2.imwrite(str(path), img_u8)
    except Exception:
        from PIL import Image  # type: ignore
        Image.fromarray(img_u8).save(path)


def resize_square(img: np.ndarray, n: int) -> np.ndarray:
    # Resize to exactly NxN; area interpolation for downscale, linear for upscale.
    try:
        import cv2  # type: ignore
        out = cv2.resize(img, (n, n), interpolation=cv2.INTER_AREA if n < img.shape[0] else cv2.INTER_LINEAR)
        return out.astype(np.uint8)
    except Exception:
        from PIL import Image  # type: ignore
        return np.asarray(Image.fromarray(img).resize((n, n), Image.BILINEAR), dtype=np.uint8)


def nearest_pow2(x: int) -> int:
    # Return nearest power of two to x.
    # Tie goes to lower power due to <= in comparison.
    if x <= 1:
        return 2
    lo = 1 << int(math.floor(math.log2(x)))
    hi = 1 << int(math.ceil(math.log2(x)))
    return lo if (x - lo) <= (hi - x) else hi


def bitrev_indices(logn: int) -> np.ndarray:
    # Build index map where position i maps to bit-reversed(i).
    return np.array([int(f"{i:0{logn}b}"[::-1], 2) for i in range(1 << logn)], dtype=np.int32)


def bitrev2d_to_natural(x: np.ndarray) -> np.ndarray:
    # Apply bit-reversal reorder on rows and columns.
    # Used because current hardware outputs DIF order on both dimensions.
    n = x.shape[0]
    idx = bitrev_indices(int(math.log2(n)))
    return x[np.ix_(idx, idx)]


def u8_to_q15(img_u8: np.ndarray) -> np.ndarray:
    # Map uint8 [0..255] -> normalized float [-1, 1) via (u8-128)/128,
    # then quantize to Q1.15 signed int16.
    x = np.clip((img_u8.astype(np.float64) - 128.0) / 128.0, -1.0, 0.999969)
    return np.clip(np.round(x * Q), -32768, 32767).astype(np.int16)


def generate_twiddle_rom(path: Path, n: int) -> None:
    # Auto-generate twiddle ROM source for current FFT size N.
    # This avoids maintaining many static ROM files by hand.
    logn = int(math.log2(n))
    lines = [
        "`default_nettype none",
        "",
        f"// Auto-generated Q1.15 twiddle ROM for W{n}^k, k=0..{n//2 - 1}.",
        "module twiddle_rom #(",
        f"    parameter int LOGN = {logn}",
        ") (",
        "    input  logic [LOGN-1:0]         idx,",
        "    output logic signed [15:0]      w_re,",
        "    output logic signed [15:0]      w_im",
        ");",
        "",
        "  always_comb begin",
        "    unique case (idx)",
    ]
    def sv16(v: int) -> str:
        return f"-16'sd{abs(v)}" if v < 0 else f"16'sd{v}"

    for k in range(n // 2):
        re = int(round(math.cos(2 * math.pi * k / n) * Q))
        im = int(round(-math.sin(2 * math.pi * k / n) * Q))
        re = max(-32768, min(32767, re))
        im = max(-32768, min(32767, im))
        lines.append(f"      {logn}'d{k}: begin w_re = {sv16(re)}; w_im = {sv16(im)}; end")
    lines += [
        "      default: begin w_re = 16'sd0; w_im = 16'sd0; end",
        "    endcase",
        "  end",
        "",
        "endmodule",
        "",
        "`default_nettype wire",
    ]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def build_hw(repo_fft_dir: Path, n: int, lanes: int) -> Path:
    # Build or reuse Verilated binary for requested N.
    # Rebuild is triggered when previous (N,LANES) differs.
    logn = int(math.log2(n))
    # Verilator's generated Makefiles fail when build directory path contains spaces.
    # Keep build artifacts under /tmp to stay robust on paths like ".../ASIC Hack/...".
    build_dir = Path(tempfile.gettempdir()) / "ttsky_fft_verilator_build"
    bin_path = build_dir / "Vfft2d_core"
    meta_path = build_dir / "fft_cfg.txt"

    if bin_path.exists() and meta_path.exists():
        prev_cfg = meta_path.read_text(encoding="utf-8").strip()
        if prev_cfg == f"{n},{lanes}" and os.access(bin_path, os.X_OK):
            # Validate binary is runnable on this host (reject stale foreign-arch artifacts).
            try:
                probe = subprocess.run(
                    [str(bin_path)],
                    cwd=repo_fft_dir,
                    capture_output=True,
                    text=True,
                    check=False,
                )
                if probe.returncode in (1, 2):
                    return bin_path
            except OSError:
                pass

    if build_dir.exists():
        # Rebuild if target FFT size changed.
        import shutil
        shutil.rmtree(build_dir)

    build_dir.mkdir(parents=True, exist_ok=True)
    twiddle_auto = build_dir / "SV_twiddle_rom_auto.sv"
    generate_twiddle_rom(twiddle_auto, n)

    # Stage RTL/testbench sources in the temp build folder so generated Makefiles
    # do not depend on space-containing absolute paths.
    staged_files = [
        "SV_fft2d_core.sv",
        "SV_fft1d_core.sv",
        "SV_butterfly.sv",
        "SV_fft_controller.sv",
        "CPP_2d_fft_tb.cpp",
    ]
    for fname in staged_files:
        shutil.copy2(repo_fft_dir / fname, build_dir / fname)

    # Verilator compile command:
    # - Parameterize SV top with -GN/-GLOGN/-GLANES
    # - Parameterize C++ with -DFFT_N/-DFFT_LOGN/-DFFT_LANES
    cmd = [
        "verilator",
        "-Wall",
        "-Wno-fatal",
        "--cc",
        "--exe",
        "--build",
        "-O2",
        "--top-module",
        "fft2d_core",
        "-Mdir",
        str(build_dir),
        f"-GN={n}",
        f"-GLOGN={logn}",
        f"-GLANES={lanes}",
        "-CFLAGS",
        f"-std=c++17 -DFFT_N={n} -DFFT_LOGN={logn} -DFFT_LANES={lanes}",
        "SV_fft2d_core.sv",
        "SV_fft1d_core.sv",
        "SV_butterfly.sv",
        "SV_fft_controller.sv",
        "SV_twiddle_rom_auto.sv",
        "CPP_2d_fft_tb.cpp",
    ]
    subprocess.run(cmd, check=True, cwd=build_dir)
    if not bin_path.exists():
        raise RuntimeError(f"build failed: {bin_path} not produced")
    meta_path.write_text(f"{n},{lanes}\n", encoding="utf-8")
    return bin_path


def parse_hw_fft(path: Path, n: int) -> np.ndarray:
    # Parse DUT file with "re im" per line into complex64-like ndarray.
    raw = np.loadtxt(path, dtype=np.int64)
    if raw.ndim == 1:
        raw = raw.reshape(1, -1)
    if raw.shape != (n * n, 2):
        raise ValueError(f"bad DUT output shape {raw.shape}, expected {(n*n, 2)}")
    return (raw[:, 0].astype(np.float64) + 1j * raw[:, 1].astype(np.float64)).reshape(n, n) / Q


def parse_perf_cycles(stdout: str) -> int | None:
    # Parse machine-readable perf line emitted by C++:
    #   PERF_CYCLES <integer>
    for ln in stdout.splitlines():
        parts = ln.strip().split()
        if len(parts) == 2 and parts[0] == "PERF_CYCLES":
            try:
                return int(parts[1])
            except ValueError:
                return None
    return None


def is_pow2(x: int) -> bool:
    return x > 0 and (x & (x - 1)) == 0


def resolve_lanes_for_n(req_lanes: int, n: int) -> int:
    # Keep lane count power-of-2 and valid for this N.
    if req_lanes < 1:
        return 1
    lanes = req_lanes
    if lanes > n:
        lanes = n
    while lanes > 1 and (n % lanes != 0):
        lanes //= 2
    return max(1, lanes)


def choose_lanes(user_lanes: int | None) -> int:
    # If not given on CLI, ask interactively once.
    if user_lanes is not None:
        lanes = user_lanes
    else:
        raw = input("LANES (power-of-2, default 1): ").strip()
        lanes = 1 if raw == "" else int(raw)
    if not is_pow2(lanes):
        raise ValueError(f"LANES must be power-of-2, got {lanes}")
    if lanes < 1:
        raise ValueError("LANES must be >= 1")
    return lanes


def process_image(
    img_path: Path,
    outdir: Path,
    order: str,
    tol_max: float,
    tol_rmse: float,
    max_n: int,
    req_lanes: int,
) -> tuple[str, float, float, bool, int | None]:
    # ---- Input preparation ----
    # img        : original uint8 grayscale image loaded from disk.
    # n          : target FFT dimension after nearest power-of-2 + cap.
    # img_n      : resized NxN uint8 tile.
    # q          : int16 Q1.15 tile.
    # x_ref      : normalized float tile used as comparison baseline.
    img = load_gray(img_path)
    n = nearest_pow2(max(img.shape[0], img.shape[1]))
    if n < 2:
        n = 2
    if n > max_n:
        n = max_n

    lanes = resolve_lanes_for_n(req_lanes, n)
    if lanes != req_lanes:
        print(f"note: adjusted LANES {req_lanes} -> {lanes} for N={n}")
    fft_dir = Path(__file__).resolve().parent
    fft_bin = build_hw(fft_dir, n, lanes)

    img_n = resize_square(img, n)
    q = u8_to_q15(img_n)
    x_ref = q.astype(np.float64) / Q

    # Case output directory name includes image stem and resolved N.
    case_dir = outdir / f"{img_path.stem}_N{n}_L{lanes}"
    case_dir.mkdir(parents=True, exist_ok=True)
    in_txt = case_dir / "input_tile.txt"
    out_txt = case_dir / "dut_fft_out.txt"
    np.savetxt(in_txt, q.reshape(-1), fmt="%d")

    # Run hardware model:
    # - input_tile.txt -> C++ -> SV FFT2D -> dut_fft_out.txt
    # Capture stdout to extract perf counter line.
    run = subprocess.run([str(fft_bin), str(in_txt), str(out_txt)], check=True, text=True, capture_output=True)
    perf_cycles = parse_perf_cycles(run.stdout)
    f_hw = parse_hw_fft(out_txt, n)
    if order == "bitrev2d":
        f_hw = bitrev2d_to_natural(f_hw)

    # Hardware has per-stage divide-by-2 scaling in both row and column passes.
    # Net FFT scaling is 1/(N*N), so multiply back by N*N before ifft2.
    x_rec = np.real(np.fft.ifft2(f_hw * (n * n)))
    u8_rec = np.clip(np.round(x_rec * 128.0 + 128.0), 0, 255).astype(np.uint8)

    # Error in normalized float domain (not uint8), then pass/fail thresholds.
    err = x_rec - x_ref
    max_err = float(np.max(np.abs(err)))
    rmse = float(np.sqrt(np.mean(err * err)))
    ok = (max_err <= tol_max) and (rmse <= tol_rmse)

    save_png(case_dir / "original.png", img_n)
    save_png(case_dir / "reconstructed.png", u8_rec)
    diff_u8 = np.clip(np.abs(img_n.astype(np.int16) - u8_rec.astype(np.int16)), 0, 255).astype(np.uint8)
    save_png(case_dir / "difference.png", diff_u8)
    # Persist numeric report for scripts/CI/manual review.
    with (case_dir / "metrics.txt").open("w", encoding="utf-8") as fp:
        fp.write(
            f"size={n}\nlanes={lanes}\nmax_abs_error={max_err:.8e}\nrmse={rmse:.8e}\npass={int(ok)}\n"
        )
        if perf_cycles is not None:
            fp.write(f"perf_cycles={perf_cycles}\n")

    return case_dir.name, max_err, rmse, ok, perf_cycles


def main() -> int:
    p = argparse.ArgumentParser(description="Process grayscale images: HW FFT -> NumPy IFFT -> compare.")
    p.add_argument("--images-dir", type=Path, default=Path("image"), help="input grayscale image folder")
    p.add_argument("--outdir", type=Path, default=Path("results"), help="result root folder")
    p.add_argument("--order", choices=["natural", "bitrev2d"], default="bitrev2d")
    p.add_argument("--tol-max", type=float, default=2.0)
    p.add_argument("--tol-rmse", type=float, default=0.02)
    p.add_argument("--max-n", type=int, default=256, help="cap FFT size to avoid huge simulations")
    p.add_argument("--lanes", type=int, default=None, help="parallel 1D FFT lanes (power-of-2). If omitted, prompt.")
    args = p.parse_args()

    # Ensure folders exist so first run is smooth.
    args.images_dir.mkdir(parents=True, exist_ok=True)
    args.outdir.mkdir(parents=True, exist_ok=True)

    exts = {".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff"}
    imgs = sorted([p for p in args.images_dir.iterdir() if p.is_file() and p.suffix.lower() in exts])
    if not imgs:
        print(f"No images found in {args.images_dir}. Put grayscale images there and rerun.")
        return 0

    req_lanes = choose_lanes(args.lanes)

    # Process each image independently; each produces its own result folder.
    for img in imgs:
        name, max_err, rmse, ok, perf_cycles = process_image(
            img, args.outdir, args.order, args.tol_max, args.tol_rmse, args.max_n, req_lanes
        )
        extra = f", cycles={perf_cycles}" if perf_cycles is not None else ""
        print(f"{name}: max_abs_error={max_err:.6e}, rmse={rmse:.6e}{extra}, {'PASS' if ok else 'WARN'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
