#include "verilated.h"
#include "Vdeblur.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

#ifndef DEBLUR_N
#define DEBLUR_N 256
#endif
#ifndef DEBLUR_DATA_W
#define DEBLUR_DATA_W 24
#endif

constexpr int kN = DEBLUR_N;
constexpr int kTot = kN * kN;
constexpr int kDataW = DEBLUR_DATA_W;

struct Cx {
  int32_t re;
  int32_t im;
};

bool parse_real_file(const std::string& path, std::vector<int32_t>& v, int n) {
  std::ifstream fin(path);
  if (!fin) return false;
  v.assign(n, 0);
  long long x = 0;
  int i = 0;
  while (fin >> x) {
    if (i >= n) return false;
    v[i++] = static_cast<int32_t>(x);
  }
  return i == n;
}

bool parse_complex_file(const std::string& path, std::vector<Cx>& v, int n) {
  std::ifstream fin(path);
  if (!fin) return false;
  v.assign(n, {0, 0});
  long long re = 0, im = 0;
  int i = 0;
  while (fin >> re >> im) {
    if (i >= n) return false;
    v[i].re = static_cast<int32_t>(re);
    v[i].im = static_cast<int32_t>(im);
    ++i;
  }
  return i == n;
}

bool write_complex_file(const std::string& path, const std::vector<Cx>& v) {
  std::ofstream fout(path);
  if (!fout) return false;
  for (const auto& x : v) fout << x.re << " " << x.im << "\n";
  return true;
}

uint32_t pack_s(int32_t x, int w) {
  if (w >= 32) return static_cast<uint32_t>(x);
  const uint64_t mask = (1ULL << w) - 1ULL;
  return static_cast<uint32_t>(static_cast<uint64_t>(x) & mask);
}

int32_t sx_u(uint32_t x, int w) {
  if (w >= 32) return static_cast<int32_t>(x);
  const uint32_t mask = (1u << w) - 1u;
  x &= mask;
  const uint32_t sign = 1u << (w - 1);
  return static_cast<int32_t>((x ^ sign) - sign);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "usage: " << argv[0]
              << " <in_blur_real.txt> <h_in_complex.txt> <out_complex.txt> <k>\n";
    return 2;
  }

  std::vector<int32_t> img;
  std::vector<Cx> h;
  if (!parse_real_file(argv[1], img, kTot)) {
    std::cerr << "failed to parse image input\n";
    return 2;
  }
  if (!parse_complex_file(argv[2], h, kTot)) {
    std::cerr << "failed to parse H input\n";
    return 2;
  }

  long long k_ll = 0;
  try {
    k_ll = std::stoll(argv[4]);
  } catch (...) {
    std::cerr << "invalid k\n";
    return 2;
  }
  if (k_ll < 0) k_ll = 0;

  Vdeblur dut;
  dut.clk = 0;
  dut.rst_n = 0;
  dut.img_valid = 0;
  dut.img_re = 0;
  dut.h_valid = 0;
  dut.h_re = 0;
  dut.h_im = 0;
  dut.k_cfg = pack_s(static_cast<int32_t>(k_ll), kDataW);
  dut.out_ready = 1;
  dut.eval();

  for (int i = 0; i < 4; ++i) {
    dut.clk = 0; dut.eval();
    dut.clk = 1; dut.eval();
  }
  dut.rst_n = 1;

  std::vector<Cx> out(kTot, {0, 0});
  int h_idx = 0;
  int img_idx = 0;
  int out_idx = 0;
  unsigned long long perf_cycles = 0;

  const long long max_cycles = 700LL * kTot + 100000LL;

  for (long long cyc = 0; cyc < max_cycles && out_idx < kTot; ++cyc) {
    dut.k_cfg = pack_s(static_cast<int32_t>(k_ll), kDataW);

    if (h_idx < kTot) {
      dut.h_valid = 1;
      dut.h_re = pack_s(h[h_idx].re, kDataW);
      dut.h_im = pack_s(h[h_idx].im, kDataW);
    } else {
      dut.h_valid = 0;
      dut.h_re = 0;
      dut.h_im = 0;
    }

    if (img_idx < kTot) {
      dut.img_valid = 1;
      dut.img_re = pack_s(img[img_idx], 16);
    } else {
      dut.img_valid = 0;
      dut.img_re = 0;
    }

    dut.out_ready = 1;

    dut.eval();

    const bool take_h = dut.h_valid && dut.h_ready;
    const bool take_img = dut.img_valid && dut.img_ready;
    const bool take_out = dut.out_valid && dut.out_ready;

    if (take_h) ++h_idx;
    if (take_img) ++img_idx;
    if (take_out) {
      out[out_idx].re = sx_u(static_cast<uint32_t>(dut.out_re), kDataW);
      out[out_idx].im = sx_u(static_cast<uint32_t>(dut.out_im), kDataW);
      ++out_idx;
    }

    if (dut.fft_perf_done) {
      perf_cycles = static_cast<unsigned long long>(dut.fft_perf_cycles);
    }

    dut.clk = 0; dut.eval();
    dut.clk = 1; dut.eval();

    if (dut.fft_perf_done) {
      perf_cycles = static_cast<unsigned long long>(dut.fft_perf_cycles);
    }
  }

  if (out_idx != kTot) {
    std::cerr << "deblur timeout (h=" << h_idx << ", img=" << img_idx << ", out=" << out_idx << ")\n";
    return 1;
  }

  if (!write_complex_file(argv[3], out)) {
    std::cerr << "failed to write output\n";
    return 2;
  }

  std::cout << "PERF_CYCLES " << perf_cycles << "\n";

  dut.final();
  return 0;
}
