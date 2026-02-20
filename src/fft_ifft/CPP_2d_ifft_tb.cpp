#include "Vifft_core.h"
#include "verilated.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
#ifndef IFFT_N
#define IFFT_N 65536
#endif
#ifndef IFFT_DATA_W
#define IFFT_DATA_W 16
#endif

constexpr int N = IFFT_N;        // total complex samples per frame
constexpr int DATA_W = IFFT_DATA_W;

struct Cx {
  int32_t re;
  int32_t im;
};

uint32_t pack_s(int32_t x) {
  if (DATA_W >= 32) return static_cast<uint32_t>(x);
  const uint64_t mask = (1ULL << DATA_W) - 1ULL;
  return static_cast<uint32_t>(static_cast<uint64_t>(x) & mask);
}

int32_t sx_u(uint32_t x) {
  if (DATA_W >= 32) return static_cast<int32_t>(x);
  const uint32_t mask = (1u << DATA_W) - 1u;
  x &= mask;
  const uint32_t sign = 1u << (DATA_W - 1);
  return static_cast<int32_t>((x ^ sign) - sign);
}

void tick(Vifft_core& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vifft_core& dut) {
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

bool parse_complex_frame(const std::string& in_path, std::vector<Cx>& frame) {
  std::ifstream fin(in_path);
  if (!fin) return false;

  int idx = 0;
  long long re = 0;
  long long im = 0;
  while (fin >> re >> im) {
    if (idx >= N) return false;
    frame[idx].re = static_cast<int32_t>(re);
    frame[idx].im = static_cast<int32_t>(im);
    ++idx;
  }
  return idx == N;
}

bool write_complex_frame(const std::string& out_path, const std::vector<Cx>& frame) {
  std::ofstream fout(out_path);
  if (!fout) return false;
  for (int i = 0; i < N; ++i) {
    fout << frame[i].re << " " << frame[i].im << "\n";
  }
  return true;
}

bool run_ifft_2d_frame(Vifft_core& dut, const std::vector<Cx>& in_frame, std::vector<Cx>& out_frame) {
  if (static_cast<int>(in_frame.size()) != N) return false;

  out_frame.assign(N, {0, 0});
  int in_idx = 0;
  int out_idx = 0;

  const int logn = []() {
    int l = 0;
    while ((1 << l) < N) ++l;
    return l;
  }();

  // Conservative timeout for one full 2D frame through ifft_core.
  const long long max_cycles = 200LL * N * logn + 50000LL;

  for (long long cyc = 0; cyc < max_cycles && out_idx < N; ++cyc) {
    if (in_idx < N) {
      dut.in_valid = 1;
      dut.in_re = pack_s(in_frame[in_idx].re);
      dut.in_im = pack_s(in_frame[in_idx].im);
    } else {
      dut.in_valid = 0;
      dut.in_re = 0;
      dut.in_im = 0;
    }
    dut.out_ready = 1;

    dut.eval();
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int32_t out_re = sx_u(static_cast<uint32_t>(dut.out_re));
    const int32_t out_im = sx_u(static_cast<uint32_t>(dut.out_im));

    tick(dut);

    if (take_in) ++in_idx;
    if (take_out) {
      out_frame[out_idx].re = out_re;
      out_frame[out_idx].im = out_im;
      ++out_idx;
    }
  }

  return out_idx == N;
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " <ifft_input_nat.txt> <ifft_output_hw.txt>\n";
    return 2;
  }

  std::vector<Cx> in_frame(N), out_frame;
  if (!parse_complex_frame(argv[1], in_frame)) {
    std::cerr << "failed to parse input frame, expected " << N << " complex entries\n";
    return 2;
  }

  Verilated::commandArgs(argc, argv);
  Vifft_core dut;
  reset_dut(dut);

  if (!run_ifft_2d_frame(dut, in_frame, out_frame)) {
    std::cerr << "2D IFFT simulation timeout before full frame output\n";
    return 1;
  }

  if (!write_complex_frame(argv[2], out_frame)) {
    std::cerr << "failed to write output file: " << argv[2] << "\n";
    return 2;
  }

  std::cout << "IFFT_CFG N=" << N << " DATA_W=" << DATA_W << "\n";
  dut.final();
  return 0;
}
