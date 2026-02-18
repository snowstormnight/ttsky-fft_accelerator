#include "Vfft2d_core.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
#ifndef FFT_N
#define FFT_N 32
#endif
#ifndef FFT_LOGN
#define FFT_LOGN 5
#endif
// N and TILE are compile-time constants passed from Python->Verilator build.
// Example for 256x256 run:
//   N=256, TILE=65536 samples.
constexpr int N = FFT_N;
constexpr int TILE = N * N;

// Complex sample container matching DUT I/O width.
struct C16 {
  int16_t re;
  int16_t im;
};

void tick(Vfft2d_core& dut) {
  // One full clock cycle:
  // 1) drive falling phase
  // 2) drive rising phase
  // DUT sequential logic updates on rising edge.
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vfft2d_core& dut) {
  // Reset protocol:
  // - hold rst_n low for a few cycles
  // - keep inputs inactive
  // - set out_ready=1 so downstream is always ready
  dut.rst_n = 0;
  dut.in_valid = 0;
  dut.in_re = 0;
  dut.in_im = 0;
  dut.out_ready = 1;
  dut.clk = 0;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

// Read one integer per line (row-major flattened image tile).
// Expected count: TILE = N*N samples.
bool parse_txt_tile(const std::string& in_path, std::vector<C16>& tile) {
  std::ifstream fin(in_path);
  if (!fin) return false;
  int idx = 0;
  int v = 0;
  while (fin >> v) {
    if (idx >= TILE) return false;
    // Input file stores real-valued image; imag is zero.
    tile[idx].re = static_cast<int16_t>(v);
    tile[idx].im = 0;
    ++idx;
  }
  return idx == TILE;
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <input_tile.txt> <fft_output_hw.txt>\n";
    return 2;
  }

  // input_tile: flattened row-major image in Q1.15 fixed-point.
  std::vector<C16> input_tile(TILE);
  if (!parse_txt_tile(argv[1], input_tile)) {
    std::cerr << "failed to parse input tile, expected " << TILE << " integers (one per line)\n";
    return 2;
  }

  Verilated::commandArgs(argc, argv);
  Vfft2d_core dut;
  reset_dut(dut);

  // Stream counters:
  // in_idx  = number of input samples accepted by DUT (valid&&ready).
  // out_idx = number of output bins accepted from DUT (valid&&ready).
  int in_idx = 0;
  int out_idx = 0;
  // out_tile stores raw DUT FFT output sequence before Python post-processing.
  std::vector<C16> out_tile(TILE);
  // block_cycles receives the hardware cycle counter exposed by DUT.
  unsigned long long block_cycles = 0;
  // got_perf indicates whether perf_done/perf_cycles was observed.
  bool got_perf = false;

  // Timeout scales with problem size.
  // Formula is intentionally conservative to avoid false timeout.
  const long long max_cycles = 50LL * TILE * FFT_LOGN + 20000LL;
  for (long long cyc = 0; cyc < max_cycles && out_idx < TILE; ++cyc) {
    // Drive input stream until all TILE samples are sent.
    if (in_idx < TILE) {
      dut.in_valid = 1;
      dut.in_re = static_cast<uint16_t>(input_tile[in_idx].re);
      dut.in_im = static_cast<uint16_t>(input_tile[in_idx].im);
    } else {
      dut.in_valid = 0;
      dut.in_re = 0;
      dut.in_im = 0;
    }
    dut.out_ready = 1;

    // Evaluate current combinational state, then handshake.
    dut.eval();
    // Sample handshakes before toggling to next cycle.
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    // Capture output values in the same cycle handshake is observed.
    const int16_t out_r = static_cast<int16_t>(dut.out_re);
    const int16_t out_i = static_cast<int16_t>(dut.out_im);
    // perf_done is a pulse from DUT when one full tile is complete.
    if (dut.perf_done) {
      block_cycles = static_cast<unsigned long long>(dut.perf_cycles);
      got_perf = true;
    }
    tick(dut);
    // perf_done is generated in sequential logic; sample again after the clock edge.
    if (dut.perf_done) {
      block_cycles = static_cast<unsigned long long>(dut.perf_cycles);
      got_perf = true;
    }

    if (take_in) ++in_idx;
    if (take_out) {
      out_tile[out_idx].re = out_r;
      out_tile[out_idx].im = out_i;
      ++out_idx;
    }
  }
  if (out_idx != TILE) {
    std::cerr << "simulation timeout before receiving full 2D FFT output\n";
    return 1;
  }

  // Output file format consumed by Python:
  // one complex sample per line: "<real_int> <imag_int>".
  std::ofstream fout(argv[2]);
  if (!fout) {
    std::cerr << "failed to open output file: " << argv[2] << "\n";
    return 2;
  }
  for (int i = 0; i < TILE; ++i) fout << out_tile[i].re << " " << out_tile[i].im << "\n";
  // Print performance counter in machine-parseable format.
  if (got_perf) {
    std::cout << "PERF_CYCLES " << block_cycles << "\n";
  }

  dut.final();
  return 0;
}
