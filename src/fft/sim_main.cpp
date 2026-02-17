#include "Vfft1d_core.h"
#include "verilated.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace {
constexpr int N = 16;

void tick(Vfft1d_core& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <input.txt> <output.txt>\n";
    return 2;
  }

  std::array<int16_t, N> in_re{};
  std::array<int16_t, N> in_im{};
  std::ifstream fin(argv[1]);
  if (!fin) {
    std::cerr << "failed to open input file: " << argv[1] << "\n";
    return 2;
  }
  for (int i = 0; i < N; ++i) {
    int r = 0, im = 0;
    if (!(fin >> r >> im)) {
      std::cerr << "input must contain exactly " << N << " lines of: <re> <im>\n";
      return 2;
    }
    in_re[i] = static_cast<int16_t>(r);
    in_im[i] = static_cast<int16_t>(im);
  }

  Verilated::commandArgs(argc, argv);
  Vfft1d_core dut;
  dut.rst_n = 0;
  dut.in_valid = 0;
  dut.in_re = 0;
  dut.in_im = 0;
  dut.out_ready = 1;
  dut.clk = 0;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;

  std::array<int16_t, N> out_re{};
  std::array<int16_t, N> out_im{};
  int in_idx = 0;
  int out_idx = 0;

  for (int cyc = 0; cyc < 20000 && out_idx < N; ++cyc) {
    if (in_idx < N) {
      dut.in_valid = 1;
      dut.in_re = static_cast<uint16_t>(in_re[in_idx]);
      dut.in_im = static_cast<uint16_t>(in_im[in_idx]);
    } else {
      dut.in_valid = 0;
      dut.in_re = 0;
      dut.in_im = 0;
    }
    dut.out_ready = 1;

    dut.eval();
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int16_t out_r = static_cast<int16_t>(dut.out_re);
    const int16_t out_i = static_cast<int16_t>(dut.out_im);
    tick(dut);

    if (take_in) ++in_idx;
    if (take_out) {
      out_re[out_idx] = out_r;
      out_im[out_idx] = out_i;
      ++out_idx;
    }
  }

  if (out_idx != N) {
    std::cerr << "simulation timeout before receiving " << N << " outputs\n";
    return 1;
  }

  std::ofstream fout(argv[2]);
  if (!fout) {
    std::cerr << "failed to open output file: " << argv[2] << "\n";
    return 2;
  }
  for (int i = 0; i < N; ++i) fout << out_re[i] << " " << out_im[i] << "\n";

  dut.final();
  return 0;
}
