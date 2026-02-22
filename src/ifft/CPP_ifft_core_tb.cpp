// Run command:
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/ifft
//   verilator -Wall --cc ifft_core.sv \
//     --top-module ifft_core -GN=16 -GDATA_W=16 -GFRAC_W=12 \
//     --exe /tmp/ttsky_fft_ws/src/ifft/CPP_ifft_core_tb.cpp --build -j 0 \
//     -CFLAGS "-std=c++17 -DIFFT_N=16 -DIFFT_DATA_W=16 -DIFFT_FRAC_W=12" \
//     -Mdir /tmp/ttsky_build/ifft
//   /tmp/ttsky_build/ifft/Vifft_core

#include "Vifft_core.h"
#include "verilated.h"

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

#ifndef IFFT_N
#define IFFT_N 16
#endif
#ifndef IFFT_DATA_W
#define IFFT_DATA_W 16
#endif
#ifndef IFFT_FRAC_W
#define IFFT_FRAC_W 12
#endif

constexpr int kN = IFFT_N;
constexpr int kDataW = IFFT_DATA_W;

struct Cx {
  int32_t re;
  int32_t im;
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
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

void print_preview(const std::vector<Cx>& out, const std::string& tag) {
  std::cout << "[INFO] " << tag << " first 8 samples:";
  for (int i = 0; i < 8 && i < static_cast<int>(out.size()); ++i) {
    std::cout << " (" << out[i].re << "," << out[i].im << ")";
  }
  std::cout << "\n";
}

bool run_frame(
    Vifft_core& dut,
    const std::vector<Cx>& in,
    std::vector<Cx>& out,
    bool& saw_frame_done,
    int& out_last_count,
    int& cycle_count
) {
  if (static_cast<int>(in.size()) != kN) return false;
  out.clear();
  out.reserve(kN);
  saw_frame_done = false;
  out_last_count = 0;
  cycle_count = 0;

  int in_idx = 0;
  const long long max_cycles = 350000;
  for (long long cyc = 0; cyc < max_cycles && static_cast<int>(out.size()) < kN; ++cyc) {
    if (in_idx < kN) {
      dut.in_valid = 1;
      dut.in_re = pack_signed(in[in_idx].re, kDataW);
      dut.in_im = pack_signed(in[in_idx].im, kDataW);
    } else {
      dut.in_valid = 0;
      dut.in_re = 0;
      dut.in_im = 0;
    }
    dut.out_ready = 1;
    dut.eval();

    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const bool out_last = dut.out_last;
    const bool frame_done_now = dut.frame_done;
    const int32_t out_re = sx(static_cast<uint32_t>(dut.out_re), kDataW);
    const int32_t out_im = sx(static_cast<uint32_t>(dut.out_im), kDataW);

    if (take_in) ++in_idx;
    if (take_out) {
      out.push_back(Cx{out_re, out_im});
      if (out_last) ++out_last_count;
    }
    if (frame_done_now) saw_frame_done = true;

    tick(dut);
    if (dut.frame_done) saw_frame_done = true;
    ++cycle_count;
  }
  return static_cast<int>(out.size()) == kN;
}

bool check_zero(const std::vector<Cx>& out, const std::string& tag) {
  for (int i = 0; i < kN; ++i) {
    if (out[i].re != 0 || out[i].im != 0) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im << ") exp=(0,0)\n";
      return false;
    }
  }
  return true;
}

bool check_constant(const std::vector<Cx>& out, int32_t re_exp, int32_t im_exp, int tol, const std::string& tag) {
  for (int i = 0; i < kN; ++i) {
    const int dre = std::abs(out[i].re - re_exp);
    const int dim = std::abs(out[i].im - im_exp);
    if (dre > tol || dim > tol) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im << ")"
                << " exp=(" << re_exp << "," << im_exp << ") tol=" << tol << "\n";
      return false;
    }
  }
  return true;
}

bool check_impulse00(const std::vector<Cx>& out, int32_t re0, int32_t im0, int tol, const std::string& tag) {
  for (int i = 0; i < kN; ++i) {
    const int32_t exp_re = (i == 0) ? re0 : 0;
    const int32_t exp_im = (i == 0) ? im0 : 0;
    const int dre = std::abs(out[i].re - exp_re);
    const int dim = std::abs(out[i].im - exp_im);
    if (dre > tol || dim > tol) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im << ")"
                << " exp=(" << exp_re << "," << exp_im << ") tol=" << tol << "\n";
      return false;
    }
  }
  return true;
}

bool run_and_report(
    Vifft_core& dut,
    const std::string& tag,
    const std::vector<Cx>& in,
    const std::function<bool(const std::vector<Cx>&, const std::string&)>& checker
) {
  std::vector<Cx> out;
  bool saw_done = false;
  int out_last_count = 0;
  int cyc = 0;
  std::cout << "[CASE] " << tag << " start\n";
  if (!run_frame(dut, in, out, saw_done, out_last_count, cyc)) {
    std::cerr << "[FAIL] " << tag << " timed out\n";
    return false;
  }
  print_preview(out, tag);
  if (!saw_done || out_last_count != 1) {
    std::cerr << "[FAIL] " << tag
              << " handshake frame_done=" << saw_done
              << " out_last_count=" << out_last_count << "\n";
    return false;
  }
  const bool ok = checker(out, tag);
  if (ok) std::cout << "[PASS] " << tag << " cycles=" << cyc << "\n";
  return ok;
}

}  // namespace

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Vifft_core dut;
  bool ok = true;

  // 1) Start.
  std::cout << "[INFO] ifft bench start (N=" << kN << ")\n";
  reset_dut(dut);

  // 2) Deterministic tests (expanded).
  std::vector<Cx> in(kN, Cx{0, 0});

  ok &= run_and_report(
      dut, "zero_spectrum", in,
      [](const std::vector<Cx>& out, const std::string& tag) {
        return check_zero(out, tag);
      });

  std::fill(in.begin(), in.end(), Cx{0, 0});
  in[0] = Cx{kN * 64, 0};
  ok &= run_and_report(
      dut, "dc_only_real_pos",
      in,
      [](const std::vector<Cx>& out, const std::string& tag) {
        return check_constant(out, 64, 0, 0, tag);
      });

  std::fill(in.begin(), in.end(), Cx{0, 0});
  in[0] = Cx{kN * -48, 0};
  ok &= run_and_report(
      dut, "dc_only_real_neg",
      in,
      [](const std::vector<Cx>& out, const std::string& tag) {
        return check_constant(out, -48, 0, 0, tag);
      });

  std::fill(in.begin(), in.end(), Cx{0, 0});
  in[0] = Cx{0, kN * 40};
  ok &= run_and_report(
      dut, "dc_only_imag_pos",
      in,
      [](const std::vector<Cx>& out, const std::string& tag) {
        return check_constant(out, 0, 40, 0, tag);
      });

  std::fill(in.begin(), in.end(), Cx{37, 0});
  ok &= run_and_report(
      dut, "constant_spectrum_real",
      in,
      [](const std::vector<Cx>& out, const std::string& tag) {
        return check_impulse00(out, 37, 0, 2, tag);
      });

  // 3) Summary.
  if (!ok) {
    std::cerr << "[FAIL] ifft bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] ifft bench complete\n";
  dut.final();
  return 0;
}

