// Run command:
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/wiener_filter
//   verilator -Wall --cc wiener_filter.v \
//     --top-module FilterGen --exe /tmp/ttsky_fft_ws/src/wiener_filter/CPP_FilterGen_tb.cpp \
//     --build -j 0 -CFLAGS "-std=c++17" -Mdir /tmp/ttsky_build/filtergen
//   /tmp/ttsky_build/filtergen/VFilterGen

#include "VFilterGen.h"
#include "verilated.h"

#include <cstdint>
#include <deque>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

constexpr int kDataW = 24;
constexpr int kFracW = 15;
constexpr int kKShift = 0;

struct InSample {
  int32_t h_re;
  int32_t h_im;
  uint32_t k;
};

struct OutSample {
  int32_t g_re;
  int32_t g_im;
};

uint32_t pack_signed(int32_t x, int w) {
  if (w >= 32) return static_cast<uint32_t>(x);
  const uint32_t mask = (1u << w) - 1u;
  return static_cast<uint32_t>(x) & mask;
}

int32_t sx(uint32_t x, int w) {
  if (w >= 32) return static_cast<int32_t>(x);
  const uint32_t sign = 1u << (w - 1);
  const uint32_t mask = (1u << w) - 1u;
  x &= mask;
  return static_cast<int32_t>((x ^ sign) - sign);
}

int32_t sat_to_w(int64_t x, int w) {
  const int64_t maxv = (int64_t(1) << (w - 1)) - 1;
  const int64_t minv = -(int64_t(1) << (w - 1));
  if (x > maxv) return static_cast<int32_t>(maxv);
  if (x < minv) return static_cast<int32_t>(minv);
  return static_cast<int32_t>(x);
}

void tick(VFilterGen& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(VFilterGen& dut) {
  dut.rst_n = 0;
  dut.valid_in = 0;
  dut.H_real = 0;
  dut.H_imag = 0;
  dut.K = 0;
  dut.ready_out = 1;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

OutSample golden(const InSample& s) {
  const int64_t hr = s.h_re;
  const int64_t hi = s.h_im;
  const int64_t h_real_sq = hr * hr;
  const int64_t h_imag_sq = hi * hi;
  const int64_t mag2_q2f = h_real_sq + h_imag_sq;
  int64_t denom_q2f = mag2_q2f + (static_cast<int64_t>(s.k) << (kFracW + kKShift));
  if (denom_q2f <= 0) denom_q2f = 1;

  const int64_t num_real = hr << (2 * kFracW);
  const int64_t num_imag = (-hi) << (2 * kFracW);

  const int64_t q_real = num_real / denom_q2f;
  const int64_t q_imag = num_imag / denom_q2f;

  return OutSample{
      sat_to_w(q_real, kDataW),
      sat_to_w(q_imag, kDataW),
  };
}

bool run_stream_test(
    VFilterGen& dut,
    const std::vector<InSample>& in_vec,
    bool allow_stall,
    const std::string& tag,
    int preview_count
) {
  std::cout << "[CASE] " << tag
            << " vectors=" << in_vec.size()
            << " allow_stall=" << (allow_stall ? 1 : 0) << "\n";

  std::deque<OutSample> expect_q;
  std::mt19937 rng(20260222 + static_cast<uint32_t>(in_vec.size()));
  std::uniform_int_distribution<int> stall_dist(0, 9);
  size_t send_idx = 0;
  size_t recv_idx = 0;
  int stall_cycles = 0;
  int cycle_count = 0;

  const long long max_cycles = 200000;
  for (long long cyc = 0; cyc < max_cycles; ++cyc) {
    const bool can_send = send_idx < in_vec.size();
    dut.valid_in = can_send ? 1 : 0;
    if (can_send) {
      dut.H_real = pack_signed(in_vec[send_idx].h_re, kDataW);
      dut.H_imag = pack_signed(in_vec[send_idx].h_im, kDataW);
      dut.K = in_vec[send_idx].k & ((1u << kDataW) - 1u);
    } else {
      dut.H_real = 0;
      dut.H_imag = 0;
      dut.K = 0;
    }

    if (allow_stall) {
      dut.ready_out = (stall_dist(rng) < 8) ? 1 : 0;
      if (!dut.ready_out) ++stall_cycles;
    } else {
      dut.ready_out = 1;
    }

    dut.eval();
    const bool take_in = dut.valid_in && dut.ready_in;
    const bool take_out = dut.valid_out && dut.ready_out;
    const int32_t g_re = sx(static_cast<uint32_t>(dut.G_real), kDataW);
    const int32_t g_im = sx(static_cast<uint32_t>(dut.G_imag), kDataW);

    if (take_in) {
      expect_q.push_back(golden(in_vec[send_idx]));
      ++send_idx;
    }

    if (take_out) {
      if (expect_q.empty()) {
        std::cerr << "[FAIL] " << tag << " output with empty expected queue\n";
        return false;
      }
      const OutSample exp = expect_q.front();
      expect_q.pop_front();
      const bool match = (g_re == exp.g_re) && (g_im == exp.g_im);
      if (!match) {
        std::cerr << "[FAIL] " << tag
                  << " idx=" << recv_idx
                  << " got=(" << g_re << "," << g_im << ")"
                  << " exp=(" << exp.g_re << "," << exp.g_im << ")\n";
        return false;
      }
      if (static_cast<int>(recv_idx) < preview_count) {
        std::cout << "[INFO] " << tag
                  << " sample[" << recv_idx << "]"
                  << " in_h=(" << in_vec[recv_idx].h_re << "," << in_vec[recv_idx].h_im << ")"
                  << " k=" << in_vec[recv_idx].k
                  << " out=(" << g_re << "," << g_im << ")\n";
      }
      ++recv_idx;
    }

    tick(dut);
    ++cycle_count;

    if (recv_idx == in_vec.size() && send_idx == in_vec.size() && expect_q.empty()) {
      std::cout << "[PASS] " << tag
                << " cycles=" << cycle_count
                << " stalls=" << stall_cycles << "\n";
      return true;
    }
  }

  std::cerr << "[FAIL] " << tag << " timeout"
            << " sent=" << send_idx
            << " recv=" << recv_idx
            << " q=" << expect_q.size() << "\n";
  return false;
}

}  // namespace

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  VFilterGen dut;
  bool ok = true;

  // 1) Start.
  std::cout << "[INFO] FilterGen bench start\n";
  reset_dut(dut);

  // 2) Deterministic vectors (expanded set).
  const int32_t one_q = (1 << kFracW);
  std::vector<InSample> deterministic{
      {one_q, 0, 0},
      {0, one_q, 0},
      {one_q, one_q, 1u << (kFracW - 4)},
      {0, 0, 1u << (kFracW - 2)},
      {-one_q, 0, 0},
      {one_q, -one_q, 0},
      {3 * one_q, 2 * one_q, 1u << (kFracW - 1)},
      {120, -85, 0},  // tiny H -> high gain stress/saturation path
  };
  ok &= run_stream_test(dut, deterministic, false, "deterministic_no_stall", 8);

  // 3) Random no-stall stream.
  std::mt19937 rng_a(1234);
  std::uniform_int_distribution<int32_t> hdist(-20000, 20000);
  std::uniform_int_distribution<uint32_t> kdist(0, 20000);
  std::vector<InSample> random_a;
  random_a.reserve(320);
  for (int i = 0; i < 320; ++i) random_a.push_back(InSample{hdist(rng_a), hdist(rng_a), kdist(rng_a)});
  ok &= run_stream_test(dut, random_a, false, "random_no_stall", 4);

  // 4) Random with backpressure.
  std::mt19937 rng_b(5678);
  std::vector<InSample> random_b;
  random_b.reserve(320);
  for (int i = 0; i < 320; ++i) random_b.push_back(InSample{hdist(rng_b), hdist(rng_b), kdist(rng_b)});
  ok &= run_stream_test(dut, random_b, true, "random_with_stall", 4);

  // 5) Extra stress vectors near dynamic range.
  std::vector<InSample> stress{
      {30000, 30000, 0},
      {-30000, 25000, 0},
      {200, -150, 0},
      {200, -150, 1},
      {1000, 1000, 10},
      {-1000, -1200, 20},
      {32767, 0, 1},
      {-32768, 0, 1},
  };
  ok &= run_stream_test(dut, stress, true, "stress_with_stall", 8);

  // 6) Summary.
  if (!ok) {
    std::cerr << "[FAIL] FilterGen bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] FilterGen bench complete\n";
  dut.final();
  return 0;
}

