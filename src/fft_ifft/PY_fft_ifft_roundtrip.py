#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

import numpy as np
from PIL import Image

Q = 32768.0


def load_gray(path: Path) -> np.ndarray:
    return np.asarray(Image.open(path).convert("L"), dtype=np.uint8)


def save_png(path: Path, img_u8: np.ndarray) -> None:
    Image.fromarray(img_u8.astype(np.uint8), mode="L").save(path)


def resize_square(img: np.ndarray, n: int) -> np.ndarray:
    resampling = getattr(Image, "Resampling", Image)
    return np.asarray(Image.fromarray(img).resize((n, n), resampling.BILINEAR), dtype=np.uint8)


def nearest_pow2(x: int) -> int:
    if x <= 1:
        return 2
    lo = 1 << int(math.floor(math.log2(x)))
    hi = 1 << int(math.ceil(math.log2(x)))
    return lo if (x - lo) <= (hi - x) else hi


def is_pow2(x: int) -> bool:
    return x > 0 and (x & (x - 1)) == 0


def resolve_lanes_for_n(req_lanes: int, n: int) -> int:
    lanes = min(max(1, req_lanes), n)
    while lanes > 1 and (n % lanes != 0):
        lanes //= 2
    return lanes


def bitrev_indices(logn: int) -> np.ndarray:
    return np.array([int(f"{i:0{logn}b}"[::-1], 2) for i in range(1 << logn)], dtype=np.int32)


def bitrev2d_to_natural_i64(x: np.ndarray) -> np.ndarray:
    n = x.shape[0]
    idx = bitrev_indices(int(math.log2(n)))
    return x[idx][:, idx, :]


def u8_to_q15(img_u8: np.ndarray) -> np.ndarray:
    x = np.clip((img_u8.astype(np.float64) - 128.0) / 128.0, -1.0, 0.999969)
    return np.clip(np.round(x * Q), -32768, 32767).astype(np.int16)


def generate_twiddle_rom(path: Path, n: int) -> None:
    logn = int(math.log2(n))

    def sv16(v: int) -> str:
        return f"-16'sd{abs(v)}" if v < 0 else f"16'sd{v}"

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


def parse_perf_cycles(stdout: str) -> int | None:
    for ln in stdout.splitlines():
        parts = ln.strip().split()
        if len(parts) == 2 and parts[0] == "PERF_CYCLES":
            try:
                return int(parts[1])
            except ValueError:
                return None
    return None


def parse_complex_tile(path: Path, n: int) -> np.ndarray:
    raw = np.loadtxt(path, dtype=np.int64)
    if raw.ndim == 1:
        raw = raw.reshape(1, -1)
    if raw.shape != (n * n, 2):
        raise ValueError(f"bad shape {raw.shape} for {path}, expected {(n*n, 2)}")
    return raw.reshape(n, n, 2)


def write_complex_tile(path: Path, x: np.ndarray) -> None:
    np.savetxt(path, x.reshape(-1, 2), fmt="%d")


def _is_runnable(bin_path: Path) -> bool:
    if not bin_path.exists() or not os.access(bin_path, os.X_OK):
        return False
    try:
        probe = subprocess.run([str(bin_path)], check=False, capture_output=True, text=True)
        return probe.returncode in (1, 2)
    except OSError:
        return False


def build_fft_hw(tool_dir: Path, n: int, lanes: int) -> Path:
    src_fft_dir = tool_dir.parent / "fft"
    logn = int(math.log2(n))

    build_dir = Path(tempfile.gettempdir()) / "ttsky_fft_ifft_fft_build"
    bin_path = build_dir / "Vfft2d_core"
    meta_path = build_dir / "fft_cfg.txt"

    if bin_path.exists() and meta_path.exists():
        prev = meta_path.read_text(encoding="utf-8").strip()
        if prev == f"{n},{lanes}" and _is_runnable(bin_path):
            return bin_path

    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    twiddle_auto = build_dir / "SV_twiddle_rom_auto.sv"
    generate_twiddle_rom(twiddle_auto, n)

    stage_map = {
        src_fft_dir / "SV_fft2d_core.sv": build_dir / "SV_fft2d_core.sv",
        src_fft_dir / "SV_fft1d_core.sv": build_dir / "SV_fft1d_core.sv",
        src_fft_dir / "SV_butterfly.sv": build_dir / "SV_butterfly.sv",
        src_fft_dir / "SV_fft_controller.sv": build_dir / "SV_fft_controller.sv",
        tool_dir / "CPP_2d_fft_tb.cpp": build_dir / "CPP_2d_fft_tb.cpp",
    }
    for src, dst in stage_map.items():
        shutil.copy2(src, dst)

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
    meta_path.write_text(f"{n},{lanes}\n", encoding="utf-8")
    return bin_path


def build_ifft_hw(tool_dir: Path, n_total: int, data_w: int, frac_w: int) -> Path:
    src_ifft_dir = tool_dir.parent / "ifft"

    build_dir = Path(tempfile.gettempdir()) / "ttsky_fft_ifft_ifft_build"
    bin_path = build_dir / "Vifft_core"
    meta_path = build_dir / "ifft_cfg.txt"

    if bin_path.exists() and meta_path.exists():
        prev = meta_path.read_text(encoding="utf-8").strip()
        if prev == f"{n_total},{data_w},{frac_w}" and _is_runnable(bin_path):
            return bin_path

    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    stage_map = {
        src_ifft_dir / "ifft_core.sv": build_dir / "ifft_core.sv",
        tool_dir / "CPP_2d_ifft_tb.cpp": build_dir / "CPP_2d_ifft_tb.cpp",
    }
    for src, dst in stage_map.items():
        shutil.copy2(src, dst)

    cmd = [
        "verilator",
        "-Wall",
        "-Wno-fatal",
        "--cc",
        "--exe",
        "--build",
        "-O2",
        "--top-module",
        "ifft_core",
        "-Mdir",
        str(build_dir),
        f"-GN={n_total}",
        f"-GDATA_W={data_w}",
        f"-GFRAC_W={frac_w}",
        "-CFLAGS",
        f"-std=c++17 -DIFFT_N={n_total} -DIFFT_DATA_W={data_w}",
        "ifft_core.sv",
        "CPP_2d_ifft_tb.cpp",
    ]
    subprocess.run(cmd, check=True, cwd=build_dir)
    meta_path.write_text(f"{n_total},{data_w},{frac_w}\n", encoding="utf-8")
    return bin_path


def process_image(
    tool_dir: Path,
    img_path: Path,
    outdir: Path,
    order: str,
    tol_max: float,
    tol_rmse: float,
    max_n: int,
    req_lanes: int,
    ifft_data_w: int | None,
    ifft_frac_w: int,
) -> tuple[str, float, float, float, bool, int | None, int, int, int]:
    img = load_gray(img_path)
    n = nearest_pow2(max(img.shape[0], img.shape[1]))
    n = min(max(2, n), max_n)

    lanes = resolve_lanes_for_n(req_lanes, n)
    if lanes != req_lanes:
        print(f"note: adjusted LANES {req_lanes} -> {lanes} for N={n}")

    n_total = n * n
    data_w = ifft_data_w if ifft_data_w is not None else 16

    fft_bin = build_fft_hw(tool_dir, n, lanes)
    ifft_bin = build_ifft_hw(tool_dir, n_total, data_w, ifft_frac_w)

    img_n = resize_square(img, n)
    q = u8_to_q15(img_n)
    x_ref = q.astype(np.float64) / Q

    case_dir = outdir / f"{img_path.stem}_N{n}_L{lanes}"
    case_dir.mkdir(parents=True, exist_ok=True)

    in_txt = case_dir / "input_tile.txt"
    fft_out_txt = case_dir / "dut_fft_out.txt"
    fft_nat_txt = case_dir / "dut_fft_nat.txt"
    ifft_out_txt = case_dir / "dut_ifft_out.txt"

    np.savetxt(in_txt, q.reshape(-1), fmt="%d")

    run_fft = subprocess.run([str(fft_bin), str(in_txt), str(fft_out_txt)], check=True, text=True, capture_output=True)
    perf_cycles = parse_perf_cycles(run_fft.stdout)

    fft_raw = parse_complex_tile(fft_out_txt, n)
    if order == "bitrev2d":
        fft_nat = bitrev2d_to_natural_i64(fft_raw)
    else:
        fft_nat = fft_raw

    # Option 1: pre-scale FFT bins before feeding IFFT so the IFFT's built-in
    # normalization does not collapse the signal magnitude.
    fft_ifft_in = fft_nat.astype(np.int64) * n_total
    hi = (1 << (data_w - 1)) - 1
    lo = -(1 << (data_w - 1))
    clip_count = int(np.count_nonzero((fft_ifft_in > hi) | (fft_ifft_in < lo)))
    if clip_count:
        print(f"note: pre-IFFT scaling clipped {clip_count} values at DATA_W={data_w}")
    fft_ifft_in = np.clip(fft_ifft_in, lo, hi)

    write_complex_tile(fft_nat_txt, fft_ifft_in)

    subprocess.run([str(ifft_bin), str(fft_nat_txt), str(ifft_out_txt)], check=True)

    ifft_raw = parse_complex_tile(ifft_out_txt, n)
    x_rec = ifft_raw[:, :, 0].astype(np.float64) / Q
    x_im = ifft_raw[:, :, 1].astype(np.float64) / Q

    err = x_rec - x_ref
    max_err = float(np.max(np.abs(err)))
    rmse = float(np.sqrt(np.mean(err * err)))
    imag_rmse = float(np.sqrt(np.mean(x_im * x_im)))
    ok = (max_err <= tol_max) and (rmse <= tol_rmse)

    u8_rec = np.clip(np.round(x_rec * 128.0 + 128.0), 0, 255).astype(np.uint8)
    diff_u8 = np.clip(np.abs(img_n.astype(np.int16) - u8_rec.astype(np.int16)), 0, 255).astype(np.uint8)

    save_png(case_dir / "original.png", img_n)
    save_png(case_dir / "reconstructed.png", u8_rec)
    save_png(case_dir / "difference.png", diff_u8)

    with (case_dir / "metrics.txt").open("w", encoding="utf-8") as fp:
        fp.write(
            f"size={n}\n"
            f"lanes={lanes}\n"
            f"ifft_n_total={n_total}\n"
            f"ifft_data_w={data_w}\n"
            f"ifft_frac_w={ifft_frac_w}\n"
            f"ifft_input_prescale={n_total}\n"
            f"ifft_input_clip_count={clip_count}\n"
            f"order={order}\n"
            f"max_abs_error={max_err:.8e}\n"
            f"rmse={rmse:.8e}\n"
            f"imag_rmse={imag_rmse:.8e}\n"
            f"pass={int(ok)}\n"
        )
        if perf_cycles is not None:
            fp.write(f"fft_perf_cycles={perf_cycles}\n")

    return case_dir.name, max_err, rmse, imag_rmse, ok, perf_cycles, n, lanes, data_w


def main() -> int:
    p = argparse.ArgumentParser(
        description="Round-trip check using hardware FFT (2D) + hardware IFFT (single-frame 2D ifft_core)."
    )
    p.add_argument("--images-dir", type=Path, default=Path("image"), help="input grayscale image folder")
    p.add_argument("--outdir", type=Path, default=Path("results"), help="result root folder")
    p.add_argument("--order", choices=["natural", "bitrev2d"], default="bitrev2d")
    p.add_argument("--tol-max", type=float, default=2.0)
    p.add_argument("--tol-rmse", type=float, default=0.02)
    p.add_argument("--max-n", type=int, default=256, help="cap FFT size to limit runtime")
    p.add_argument("--lanes", type=int, default=1, help="parallel FFT lanes (power-of-2)")
    p.add_argument("--ifft-data-w", type=int, default=None, help="ifft_core DATA_W override; default is 16")
    p.add_argument("--ifft-frac-w", type=int, default=15, help="ifft_core FRAC_W (twiddle fractional bits)")
    args = p.parse_args()

    if not is_pow2(args.lanes):
        raise ValueError(f"--lanes must be power-of-2, got {args.lanes}")
    if args.lanes < 1:
        raise ValueError("--lanes must be >= 1")
    if args.ifft_data_w is not None and args.ifft_data_w < 16:
        raise ValueError("--ifft-data-w must be >= 16 when provided")

    args.images_dir.mkdir(parents=True, exist_ok=True)
    args.outdir.mkdir(parents=True, exist_ok=True)

    exts = {".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff"}
    imgs = sorted([p for p in args.images_dir.iterdir() if p.is_file() and p.suffix.lower() in exts])
    if not imgs:
        print(f"No images found in {args.images_dir}. Put grayscale images there and rerun.")
        return 0

    tool_dir = Path(__file__).resolve().parent
    for img in imgs:
        name, max_err, rmse, imag_rmse, ok, cycles, n, lanes, data_w = process_image(
            tool_dir,
            img,
            args.outdir,
            args.order,
            args.tol_max,
            args.tol_rmse,
            args.max_n,
            args.lanes,
            args.ifft_data_w,
            args.ifft_frac_w,
        )
        perf = f", fft_cycles={cycles}" if cycles is not None else ""
        print(
            f"{name}: N={n}, lanes={lanes}, ifft_data_w={data_w}, "
            f"max_abs_error={max_err:.6e}, rmse={rmse:.6e}, imag_rmse={imag_rmse:.6e}{perf}, "
            f"{'PASS' if ok else 'WARN'}"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
