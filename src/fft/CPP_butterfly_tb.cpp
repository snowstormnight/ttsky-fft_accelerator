// Run command:
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/fft
//   verilator -Wall --cc SV_butterfly.sv \
//     --top-module butterfly --exe /tmp/ttsky_fft_ws/src/fft/CPP_butterfly_tb.cpp --build -j 0 \
//     -CFLAGS "-std=c++17" -Mdir /tmp/ttsky_build/fft_butterfly
//   /tmp/ttsky_build/fft_butterfly/Vbutterfly

#include "Vbutterfly.h"
#include "verilated.h"

#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

struct Cx16 {
  int16_t re;
  int16_t im;
};

struct CaseSpec {
  std::string name;
  Cx16 a;
  Cx16 b;
  Cx16 w;
};

int16_t sat16(int32_t x) {
  if (x > 32767) return 32767;
  if (x < -32768) return -32768;
  return static_cast<int16_t>(x);
}

void tick(Vbutterfly& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vbutterfly& dut) {
  dut.rst_n = 0;
  dut.start = 0;
  dut.a_re = 0;
  dut.a_im = 0;
  dut.b_re = 0;
  dut.b_im = 0;
  dut.w_re = 0;
  dut.w_im = 0;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

Cx16 golden_sum(const Cx16& a, const Cx16& b) {
  Cx16 y{};
  y.re = static_cast<int16_t>((static_cast<int32_t>(a.re) + static_cast<int32_t>(b.re)) >> 1);
  y.im = static_cast<int16_t>((static_cast<int32_t>(a.im) + static_cast<int32_t>(b.im)) >> 1);
  return y;
}

Cx16 golden_diff_mul(const Cx16& a, const Cx16& b, const Cx16& w) {
  const int32_t dr = (static_cast<int32_t>(a.re) - static_cast<int32_t>(b.re)) >> 1;
  const int32_t di = (static_cast<int32_t>(a.im) - static_cast<int32_t>(b.im)) >> 1;
  const int64_t p1 = static_cast<int64_t>(dr) * static_cast<int64_t>(w.re);
  const int64_t p2 = static_cast<int64_t>(di) * static_cast<int64_t>(w.im);
  const int64_t p3 = static_cast<int64_t>(dr) * static_cast<int64_t>(w.im);
  const int64_t p4 = static_cast<int64_t>(di) * static_cast<int64_t>(w.re);
  Cx16 y{};
  y.re = sat16(static_cast<int32_t>((p1 - p2) >> 15));
  y.im = sat16(static_cast<int32_t>((p3 + p4) >> 15));
  return y;
}

bool run_case(
    Vbutterfly& dut,
    const CaseSpec& tc,
    bool verbose,
    int& done_cycle
) {
  const Cx16 exp_a = golden_sum(tc.a, tc.b);
  const Cx16 exp_b = golden_diff_mul(tc.a, tc.b, tc.w);

  if (verbose) {
    std::cout << "[CASE] " << tc.name
              << " a=(" << tc.a.re << "," << tc.a.im << ")"
              << " b=(" << tc.b.re << "," << tc.b.im << ")"
              << " w=(" << tc.w.re << "," << tc.w.im << ")\n";
  }

  dut.a_re = static_cast<uint16_t>(tc.a.re);
  dut.a_im = static_cast<uint16_t>(tc.a.im);
  dut.b_re = static_cast<uint16_t>(tc.b.re);
  dut.b_im = static_cast<uint16_t>(tc.b.im);
  dut.w_re = static_cast<uint16_t>(tc.w.re);
  dut.w_im = static_cast<uint16_t>(tc.w.im);
  dut.start = 1;
  tick(dut);
  dut.start = 0;

  bool saw_busy = false;
  done_cycle = -1;
  for (int cyc = 0; cyc < 20; ++cyc) {
    if (dut.busy) saw_busy = true;
    if (dut.done) {
      done_cycle = cyc;
      const int16_t got_a_re = static_cast<int16_t>(dut.out_a_re);
      const int16_t got_a_im = static_cast<int16_t>(dut.out_a_im);
      const int16_t got_b_re = static_cast<int16_t>(dut.out_b_re);
      const int16_t got_b_im = static_cast<int16_t>(dut.out_b_im);
      const bool ok = (got_a_re == exp_a.re) && (got_a_im == exp_a.im) &&
                      (got_b_re == exp_b.re) && (got_b_im == exp_b.im) && saw_busy;
      if (!ok) {
        std::cerr << "[FAIL] " << tc.name
                  << " got a=(" << got_a_re << "," << got_a_im << ")"
                  << " b=(" << got_b_re << "," << got_b_im << ")"
                  << " exp a=(" << exp_a.re << "," << exp_a.im << ")"
                  << " b=(" << exp_b.re << "," << exp_b.im << ")"
                  << " busy=" << saw_busy << " done_cycle=" << done_cycle << "\n";
      } else if (verbose) {
        std::cout << "[PASS] " << tc.name
                  << " out_a=(" << got_a_re << "," << got_a_im << ")"
                  << " out_b=(" << got_b_re << "," << got_b_im << ")"
                  << " done_cycle=" << done_cycle << "\n";
      }
      return ok;
    }
    tick(dut);
  }

  std::cerr << "[FAIL] " << tc.name << " timed out waiting for done pulse\n";
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Vbutterfly dut;

  // 1) Start.
  std::cout << "[INFO] butterfly bench start\n";
  reset_dut(dut);
  bool ok = true;

  // 2) Deterministic tests (expanded set).
  std::vector<CaseSpec> fixed_tests{
      {"identity_twiddle", {12000, -3000}, {-2000, 5000}, {32767, 0}},
      {"j_twiddle", {9000, 4000}, {-7000, 2000}, {0, 32767}},
      {"pi_over_4_twiddle", {16384, -8192}, {4096, 2048}, {23170, -23170}},
      {"neg_j_twiddle", {6000, -7000}, {-5000, 2000}, {0, -32768}},
      {"neg_identity_twiddle", {7000, 7000}, {-3000, -9000}, {-32768, 0}},
      {"zero_diff_path", {5000, -1234}, {5000, -1234}, {18205, -27246}},
      {"saturation_stress", {32767, 32767}, {-32768, -32768}, {32767, 32767}},
  };

  int done_cycle = -1;
  for (const auto& tc : fixed_tests) {
    ok &= run_case(dut, tc, true, done_cycle);
    if (!ok) break;
  }

  // 3) Random regression.
  std::mt19937 rng(20260222);
  std::uniform_int_distribution<int32_t> dist(-32768, 32767);
  const int random_count = 400;
  int random_pass = 0;
  for (int i = 0; i < random_count && ok; ++i) {
    CaseSpec tc{
        "random_" + std::to_string(i),
        {static_cast<int16_t>(dist(rng)), static_cast<int16_t>(dist(rng))},
        {static_cast<int16_t>(dist(rng)), static_cast<int16_t>(dist(rng))},
        {static_cast<int16_t>(dist(rng)), static_cast<int16_t>(dist(rng))},
    };
    const bool verbose = (i < 5) || ((i + 1) % 100 == 0);
    ok &= run_case(dut, tc, verbose, done_cycle);
    if (ok) ++random_pass;
  }
  std::cout << "[INFO] random_pass=" << random_pass << "/" << random_count << "\n";

  // 4) Summary.
  if (!ok) {
    std::cerr << "[FAIL] butterfly bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] butterfly bench complete\n";
  dut.final();
  return 0;
}

