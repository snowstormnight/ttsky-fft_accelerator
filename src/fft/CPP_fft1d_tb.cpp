// Run command:
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/fft
//   verilator -Wall --cc SV_fft1d_core.sv SV_fft_controller.sv SV_butterfly.sv SV_twiddle_rom.sv \
//     --top-module fft1d_core --exe /tmp/ttsky_fft_ws/src/fft/CPP_fft1d_tb.cpp --build -j 0 \
//     -CFLAGS "-std=c++17" -Mdir /tmp/ttsky_build/fft1d
//   /tmp/ttsky_build/fft1d/Vfft1d_core

#include "Vfft1d_core.h"
#include "verilated.h"

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int kN = 32;
constexpr int kLogN = 5;

struct Cx16 {
  int16_t re;
  int16_t im;
};

void tick(Vfft1d_core& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vfft1d_core& dut) {
  dut.rst_n = 0;
  dut.in_valid = 0;
  dut.in_re = 0;
  dut.in_im = 0;
  dut.out_ready = 1;
  dut.eval();
  for (int i = 0; i < 4; ++i) tick(dut);
  dut.rst_n = 1;
}

int bit_reverse(int x, int bits) {
  int r = 0;
  for (int i = 0; i < bits; ++i) {
    r = (r << 1) | ((x >> i) & 1);
  }
  return r;
}

void print_preview(const std::vector<Cx16>& out, const std::string& tag) {
  std::cout << "[INFO] " << tag << " first 8 bins:";
  for (int i = 0; i < 8 && i < static_cast<int>(out.size()); ++i) {
    std::cout << " (" << out[i].re << "," << out[i].im << ")";
  }
  std::cout << "\n";
}

bool run_frame(
    Vfft1d_core& dut,
    const std::vector<Cx16>& in,
    std::vector<Cx16>& out,
    int& cycle_count
) {
  if (static_cast<int>(in.size()) != kN) return false;
  out.clear();
  out.reserve(kN);
  cycle_count = 0;

  int in_idx = 0;
  const long long max_cycles = 120000;
  for (long long cyc = 0; cyc < max_cycles && static_cast<int>(out.size()) < kN; ++cyc) {
    if (in_idx < kN) {
      dut.in_valid = 1;
      dut.in_re = static_cast<uint16_t>(in[in_idx].re);
      dut.in_im = static_cast<uint16_t>(in[in_idx].im);
    } else {
      dut.in_valid = 0;
      dut.in_re = 0;
      dut.in_im = 0;
    }
    dut.out_ready = 1;
    dut.eval();

    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int16_t out_re = static_cast<int16_t>(dut.out_re);
    const int16_t out_im = static_cast<int16_t>(dut.out_im);
    tick(dut);

    if (take_in) ++in_idx;
    if (take_out) out.push_back(Cx16{out_re, out_im});
    ++cycle_count;
  }

  return static_cast<int>(out.size()) == kN;
}

bool check_all_zero(const std::vector<Cx16>& out, const std::string& tag) {
  for (int i = 0; i < kN; ++i) {
    if (out[i].re != 0 || out[i].im != 0) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im << ") exp=(0,0)\n";
      return false;
    }
  }
  return true;
}

bool check_dc_only(
    const std::vector<Cx16>& out,
    int16_t dc_re,
    int16_t dc_im,
    const std::string& tag
) {
  for (int i = 0; i < kN; ++i) {
    const int16_t exp_re = (i == 0) ? dc_re : 0;
    const int16_t exp_im = (i == 0) ? dc_im : 0;
    if (out[i].re != exp_re || out[i].im != exp_im) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im
                << ") exp=(" << exp_re << "," << exp_im << ")\n";
      return false;
    }
  }
  return true;
}

bool check_all_equal(
    const std::vector<Cx16>& out,
    int16_t re_val,
    int16_t im_val,
    int tol,
    const std::string& tag
) {
  for (int i = 0; i < kN; ++i) {
    const int dre = std::abs(static_cast<int>(out[i].re) - static_cast<int>(re_val));
    const int dim = std::abs(static_cast<int>(out[i].im) - static_cast<int>(im_val));
    if (dre > tol || dim > tol) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im
                << ") exp=(" << re_val << "," << im_val << ")"
                << " tol=" << tol << "\n";
      return false;
    }
  }
  return true;
}

bool check_single_bin(
    const std::vector<Cx16>& out,
    int bin_idx,
    int16_t re_val,
    int16_t im_val,
    int tol,
    const std::string& tag
) {
  for (int i = 0; i < kN; ++i) {
    const int16_t exp_re = (i == bin_idx) ? re_val : 0;
    const int16_t exp_im = (i == bin_idx) ? im_val : 0;
    const int dre = std::abs(static_cast<int>(out[i].re) - static_cast<int>(exp_re));
    const int dim = std::abs(static_cast<int>(out[i].im) - static_cast<int>(exp_im));
    if (dre > tol || dim > tol) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im
                << ") exp=(" << exp_re << "," << exp_im << ")"
                << " tol=" << tol << "\n";
      return false;
    }
  }
  return true;
}

bool run_and_report(
    Vfft1d_core& dut,
    const std::string& tag,
    const std::vector<Cx16>& in,
    const std::function<bool(const std::vector<Cx16>&, const std::string&)>& checker
) {
  std::vector<Cx16> out;
  int cyc = 0;
  std::cout << "[CASE] " << tag << " start\n";
  if (!run_frame(dut, in, out, cyc)) {
    std::cerr << "[FAIL] " << tag << " timed out\n";
    return false;
  }
  print_preview(out, tag);
  const bool ok = checker(out, tag);
  if (ok) {
    std::cout << "[PASS] " << tag << " cycles=" << cyc << "\n";
  }
  return ok;
}

}  // namespace

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Vfft1d_core dut;
  bool ok = true;

  // 1) Start.
  std::cout << "[INFO] fft1d bench start (N=" << kN << ")\n";
  reset_dut(dut);

  // 2) Deterministic tests (expanded).
  std::vector<Cx16> in(kN, Cx16{0, 0});

  ok &= run_and_report(
      dut, "zero_frame", in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_all_zero(out, tag);
      });

  std::fill(in.begin(), in.end(), Cx16{1024, 0});
  ok &= run_and_report(
      dut, "const_pos_real", in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_dc_only(out, 1024, 0, tag);
      });

  std::fill(in.begin(), in.end(), Cx16{-1024, 0});
  ok &= run_and_report(
      dut, "const_neg_real", in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_dc_only(out, -1024, 0, tag);
      });

  std::fill(in.begin(), in.end(), Cx16{0, 768});
  ok &= run_and_report(
      dut, "const_imag", in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_dc_only(out, 0, 768, tag);
      });

  std::fill(in.begin(), in.end(), Cx16{300, -500});
  ok &= run_and_report(
      dut, "const_complex", in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_dc_only(out, 300, -500, tag);
      });

  std::fill(in.begin(), in.end(), Cx16{0, 0});
  in[0] = Cx16{1024, 0};
  ok &= run_and_report(
      dut, "impulse_n0_real",
      in,
      [](const std::vector<Cx16>& out, const std::string& tag) {
        return check_all_equal(out, 32, 0, 3, tag);
      });

  for (int i = 0; i < kN; ++i) in[i] = (i & 1) ? Cx16{-900, 0} : Cx16{900, 0};
  const int spike_idx = bit_reverse(kN / 2, kLogN);  // DIF output order
  ok &= run_and_report(
      dut, "alternating_real_nyquist",
      in,
      [spike_idx](const std::vector<Cx16>& out, const std::string& tag) {
        return check_single_bin(out, spike_idx, 900, 0, 2, tag);
      });

  // 3) Summary.
  if (!ok) {
    std::cerr << "[FAIL] fft1d bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] fft1d bench complete\n";
  dut.final();
  return 0;
}
