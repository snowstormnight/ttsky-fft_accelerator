#!/usr/bin/env python3
from __future__ import annotations

import argparse
import math
import subprocess
from pathlib import Path

import numpy as np

Q = 32768.0


def load_mnist_test_images(local_npz: Path | None = None) -> np.ndarray:
    if local_npz is not None and local_npz.exists():
        d = np.load(local_npz)
        if "x_test" not in d:
            raise RuntimeError(f"{local_npz} does not contain x_test")
        return d["x_test"].astype(np.uint8)
    # Primary path requested: tensorflow.keras.datasets
    try:
        from tensorflow.keras.datasets import mnist  # type: ignore
        (_, _), (x_test, _) = mnist.load_data()
        return x_test.astype(np.uint8)
    except Exception:
        pass
    # Equivalent fallback: keras.datasets
    try:
        from keras.datasets import mnist  # type: ignore
        (_, _), (x_test, _) = mnist.load_data()
        return x_test.astype(np.uint8)
    except Exception as exc:
        raise RuntimeError("MNIST loader unavailable. Install tensorflow or keras.") from exc


def write_png(path: Path, img_u8: np.ndarray) -> None:
    try:
        import cv2  # type: ignore
        cv2.imwrite(str(path), img_u8)
    except Exception:
        try:
            from PIL import Image  # type: ignore
            Image.fromarray(img_u8).save(path)
        except Exception as exc:
            raise RuntimeError("Need cv2 or pillow to save PNG images.") from exc


def bitrev_indices(logn: int) -> np.ndarray:
    return np.array([int(f"{i:0{logn}b}"[::-1], 2) for i in range(1 << logn)], dtype=np.int32)


def bitrev2d_to_natural(x: np.ndarray) -> np.ndarray:
    n = x.shape[0]
    if (n & (n - 1)) != 0:
        raise ValueError("bit-reversal requires power-of-2 dimension.")
    idx = bitrev_indices(int(math.log2(n)))
    return x[np.ix_(idx, idx)]


def u8_to_q15(img_u8: np.ndarray) -> np.ndarray:
    x = np.clip((img_u8.astype(np.float64) - 128.0) / 128.0, -1.0, 0.999969)
    return np.clip(np.round(x * Q), -32768, 32767).astype(np.int16)


def run_hw_fft(fft_bin: Path, in_txt: Path, out_txt: Path) -> None:
    subprocess.run([str(fft_bin), str(in_txt), str(out_txt)], check=True)


def parse_hw_fft(path: Path, n: int) -> np.ndarray:
    raw = np.loadtxt(path, dtype=np.int64)
    if raw.ndim == 1:
        raw = raw.reshape(1, -1)
    if raw.shape != (n * n, 2):
        raise ValueError(f"bad DUT output shape {raw.shape}, expected {(n*n, 2)}")
    return (raw[:, 0].astype(np.float64) + 1j * raw[:, 1].astype(np.float64)).reshape(n, n) / Q


def pad_q15_to_hw(q28: np.ndarray, hw_n: int) -> np.ndarray:
    q_hw = np.zeros((hw_n, hw_n), dtype=np.int16)
    q_hw[: q28.shape[0], : q28.shape[1]] = q28
    return q_hw


def main() -> int:
    p = argparse.ArgumentParser(description="MNIST hardware FFT -> NumPy IFFT verification (10 images).")
    p.add_argument("--fft-bin", type=Path, default=Path("fft_hw"), help="hardware fft binary path")
    p.add_argument("--outdir", type=Path, default=Path("mnist_fft_check"), help="output folder")
    p.add_argument("--num-images", type=int, default=10, help="number of test images")
    p.add_argument(
        "--order",
        choices=["natural", "bitrev2d"],
        default="bitrev2d",
        help="hardware output ordering",
    )
    p.add_argument("--tol-max", type=float, default=0.035, help="recommended max error tolerance (float domain)")
    p.add_argument("--tol-rmse", type=float, default=0.005, help="recommended RMSE tolerance (float domain)")
    p.add_argument("--hw-n", type=int, default=32, help="hardware FFT size (power of 2)")
    p.add_argument("--mnist-npz", type=Path, default=Path("data/mnist.npz"), help="local mnist.npz path")
    args = p.parse_args()

    if not args.fft_bin.exists():
        raise FileNotFoundError(f"hardware binary not found: {args.fft_bin}")

    x_test = load_mnist_test_images(args.mnist_npz)
    n_img = int(x_test.shape[1])
    if x_test.shape[1] != x_test.shape[2]:
        raise ValueError("MNIST images must be square.")
    if args.hw_n < n_img:
        raise ValueError("hw-n must be >= 28 for MNIST.")

    outdir = args.outdir
    outdir.mkdir(parents=True, exist_ok=True)

    ncases = min(args.num_images, x_test.shape[0])
    print(f"Running {ncases} MNIST images of size {n_img}x{n_img} on HW {args.hw_n}x{args.hw_n}")

    for i in range(ncases):
        case_dir = outdir / f"img_{i:02d}"
        case_dir.mkdir(parents=True, exist_ok=True)

        img_u8 = x_test[i]
        q = u8_to_q15(img_u8)
        q_hw = pad_q15_to_hw(q, args.hw_n)
        x_ref = q.astype(np.float64) / Q

        in_txt = case_dir / "input_tile.txt"
        out_txt = case_dir / "dut_fft_out.txt"
        np.savetxt(in_txt, q_hw.reshape(-1), fmt="%d")
        run_hw_fft(args.fft_bin, in_txt, out_txt)

        f_hw = parse_hw_fft(out_txt, args.hw_n)
        if args.order == "bitrev2d":
            f_hw = bitrev2d_to_natural(f_hw)

        # Hardware FFT scales by 1/N each pass -> 1/N^2 for 2D. Undo before IFFT.
        x_rec_full = np.real(np.fft.ifft2(f_hw * (args.hw_n * args.hw_n)))
        x_rec = x_rec_full[:n_img, :n_img]
        u8_rec = np.clip(np.round(x_rec * 128.0 + 128.0), 0, 255).astype(np.uint8)

        err = x_rec - x_ref
        max_abs_error = float(np.max(np.abs(err)))
        rmse = float(np.sqrt(np.mean(err * err)))
        ok = (max_abs_error <= args.tol_max) and (rmse <= args.tol_rmse)

        write_png(case_dir / "original.png", img_u8)
        write_png(case_dir / "reconstructed.png", u8_rec)
        diff_u8 = np.clip(np.abs(img_u8.astype(np.int16) - u8_rec.astype(np.int16)), 0, 255).astype(np.uint8)
        write_png(case_dir / "difference.png", diff_u8)

        with (case_dir / "metrics.txt").open("w", encoding="utf-8") as fp:
            fp.write(f"max_abs_error={max_abs_error:.8e}\n")
            fp.write(f"rmse={rmse:.8e}\n")
            fp.write(f"pass={int(ok)}\n")

        print(
            f"img_{i:02d}: max_abs_error={max_abs_error:.6e}, rmse={rmse:.6e}, "
            f"{'PASS' if ok else 'WARN'}"
        )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
