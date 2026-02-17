# FFT 1D Core (TinyTapeout-friendly)

- Architecture: radix-2 **DIF**, iterative in-place engine, one butterfly datapath reused for all stages.
- Arithmetic: signed Q1.15 complex, stage scaling (`>>> 1`) inside every butterfly.
- Ordering: input is natural order, output is **bit-reversed order**.
- Resources: one butterfly, one time-shared real multiplier, small sample RAM, twiddle ROM, FSM controller.

## Performance (N=16)

- Butterflies per FFT: `N/2 * log2(N) = 32`
- Butterfly schedule: `5` cycles per butterfly (4 mul cycles + 1 controller issue gap)
- Compute cycles: `160`
- Total cycles per block (load + compute + drain): `16 + 160 + 16 = 192`
- Throughput at 50 MHz: `~260k FFT/s` (assuming continuous valid/ready)

## Verilator + Python test

From `src/fft`:

```bash
python3 test_fft.py --rebuild
python3 test_fft.py --image /path/to/grayscale.png
```

Test method coverage:

- Generated 16x16 matrices: impulse, constant, random, sinusoidal
- Real grayscale image tile (resize/crop to 16x16)
- Flow: row FFT then column FFT, compared against NumPy FFT2 with scaling and bit-reversed indexing
