#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
import subprocess
from pathlib import Path

import numpy as np

Q = 32768.0


def load_gray(path: Path) -> np.ndarray:
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
    try:
        import cv2  # type: ignore
        cv2.imwrite(str(path), img_u8)
    except Exception:
        from PIL import Image  # type: ignore
        Image.fromarray(img_u8).save(path)


def resize_square(img: np.ndarray, n: int) -> np.ndarray:
    try:
        import cv2  # type: ignore
        out = cv2.resize(img, (n, n), interpolation=cv2.INTER_AREA if n < img.shape[0] else cv2.INTER_LINEAR)
        return out.astype(np.uint8)
    except Exception:
        from PIL import Image  # type: ignore
        return np.asarray(Image.fromarray(img).resize((n, n), Image.BILINEAR), dtype=np.uint8)


def nearest_pow2(x: int) -> int:
    if x <= 1:
        return 2
    lo = 1 << int(math.floor(math.log2(x)))
    hi = 1 << int(math.ceil(math.log2(x)))
    return lo if (x - lo) <= (hi - x) else hi


def bitrev_indices(logn: int) -> np.ndarray:
    return np.array([int(f"{i:0{logn}b}"[::-1], 2) for i in range(1 << logn)], dtype=np.int32)


def bitrev2d_to_natural(x: np.ndarray) -> np.ndarray:
    n = x.shape[0]
    idx = bitrev_indices(int(math.log2(n)))
    return x[np.ix_(idx, idx)]


def u8_to_q15(img_u8: np.ndarray) -> np.ndarray:
    x = np.clip((img_u8.astype(np.float64) - 128.0) / 128.0, -1.0, 0.999969)
    return np.clip(np.round(x * Q), -32768, 32767).astype(np.int16)


def generate_twiddle_rom(path: Path, n: int) -> None:
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


def build_hw(repo_fft_dir: Path, n: int) -> Path:
    logn = int(math.log2(n))
    build_dir = repo_fft_dir / "build"
    bin_path = build_dir / "Vfft2d_core"
    meta_path = build_dir / "fft_n.txt"

    if bin_path.exists() and meta_path.exists():
        prev_n = meta_path.read_text(encoding="utf-8").strip()
        if prev_n == str(n):
            return bin_path

    if build_dir.exists():
        # Rebuild if target FFT size changed.
        import shutil
        shutil.rmtree(build_dir)

    build_dir.mkdir(parents=True, exist_ok=True)
    twiddle_auto = build_dir / "SV_twiddle_rom_auto.sv"
    generate_twiddle_rom(twiddle_auto, n)

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
        str(build_dir.name),
        f"-GN={n}",
        f"-GLOGN={logn}",
        "-CFLAGS",
        f"-std=c++17 -DFFT_N={n} -DFFT_LOGN={logn}",
        "SV_fft2d_core.sv",
        "SV_fft1d_core.sv",
        "SV_butterfly.sv",
        "SV_fft_controller.sv",
        str(twiddle_auto),
        "CPP_2d_fft_tb.cpp",
    ]
    subprocess.run(cmd, check=True, cwd=repo_fft_dir)
    if not bin_path.exists():
        raise RuntimeError(f"build failed: {bin_path} not produced")
    meta_path.write_text(f"{n}\n", encoding="utf-8")
    return bin_path


def parse_hw_fft(path: Path, n: int) -> np.ndarray:
    raw = np.loadtxt(path, dtype=np.int64)
    if raw.ndim == 1:
        raw = raw.reshape(1, -1)
    if raw.shape != (n * n, 2):
        raise ValueError(f"bad DUT output shape {raw.shape}, expected {(n*n, 2)}")
    return (raw[:, 0].astype(np.float64) + 1j * raw[:, 1].astype(np.float64)).reshape(n, n) / Q


def process_image(
    img_path: Path,
    outdir: Path,
    order: str,
    tol_max: float,
    tol_rmse: float,
    max_n: int,
) -> tuple[str, float, float, bool]:
    img = load_gray(img_path)
    n = nearest_pow2(max(img.shape[0], img.shape[1]))
    if n < 2:
        n = 2
    if n > max_n:
        n = max_n

    fft_dir = Path(__file__).resolve().parent
    fft_bin = build_hw(fft_dir, n)

    img_n = resize_square(img, n)
    q = u8_to_q15(img_n)
    x_ref = q.astype(np.float64) / Q

    case_dir = outdir / f"{img_path.stem}_N{n}"
    case_dir.mkdir(parents=True, exist_ok=True)
    in_txt = case_dir / "input_tile.txt"
    out_txt = case_dir / "dut_fft_out.txt"
    np.savetxt(in_txt, q.reshape(-1), fmt="%d")

    subprocess.run([str(fft_bin), str(in_txt), str(out_txt)], check=True)
    f_hw = parse_hw_fft(out_txt, n)
    if order == "bitrev2d":
        f_hw = bitrev2d_to_natural(f_hw)

    x_rec = np.real(np.fft.ifft2(f_hw * (n * n)))
    u8_rec = np.clip(np.round(x_rec * 128.0 + 128.0), 0, 255).astype(np.uint8)

    err = x_rec - x_ref
    max_err = float(np.max(np.abs(err)))
    rmse = float(np.sqrt(np.mean(err * err)))
    ok = (max_err <= tol_max) and (rmse <= tol_rmse)

    save_png(case_dir / "original.png", img_n)
    save_png(case_dir / "reconstructed.png", u8_rec)
    diff_u8 = np.clip(np.abs(img_n.astype(np.int16) - u8_rec.astype(np.int16)), 0, 255).astype(np.uint8)
    save_png(case_dir / "difference.png", diff_u8)
    with (case_dir / "metrics.txt").open("w", encoding="utf-8") as fp:
        fp.write(f"size={n}\nmax_abs_error={max_err:.8e}\nrmse={rmse:.8e}\npass={int(ok)}\n")

    return case_dir.name, max_err, rmse, ok


def main() -> int:
    p = argparse.ArgumentParser(description="Process grayscale images: HW FFT -> NumPy IFFT -> compare.")
    p.add_argument("--images-dir", type=Path, default=Path("image"), help="input grayscale image folder")
    p.add_argument("--outdir", type=Path, default=Path("results"), help="result root folder")
    p.add_argument("--order", choices=["natural", "bitrev2d"], default="bitrev2d")
    p.add_argument("--tol-max", type=float, default=0.035)
    p.add_argument("--tol-rmse", type=float, default=0.005)
    p.add_argument("--max-n", type=int, default=256, help="cap FFT size to avoid huge simulations")
    args = p.parse_args()

    args.images_dir.mkdir(parents=True, exist_ok=True)
    args.outdir.mkdir(parents=True, exist_ok=True)

    exts = {".png", ".jpg", ".jpeg", ".bmp", ".tif", ".tiff"}
    imgs = sorted([p for p in args.images_dir.iterdir() if p.is_file() and p.suffix.lower() in exts])
    if not imgs:
        print(f"No images found in {args.images_dir}. Put grayscale images there and rerun.")
        return 0

    for img in imgs:
        name, max_err, rmse, ok = process_image(
            img, args.outdir, args.order, args.tol_max, args.tol_rmse, args.max_n
        )
        print(f"{name}: max_abs_error={max_err:.6e}, rmse={rmse:.6e}, {'PASS' if ok else 'WARN'}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
