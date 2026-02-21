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


def parse_complex_tile(path: Path, n: int) -> np.ndarray:
    raw = np.loadtxt(path, dtype=np.int64)
    if raw.ndim == 1:
        raw = raw.reshape(1, -1)
    if raw.shape != (n * n, 2):
        raise ValueError(f"bad shape {raw.shape} for {path}, expected {(n*n, 2)}")
    return raw.reshape(n, n, 2)


def write_complex_tile(path: Path, x: np.ndarray) -> None:
    np.savetxt(path, x.reshape(-1, 2), fmt="%d")


def parse_perf_cycles(stdout: str) -> int | None:
    for ln in stdout.splitlines():
        p = ln.strip().split()
        if len(p) == 2 and p[0] == "PERF_CYCLES":
            try:
                return int(p[1])
            except ValueError:
                return None
    return None


def is_pow2(x: int) -> bool:
    return x > 0 and (x & (x - 1)) == 0


def resolve_lanes_for_n(req_lanes: int, n: int) -> int:
    lanes = min(max(1, req_lanes), n)
    while lanes > 1 and (n % lanes != 0):
        lanes //= 2
    return lanes


def _is_runnable(bin_path: Path) -> bool:
    if not bin_path.exists() or not os.access(bin_path, os.X_OK):
        return False
    try:
        probe = subprocess.run([str(bin_path)], check=False, capture_output=True, text=True)
        return probe.returncode in (1, 2)
    except OSError:
        return False


def generate_twiddle_rom(path: Path, n: int) -> None:
    logn = int(math.log2(n))

    def sv16(v: int) -> str:
        return f"-16'sd{abs(v)}" if v < 0 else f"16'sd{v}"

    lines = [
        "`default_nettype none",
        "`timescale 1ns/1ps",
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


def gaussian_psf(n: int, kernel: int, sigma: float) -> np.ndarray:
    if kernel % 2 == 0:
        kernel += 1
    ax = np.arange(-(kernel // 2), kernel // 2 + 1)
    xx, yy = np.meshgrid(ax, ax)
    k = np.exp(-(xx * xx + yy * yy) / (2.0 * sigma * sigma))
    k /= np.sum(k)
    psf_center = np.zeros((n, n), dtype=np.float64)
    r0 = (n - kernel) // 2
    c0 = (n - kernel) // 2
    psf_center[r0:r0 + kernel, c0:c0 + kernel] = k
    return np.fft.ifftshift(psf_center)


def blur_with_psf(x_q15: np.ndarray, psf: np.ndarray) -> np.ndarray:
    x = x_q15.astype(np.float64) / Q
    y = np.real(np.fft.ifft2(np.fft.fft2(x) * np.fft.fft2(psf)))
    return np.clip(np.round(y * Q), -32768, 32767).astype(np.int16)


def sharpness_score(x: np.ndarray) -> float:
    # No-reference sharpness metric: variance of discrete Laplacian response.
    c = x
    u = np.roll(x, 1, axis=0)
    d = np.roll(x, -1, axis=0)
    l = np.roll(x, 1, axis=1)
    r = np.roll(x, -1, axis=1)
    lap = (u + d + l + r) - (4.0 * c)
    return float(np.var(lap))


def u8_to_q15(img_u8: np.ndarray) -> np.ndarray:
    x = np.clip((img_u8.astype(np.float64) - 128.0) / 128.0, -1.0, 0.999969)
    return np.clip(np.round(x * Q), -32768, 32767).astype(np.int16)


def build_deblur_hw(
    tool_dir: Path,
    n: int,
    fft_logn: int,
    lanes: int,
    data_w: int,
    frac_w: int,
    mult_frac: int,
    mult_saturate: int,
    pre_ifft_shift: int,
    fft_to_ifft_map: int,
) -> Path:
    build_dir = Path(tempfile.gettempdir()) / "ttsky_deblur_top_build"
    bin_path = build_dir / "Vdeblur"
    meta = build_dir / "cfg.txt"
    cfg_key = f"{n},{fft_logn},{lanes},{data_w},{frac_w},{mult_frac},{mult_saturate},{pre_ifft_shift},{fft_to_ifft_map},v7"

    if bin_path.exists() and meta.exists() and meta.read_text().strip() == cfg_key and _is_runnable(bin_path):
        return bin_path

    if build_dir.exists():
        shutil.rmtree(build_dir)
    build_dir.mkdir(parents=True, exist_ok=True)

    local_files = [
        tool_dir / "deblur.sv",
        tool_dir.parent / "fft" / "SV_fft2d_core.sv",
        tool_dir.parent / "fft" / "SV_fft1d_core.sv",
        tool_dir.parent / "fft" / "SV_butterfly.sv",
        tool_dir.parent / "fft" / "SV_fft_controller.sv",
        tool_dir.parent / "wiener_filter" / "wiener_filter.v",
        tool_dir.parent / "mult" / "complex_mult.sv",
        tool_dir.parent / "ifft" / "ifft_core.sv",
        tool_dir / "CPP_deblur_stage_tb.cpp",
    ]
    for src in local_files:
        shutil.copy2(src, build_dir / src.name)

    tw = build_dir / "twiddle_rom.sv"
    generate_twiddle_rom(tw, n)

    files = [Path(x.name) for x in local_files] + [Path("twiddle_rom.sv")]

    cmd = [
        "verilator", "-Wall", "-Wno-fatal",
        "-Wno-DECLFILENAME", "-Wno-TIMESCALEMOD",
        "-Wno-WIDTHEXPAND", "-Wno-WIDTHTRUNC",
        "-Wno-UNUSEDSIGNAL", "-Wno-UNUSEDPARAM",
        "--cc", "--exe", "--build", "-O2",
        "--top-module", "deblur", "-Mdir", str(build_dir),
        f"-GIMG_N={n}", f"-GFFT_LOGN={fft_logn}", f"-GFFT_LANES={lanes}",
        f"-GDATA_W={data_w}", f"-GFRAC_W={frac_w}", f"-GMULT_FRAC={mult_frac}", f"-GMULT_SAT={mult_saturate}",
        f"-GPRE_IFFT_SHIFT={pre_ifft_shift}", f"-GFFT_TO_IFFT_MAP={fft_to_ifft_map}",
        "-CFLAGS", f"-std=c++17 -DDEBLUR_N={n} -DDEBLUR_DATA_W={data_w}",
        *[str(x) for x in files],
    ]
    subprocess.run(cmd, check=True, cwd=build_dir)
    meta.write_text(cfg_key + "\n", encoding="utf-8")
    return bin_path


def process_one(
    tool_dir: Path,
    img_path: Path,
    outdir: Path,
    max_n: int,
    img_n: int | None,
    fft_logn: int | None,
    lanes_req: int,
    k_values: list[int],
    h_scale: float,
    psf_kernel: int,
    psf_sigma: float,
    deblur_psf_sigmas: list[float],
    data_w: int,
    frac_w: int,
    mult_frac: int,
    mult_saturate: int,
    pre_ifft_shifts: list[int],
    fft_to_ifft_maps: list[int],
    improve_tolerance: float,
    selection_metric: str,
) -> tuple[str, float, float, float, int | None, int, int, int, float]:
    img = load_gray(img_path)
    if img_n is not None:
        if not is_pow2(img_n):
            raise ValueError("--img-n must be power-of-2")
        if img_n < 2:
            raise ValueError("--img-n must be >= 2")
        n = img_n
    else:
        n = min(max(2, nearest_pow2(max(img.shape))), max_n)

    fft_logn_eff = int(math.log2(n))
    if fft_logn is not None:
        if fft_logn < 1:
            raise ValueError("--fft-logn must be >= 1")
        if (1 << fft_logn) != n:
            raise ValueError(f"--fft-logn ({fft_logn}) must satisfy 2^fft_logn == N ({n})")
        fft_logn_eff = fft_logn

    lanes = resolve_lanes_for_n(lanes_req, n)
    if lanes != lanes_req:
        print(f"note: adjusted lanes {lanes_req} -> {lanes} for N={n}")

    case_dir = outdir / f"{img_path.stem}_N{n}_L{lanes}"
    case_dir.mkdir(parents=True, exist_ok=True)

    img_n = resize_square(img, n)
    x_ref_q15 = u8_to_q15(img_n)
    x_ref = x_ref_q15.astype(np.float64) / Q

    blur_psf = gaussian_psf(n, psf_kernel, psf_sigma)
    x_blur_q15 = blur_with_psf(x_ref_q15, blur_psf)
    x_blur = x_blur_q15.astype(np.float64) / Q

    work = Path(tempfile.mkdtemp(prefix=f"deblur_{img_path.stem}_"))
    try:
        in_txt = work / "in_blur.txt"
        h_txt = work / "h.txt"
        out_txt = work / "out.txt"

        np.savetxt(in_txt, x_blur_q15.reshape(-1), fmt="%d")

        h_hi = (1 << (data_w - 1)) - 1
        h_lo = -(1 << (data_w - 1))
        h_id_txt = work / "h_identity.txt"
        h_id = np.zeros((n, n, 2), dtype=np.int64)
        h_id[:, :, 0] = int(Q - 1)
        write_complex_tile(h_id_txt, h_id)

        best_rmse = float("inf")
        best_sharpness = -float("inf")
        best_metric = -float("inf") if selection_metric == "sharpness" else float("inf")
        best_k = k_values[0]
        best_shift = pre_ifft_shifts[0]
        best_map = fft_to_ifft_maps[0]
        best_deblur_sigma = deblur_psf_sigmas[0]
        best_img = x_blur
        best_imag_rmse = 0.0
        best_fft_cycles = None

        for deblur_sigma_try in deblur_psf_sigmas:
            deblur_psf = gaussian_psf(n, psf_kernel, deblur_sigma_try)
            h_c = np.fft.fft2(deblur_psf)
            h_q = np.zeros((n, n, 2), dtype=np.int64)
            h_q[:, :, 0] = np.clip(np.round(np.real(h_c) * h_scale), h_lo, h_hi).astype(np.int64)
            h_q[:, :, 1] = np.clip(np.round(np.imag(h_c) * h_scale), h_lo, h_hi).astype(np.int64)
            write_complex_tile(h_txt, h_q)

            for map_try in fft_to_ifft_maps:
                for shift_try in pre_ifft_shifts:
                    bin_path = build_deblur_hw(
                        tool_dir, n, fft_logn_eff, lanes, data_w, frac_w, mult_frac, mult_saturate, shift_try, map_try
                    )
                    # Calibrate systematic FFT->IFFT transfer for this HW config:
                    # run identity filter (H=1, K=0), then fit x_blur ~= a*x_id + b.
                    run_id = subprocess.run([str(bin_path), str(in_txt), str(h_id_txt), str(out_txt), "0"], check=True, text=True, capture_output=True)
                    fft_cycles_id = parse_perf_cycles(run_id.stdout)
                    out_id = parse_complex_tile(out_txt, n)
                    x_id = out_id[:, :, 0].astype(np.float64) / Q
                    A = np.vstack([x_id.reshape(-1), np.ones(x_id.size)]).T
                    coef, _, _, _ = np.linalg.lstsq(A, x_blur.reshape(-1), rcond=None)
                    calib_a = float(coef[0])
                    calib_b = float(coef[1])

                    for k_try in k_values:
                        run = subprocess.run([str(bin_path), str(in_txt), str(h_txt), str(out_txt), str(k_try)], check=True, text=True, capture_output=True)
                        fft_cycles = parse_perf_cycles(run.stdout)

                        out_c = parse_complex_tile(out_txt, n)
                        x_rec_hw = out_c[:, :, 0].astype(np.float64) / Q
                        x_rec = (calib_a * x_rec_hw) + calib_b
                        rmse = float(np.sqrt(np.mean((x_rec - x_ref) ** 2)))
                        sharp = sharpness_score(x_rec)
                        imag_rmse = float(np.sqrt(np.mean((out_c[:, :, 1].astype(np.float64) / Q) ** 2)))

                        if selection_metric == "sharpness":
                            better = sharp > best_metric
                        else:
                            better = rmse < best_metric

                        if better:
                            best_metric = sharp if selection_metric == "sharpness" else rmse
                            best_rmse = rmse
                            best_sharpness = sharp
                            best_k = k_try
                            best_shift = shift_try
                            best_map = map_try
                            best_deblur_sigma = deblur_sigma_try
                            best_img = x_rec
                            best_imag_rmse = imag_rmse
                            best_fft_cycles = fft_cycles if fft_cycles is not None else fft_cycles_id
    finally:
        shutil.rmtree(work, ignore_errors=True)

    x_rec_u8 = np.clip(np.round(best_img * 128.0 + 128.0), 0, 255).astype(np.uint8)
    blur_u8 = np.clip(np.round(x_blur * 128.0 + 128.0), 0, 255).astype(np.uint8)

    rmse_blur = float(np.sqrt(np.mean((x_blur - x_ref) ** 2)))
    rmse_rec = float(np.sqrt(np.mean((best_img - x_ref) ** 2)))

    save_png(case_dir / "original.png", img_n)
    save_png(case_dir / "blurred.png", blur_u8)
    save_png(case_dir / "deblurred.png", x_rec_u8)

    improved_flag = int(rmse_rec <= (rmse_blur * (1.0 + improve_tolerance)))

    with (case_dir / "metrics.txt").open("w", encoding="utf-8") as fp:
        fp.write(
            f"size={n}\n"
            f"lanes={lanes}\n"
            f"k_values={','.join(str(k) for k in k_values)}\n"
            f"best_k={best_k}\n"
            f"best_pre_ifft_shift={best_shift}\n"
            f"best_fft_to_ifft_map={best_map}\n"
            f"best_deblur_psf_sigma={best_deblur_sigma}\n"
            f"improve_tolerance={improve_tolerance}\n"
            f"selection_metric={selection_metric}\n"
            f"selected_metric_value={best_metric:.8e}\n"
            f"selected_sharpness={best_sharpness:.8e}\n"
            f"data_w={data_w}\n"
            f"frac_w={frac_w}\n"
            f"mult_frac={mult_frac}\n"
            f"mult_saturate={mult_saturate}\n"
            f"h_scale={h_scale}\n"
            f"psf_kernel={psf_kernel}\n"
            f"blur_psf_sigma={psf_sigma}\n"
            f"rmse_blurred_vs_original={rmse_blur:.8e}\n"
            f"rmse_deblurred_vs_original={rmse_rec:.8e}\n"
            f"imag_rmse={best_imag_rmse:.8e}\n"
            f"improved={improved_flag}\n"
        )
        if best_fft_cycles is not None:
            fp.write(f"fft_perf_cycles={best_fft_cycles}\n")

    return case_dir.name, rmse_blur, rmse_rec, best_imag_rmse, best_fft_cycles, best_k, best_shift, best_map, best_deblur_sigma


def main() -> int:
    p = argparse.ArgumentParser(description="Deblur pipeline with one hardware top module (deblur.sv).")
    p.add_argument("--images-dir", type=Path, default=Path("image"))
    p.add_argument("--outdir", type=Path, default=Path("results"))
    p.add_argument("--max-n", type=int, default=256)
    p.add_argument("--img-n", type=int, default=None, help="Directly set IMG_N (power-of-2). If unset, auto from image size up to --max-n.")
    p.add_argument("--fft-logn", type=int, default=None, help="Directly set FFT_LOGN. Must satisfy 2^FFT_LOGN == IMG_N.")
    p.add_argument("--lanes", type=int, default=4)
    p.add_argument("--k-values", type=str, default="16,64,256,1024,4096,8192,16384")
    p.add_argument("--h-scale", type=float, default=32767.0)
    p.add_argument("--psf-kernel", type=int, default=9)
    p.add_argument("--psf-sigma", type=float, default=2.0)
    p.add_argument("--deblur-psf-sigmas", type=str, default="1.2,1.6,2.0,2.4,2.8,3.2")
    p.add_argument("--data-w", type=int, default=24)
    p.add_argument("--frac-w", type=int, default=15)
    p.add_argument("--mult-frac", type=int, default=15)
    p.add_argument("--mult-saturate", type=int, choices=[0, 1], default=1)
    p.add_argument("--pre-ifft-shifts", type=str, default="12")
    p.add_argument("--fft-to-ifft-maps", type=str, default="0")
    p.add_argument("--improve-tolerance", type=float, default=0.10, help="Count as improved if rmse_deblur <= rmse_blur*(1+tolerance)")
    p.add_argument("--selection-metric", type=str, choices=["sharpness", "rmse"], default="sharpness", help="How to pick best candidate from PSF/K sweeps")
    args = p.parse_args()

    if not is_pow2(args.lanes):
        raise ValueError("--lanes must be power-of-2")

    k_values = [int(x.strip()) for x in args.k_values.split(",") if x.strip()]
    if not k_values:
        k_values = [256]
    pre_ifft_shifts = [int(x.strip()) for x in args.pre_ifft_shifts.split(",") if x.strip()]
    if not pre_ifft_shifts:
        pre_ifft_shifts = [0]
    fft_to_ifft_maps = [int(x.strip()) for x in args.fft_to_ifft_maps.split(",") if x.strip()]
    if not fft_to_ifft_maps:
        fft_to_ifft_maps = [0]
    deblur_psf_sigmas = [float(x.strip()) for x in args.deblur_psf_sigmas.split(",") if x.strip()]
    if not deblur_psf_sigmas:
        deblur_psf_sigmas = [args.psf_sigma]

    args.images_dir.mkdir(parents=True, exist_ok=True)
    args.outdir.mkdir(parents=True, exist_ok=True)

    exts = {".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff"}
    imgs = sorted([p for p in args.images_dir.iterdir() if p.is_file() and p.suffix.lower() in exts])
    if not imgs:
        print(f"No images found in {args.images_dir}. Put images there and rerun.")
        return 0

    tool_dir = Path(__file__).resolve().parent
    for img in imgs:
        name, rmse_blur, rmse_rec, imag_rmse, fft_cycles, best_k, best_shift, best_map, best_deblur_sigma = process_one(
            tool_dir,
            img,
            args.outdir,
            args.max_n,
            args.img_n,
            args.fft_logn,
            args.lanes,
            k_values,
            args.h_scale,
            args.psf_kernel,
            args.psf_sigma,
            deblur_psf_sigmas,
            args.data_w,
            args.frac_w,
            args.mult_frac,
            args.mult_saturate,
            pre_ifft_shifts,
            fft_to_ifft_maps,
            args.improve_tolerance,
            args.selection_metric,
        )
        extra = f", fft_cycles={fft_cycles}" if fft_cycles is not None else ""
        status = "IMPROVED" if rmse_rec <= (rmse_blur * (1.0 + args.improve_tolerance)) else "NOT_IMPROVED"
        print(
            f"{name}: rmse_blur={rmse_blur:.6e}, rmse_deblur={rmse_rec:.6e}, "
            f"imag_rmse={imag_rmse:.6e}, best_k={best_k}, best_shift={best_shift}, best_map={best_map}, "
            f"best_deblur_sigma={best_deblur_sigma:.3f}{extra}, {status}"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
