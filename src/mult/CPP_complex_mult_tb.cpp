// Run command:
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/mult
//   verilator -Wall --cc complex_mult.sv \
//     --top-module complex_mult --exe /tmp/ttsky_fft_ws/src/mult/CPP_complex_mult_tb.cpp \
//     --build -j 0 -CFLAGS "-std=c++17" -Mdir /tmp/ttsky_build/complex_mult
//   /tmp/ttsky_build/complex_mult/Vcomplex_mult

#include "Vcomplex_mult.h"
#include "verilated.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int kInW = 16;
constexpr int kOutW = 16;
constexpr int kFrac = 8;
constexpr int kSumW = kInW + 1;
constexpr int kMulW = 2 * kInW;

struct Cx {
  int32_t re;
  int32_t im;
};

struct CaseSpec {
  std::string name;
  Cx a;
  Cx b;
};

int32_t sx(uint32_t x, int w) {
  if (w >= 32) return static_cast<int32_t>(x);
  const uint32_t sign = 1u << (w - 1);
  const uint32_t mask = (1u << w) - 1u;
  x &= mask;
  return static_cast<int32_t>((x ^ sign) - sign);
}

uint32_t pack(int32_t x, int w) {
  if (w >= 32) return static_cast<uint32_t>(x);
  const uint32_t mask = (1u << w) - 1u;
  return static_cast<uint32_t>(x) & mask;
}

int64_t sx_w(int64_t x, int w) {
  if (w >= 63) return x;
  const int64_t sign = int64_t(1) << (w - 1);
  const int64_t mask = (int64_t(1) << w) - 1;
  x &= mask;
  return (x ^ sign) - sign;
}

void tick(Vcomplex_mult& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vcomplex_mult& dut) {
  dut.rst_n = 0;
  dut.in_valid = 0;
  dut.out_ready = 1;
  dut.a_re = 0;
  dut.a_im = 0;
  dut.b_re = 0;
  dut.b_im = 0;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

Cx golden(const Cx& a, const Cx& b) {
  // RTL defaults: USE_GAUSS_3M=1, ROUND=1, SATURATE=0.
  const int64_t ar_p_ai = sx_w(int64_t(a.re) + int64_t(a.im), kSumW);
  const int64_t br_p_bi = sx_w(int64_t(b.re) + int64_t(b.im), kSumW);

  const int64_t k1 = sx_w(int64_t(a.re) * int64_t(b.re), kMulW);
  const int64_t k2 = sx_w(int64_t(a.im) * int64_t(b.im), kMulW);
  const int64_t k3 = sx_w(ar_p_ai * br_p_bi, kMulW);

  int64_t real_s = k1 - k2;
  int64_t imag_s = k3 - k1 - k2;

  if (kFrac > 0) {
    const int64_t bias = int64_t(1) << (kFrac - 1);
    real_s = (real_s >= 0) ? ((real_s + bias) >> kFrac) : ((real_s - bias) >> kFrac);
    imag_s = (imag_s >= 0) ? ((imag_s + bias) >> kFrac) : ((imag_s - bias) >> kFrac);
  }

  return Cx{
      sx(pack(static_cast<int32_t>(real_s), kOutW), kOutW),
      sx(pack(static_cast<int32_t>(imag_s), kOutW), kOutW),
  };
}

bool run_case(
    Vcomplex_mult& dut,
    const CaseSpec& tc,
    bool verbose
) {
  const Cx exp = golden(tc.a, tc.b);
  if (verbose) {
    std::cout << "[CASE] " << tc.name
              << " a=(" << tc.a.re << "," << tc.a.im << ")"
              << " b=(" << tc.b.re << "," << tc.b.im << ")"
              << " exp=(" << exp.re << "," << exp.im << ")\n";
  }

  bool sent = false;
  const long long max_cycles = 300;
  for (long long cyc = 0; cyc < max_cycles; ++cyc) {
    if (!sent) {
      dut.in_valid = 1;
      dut.a_re = pack(tc.a.re, kInW);
      dut.a_im = pack(tc.a.im, kInW);
      dut.b_re = pack(tc.b.re, kInW);
      dut.b_im = pack(tc.b.im, kInW);
    } else {
      dut.in_valid = 0;
      dut.a_re = 0;
      dut.a_im = 0;
      dut.b_re = 0;
      dut.b_im = 0;
    }
    dut.out_ready = 1;
    dut.eval();

    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int32_t got_re = sx(static_cast<uint32_t>(dut.y_re), kOutW);
    const int32_t got_im = sx(static_cast<uint32_t>(dut.y_im), kOutW);
    tick(dut);

    if (take_in) sent = true;
    if (take_out) {
      const bool ok = (got_re == exp.re) && (got_im == exp.im);
      if (!ok) {
        std::cerr << "[FAIL] " << tc.name
                  << " got=(" << got_re << "," << got_im << ")"
                  << " exp=(" << exp.re << "," << exp.im << ")\n";
      } else if (verbose) {
        std::cout << "[PASS] " << tc.name
                  << " got=(" << got_re << "," << got_im << ")\n";
      }
      return ok;
    }
  }
  std::cerr << "[FAIL] " << tc.name << " timed out waiting for output\n";
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Vcomplex_mult dut;
  bool ok = true;

  // 1) Start.
  std::cout << "[INFO] complex_mult bench start\n";
  reset_dut(dut);

  // 2) Deterministic vectors (expanded).
  std::vector<CaseSpec> fixed_tests{
      {"zero_zero", {0, 0}, {0, 0}},
      {"real_x_real", {256, 0}, {128, 0}},
      {"real_x_imag", {256, 0}, {0, 128}},
      {"imag_x_imag", {0, 256}, {0, 256}},
      {"conjugate_pair", {256, 256}, {256, -256}},
      {"mixed_signs", {128, -64}, {-96, 32}},
      {"scaled_mixed", {-512, 128}, {64, -32}},
      {"near_limits", {30000, -28000}, {29000, 27000}},
  };
  for (const auto& tc : fixed_tests) {
    ok &= run_case(dut, tc, true);
    if (!ok) break;
  }

  // 3) Random regression.
  std::mt19937 rng(20260222);
  std::uniform_int_distribution<int32_t> dist(-32768, 32767);
  const int random_count = 2500;
  int random_pass = 0;
  for (int i = 0; i < random_count && ok; ++i) {
    CaseSpec tc{
        "random_" + std::to_string(i),
        {dist(rng), dist(rng)},
        {dist(rng), dist(rng)},
    };
    const bool verbose = (i < 5) || ((i + 1) % 500 == 0);
    ok &= run_case(dut, tc, verbose);
    if (ok) ++random_pass;
  }
  std::cout << "[INFO] random_pass=" << random_pass << "/" << random_count << "\n";

  // 4) Summary.
  if (!ok) {
    std::cerr << "[FAIL] complex_mult bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] complex_mult bench complete\n";
  dut.final();
  return 0;
}

