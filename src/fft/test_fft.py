#!/usr/bin/env python3
import argparse
import subprocess
import tempfile
from pathlib import Path

try:
    import numpy as np
except Exception as exc:
    raise SystemExit("numpy is required. Install dependencies from src/fft/requirements.txt") from exc

try:
    from PIL import Image
except Exception:
    Image = None

try:
    import cv2  # type: ignore
except Exception:
    cv2 = None

N = 16
LOGN = 4
Q = 32768.0
ROOT = Path(__file__).resolve().parent
BUILD = ROOT / "build"
BIN = BUILD / "Vfft1d_core"


def build_model(rebuild: bool) -> None:
    if BIN.exists() and not rebuild:
        return
    BUILD.mkdir(parents=True, exist_ok=True)
    cmd = [
        "verilator",
        "-Wall",
        "-Wno-fatal",
        "--cc",
        "--exe",
        "--build",
        "-O2",
        "--top-module",
        "fft1d_core",
        "-Mdir",
        str(BUILD),
        "-CFLAGS",
        "-std=c++17",
        str(ROOT / "fft1d_core.sv"),
        str(ROOT / "butterfly.sv"),
        str(ROOT / "twiddle_rom.sv"),
        str(ROOT / "fft_controller.sv"),
        str(ROOT / "sim_main.cpp"),
    ]
    subprocess.run(cmd, check=True, cwd=ROOT)


def bitrev_indices() -> np.ndarray:
    return np.array([int(f"{i:0{LOGN}b}"[::-1], 2) for i in range(N)], dtype=np.int32)


def run_fft1d(vec: np.ndarray) -> np.ndarray:
    re_q = np.clip(np.round(np.real(vec) * Q), -32768, 32767).astype(np.int16)
    im_q = np.clip(np.round(np.imag(vec) * Q), -32768, 32767).astype(np.int16)
    with tempfile.TemporaryDirectory(prefix="fft_blk_") as td:
        td_path = Path(td)
        in_path = td_path / "in.txt"
        out_path = td_path / "out.txt"
        with in_path.open("w", encoding="utf-8") as f:
            for r, i in zip(re_q, im_q):
                f.write(f"{int(r)} {int(i)}\n")
        subprocess.run([str(BIN), str(in_path), str(out_path)], check=True, cwd=ROOT)
        out = np.loadtxt(out_path, dtype=np.int32)
    return (out[:, 0].astype(np.float64) + 1j * out[:, 1].astype(np.float64)) / Q


def run_fft2d_hw(tile: np.ndarray) -> np.ndarray:
    row_fft = np.zeros((N, N), dtype=np.complex128)
    for r in range(N):
        row_fft[r, :] = run_fft1d(tile[r, :].astype(np.complex128))
    out = np.zeros((N, N), dtype=np.complex128)
    for c in range(N):
        out[:, c] = run_fft1d(row_fft[:, c])
    return out


def reference_fft2d(tile: np.ndarray) -> np.ndarray:
    idx = bitrev_indices()
    ref = np.fft.fft2(tile.astype(np.complex128)) / (N * N)
    return ref[np.ix_(idx, idx)]


def check_case(name: str, tile: np.ndarray, max_mae: float, max_abs: float) -> bool:
    hw = run_fft2d_hw(tile)
    ref = reference_fft2d(tile)
    err = np.abs(hw - ref)
    mae = float(np.mean(err))
    mxe = float(np.max(err))
    print(f"{name:12s} MAE={mae:.6f} MAX={mxe:.6f}")
    return mae <= max_mae and mxe <= max_abs


def generated_tiles(seed: int) -> dict[str, np.ndarray]:
    rng = np.random.default_rng(seed)
    impulse = np.zeros((N, N), dtype=np.float64)
    impulse[0, 0] = 0.85
    const = np.full((N, N), 0.25, dtype=np.float64)
    rand = rng.uniform(-0.6, 0.6, size=(N, N))
    x = np.arange(N, dtype=np.float64)
    y = np.arange(N, dtype=np.float64)
    xx, yy = np.meshgrid(x, y)
    sinus = 0.45 * np.sin(2 * np.pi * 3 * xx / N) + 0.35 * np.cos(2 * np.pi * 2 * yy / N)
    sinus = np.clip(sinus, -0.95, 0.95)
    return {"impulse": impulse, "constant": const, "random": rand, "sinusoidal": sinus}


def load_image_tile(path: Path) -> np.ndarray:
    if Image is not None:
        img = Image.open(path).convert("L").resize((N, N))
        arr = np.asarray(img, dtype=np.float64)
    elif cv2 is not None:
        arr = cv2.imread(str(path), cv2.IMREAD_GRAYSCALE)
        if arr is None:
            raise RuntimeError(f"failed to read image: {path}")
        arr = cv2.resize(arr, (N, N), interpolation=cv2.INTER_AREA).astype(np.float64)
    else:
        raise RuntimeError("install pillow or opencv-python for image-mode test")
    return np.clip((arr - 128.0) / 128.0, -1.0, 0.999969)


def main() -> int:
    p = argparse.ArgumentParser(description="Verilator + Python checks for fft1d_core")
    p.add_argument("--rebuild", action="store_true", help="force verilator rebuild")
    p.add_argument("--seed", type=int, default=7, help="seed for random matrix")
    p.add_argument("--image", type=Path, default=None, help="optional grayscale image path")
    p.add_argument("--max-mae", type=float, default=0.03, help="pass threshold for MAE")
    p.add_argument("--max-abs", type=float, default=0.12, help="pass threshold for max abs error")
    args = p.parse_args()

    build_model(args.rebuild)
    all_ok = True
    for name, tile in generated_tiles(args.seed).items():
        all_ok &= check_case(name, tile, args.max_mae, args.max_abs)

    if args.image is not None:
        all_ok &= check_case("image", load_image_tile(args.image), args.max_mae, args.max_abs)

    print("PASS" if all_ok else "FAIL")
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
