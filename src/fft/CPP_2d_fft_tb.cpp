#include "Vfft2d_core.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

namespace {
constexpr int N = 32;
constexpr int TILE = N * N;

struct C16 {
  int16_t re;
  int16_t im;
};

void tick(Vfft2d_core& dut) {
  // One full clock cycle.
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vfft2d_core& dut) {
  // Active-low synchronous-style reset sequence for a few cycles.
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

bool parse_txt_tile(const std::string& in_path, std::array<C16, TILE>& tile) {
  std::ifstream fin(in_path);
  if (!fin) return false;
  int idx = 0;
  int v = 0;
  while (fin >> v) {
    if (idx >= TILE) return false;
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

  std::array<C16, TILE> input_tile{};
  if (!parse_txt_tile(argv[1], input_tile)) {
    std::cerr << "failed to parse input tile, expected 1024 integers (one per line)\n";
    return 2;
  }

  Verilated::commandArgs(argc, argv);
  Vfft2d_core dut;
  reset_dut(dut);

  int in_idx = 0;
  int out_idx = 0;
  std::array<C16, TILE> out_tile{};

  for (int cyc = 0; cyc < 2000000 && out_idx < TILE; ++cyc) {
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

    dut.eval();
    // Sample handshakes before toggling to next cycle.
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int16_t out_r = static_cast<int16_t>(dut.out_re);
    const int16_t out_i = static_cast<int16_t>(dut.out_im);
    tick(dut);

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

  // Output format: one complex bin per line: "<re> <im>".
  std::ofstream fout(argv[2]);
  if (!fout) {
    std::cerr << "failed to open output file: " << argv[2] << "\n";
    return 2;
  }
  for (int i = 0; i < TILE; ++i) fout << out_tile[i].re << " " << out_tile[i].im << "\n";

  dut.final();
  return 0;
}
