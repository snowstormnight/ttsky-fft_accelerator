// Run command (unit-test mode, no args):
//   ln -sfn "$(pwd)" /tmp/ttsky_fft_ws && cd /tmp/ttsky_fft_ws/src/fft
//   verilator -Wall --cc SV_fft2d_core.sv SV_fft1d_core.sv SV_fft_controller.sv SV_butterfly.sv SV_twiddle_rom.sv \
//     --top-module fft2d_core --exe /tmp/ttsky_fft_ws/src/fft/CPP_2d_fft_tb.cpp --build -j 0 \
//     -CFLAGS "-std=c++17" -Mdir /tmp/ttsky_build/fft2d
//   /tmp/ttsky_build/fft2d/Vfft2d_core
//
// Run command (file I/O mode used by Python flow):
//   /tmp/ttsky_build/fft2d/Vfft2d_core <input_tile.txt> <fft_output_hw.txt>

#include "Vfft2d_core.h"
#include "verilated.h"

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
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
#ifndef FFT_LANES
#define FFT_LANES 1
#endif

constexpr int kN = FFT_N;
constexpr int kLogN = FFT_LOGN;
constexpr int kTile = kN * kN;
constexpr int kLanes = FFT_LANES;

struct C16 {
  int16_t re;
  int16_t im;
};

void tick(Vfft2d_core& dut) {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void reset_dut(Vfft2d_core& dut) {
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

int bit_reverse(int x, int bits) {
  int r = 0;
  for (int i = 0; i < bits; ++i) {
    r = (r << 1) | ((x >> i) & 1);
  }
  return r;
}

void print_preview(const std::vector<C16>& out, const std::string& tag) {
  std::cout << "[INFO] " << tag << " first 6 bins:";
  for (int i = 0; i < 6 && i < static_cast<int>(out.size()); ++i) {
    std::cout << " (" << out[i].re << "," << out[i].im << ")";
  }
  std::cout << "\n";
}

bool run_frame(
    Vfft2d_core& dut,
    const std::vector<C16>& input_tile,
    std::vector<C16>& out_tile,
    uint64_t& perf_cycles,
    bool& got_perf,
    int& cycle_count
) {
  if (static_cast<int>(input_tile.size()) != kTile) return false;
  out_tile.assign(kTile, C16{0, 0});
  got_perf = false;
  perf_cycles = 0;
  cycle_count = 0;

  int in_idx = 0;
  int out_idx = 0;
  const long long max_cycles = 80LL * kTile * FFT_LOGN + 40000LL;

  for (long long cyc = 0; cyc < max_cycles && out_idx < kTile; ++cyc) {
    if (in_idx < kTile) {
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
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;
    const int16_t out_r = static_cast<int16_t>(dut.out_re);
    const int16_t out_i = static_cast<int16_t>(dut.out_im);
    if (dut.perf_done) {
      perf_cycles = static_cast<uint64_t>(dut.perf_cycles);
      got_perf = true;
    }
    tick(dut);
    if (dut.perf_done) {
      perf_cycles = static_cast<uint64_t>(dut.perf_cycles);
      got_perf = true;
    }

    if (take_in) ++in_idx;
    if (take_out) {
      out_tile[out_idx] = C16{out_r, out_i};
      ++out_idx;
    }
    ++cycle_count;
  }
  return out_idx == kTile;
}

bool parse_txt_tile(const std::string& in_path, std::vector<C16>& tile) {
  std::ifstream fin(in_path);
  if (!fin) return false;
  tile.assign(kTile, C16{0, 0});
  int idx = 0;
  int v = 0;
  while (fin >> v) {
    if (idx >= kTile) return false;
    tile[idx].re = static_cast<int16_t>(v);
    tile[idx].im = 0;
    ++idx;
  }
  return idx == kTile;
}

bool write_txt_tile_complex(const std::string& out_path, const std::vector<C16>& tile) {
  std::ofstream fout(out_path);
  if (!fout) return false;
  for (int i = 0; i < kTile; ++i) fout << tile[i].re << " " << tile[i].im << "\n";
  return true;
}

bool check_all_zero(const std::vector<C16>& out, const std::string& tag) {
  for (int i = 0; i < kTile; ++i) {
    if (out[i].re != 0 || out[i].im != 0) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im << ") exp=(0,0)\n";
      return false;
    }
  }
  return true;
}

bool check_dc_only(const std::vector<C16>& out, int16_t dc_val, const std::string& tag) {
  for (int i = 0; i < kTile; ++i) {
    const int16_t exp_re = (i == 0) ? dc_val : 0;
    if (out[i].re != exp_re || out[i].im != 0) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im
                << ") exp=(" << exp_re << ",0)\n";
      return false;
    }
  }
  return true;
}

bool check_single_bin_2d(
    const std::vector<C16>& out,
    int k_row,
    int k_col,
    int16_t re_val,
    int16_t im_val,
    int tol,
    const std::string& tag
) {
  const int br = bit_reverse(k_row, kLogN);
  const int bc = bit_reverse(k_col, kLogN);
  const int idx_hot = br * kN + bc;

  for (int i = 0; i < kTile; ++i) {
    const int16_t exp_re = (i == idx_hot) ? re_val : 0;
    const int16_t exp_im = (i == idx_hot) ? im_val : 0;
    const int dre = std::abs(static_cast<int>(out[i].re) - static_cast<int>(exp_re));
    const int dim = std::abs(static_cast<int>(out[i].im) - static_cast<int>(exp_im));
    if (dre > tol || dim > tol) {
      std::cerr << "[FAIL] " << tag << " idx=" << i
                << " got=(" << out[i].re << "," << out[i].im
                << ") exp=(" << exp_re << "," << exp_im << ")"
                << " hot_idx=" << idx_hot << " tol=" << tol << "\n";
      return false;
    }
  }
  return true;
}

bool run_and_report(
    Vfft2d_core& dut,
    const std::string& tag,
    const std::vector<C16>& in,
    const std::function<bool(const std::vector<C16>&, const std::string&)>& checker
) {
  std::vector<C16> out;
  uint64_t perf_cycles = 0;
  bool got_perf = false;
  int cyc = 0;

  std::cout << "[CASE] " << tag << " start\n";
  if (!run_frame(dut, in, out, perf_cycles, got_perf, cyc)) {
    std::cerr << "[FAIL] " << tag << " timed out\n";
    return false;
  }
  print_preview(out, tag);
  const bool ok = checker(out, tag);
  if (ok) {
    std::cout << "[PASS] " << tag
              << " sim_cycles=" << cyc
              << " perf_cycles=" << (got_perf ? std::to_string(perf_cycles) : "n/a")
              << "\n";
  }
  return ok;
}

int run_unit_tests(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Vfft2d_core dut;
  bool ok = true;

  // 1) Start.
  std::cout << "[INFO] fft2d bench start (N=" << kN << ", lanes=" << kLanes << ")\n";
  reset_dut(dut);

  // 2) Deterministic tests (expanded).
  std::vector<C16> in(kTile, C16{0, 0});
  ok &= run_and_report(
      dut, "zero_tile", in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_all_zero(out, tag);
      });

  std::fill(in.begin(), in.end(), C16{512, 0});
  ok &= run_and_report(
      dut, "const_pos_tile", in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_dc_only(out, 512, tag);
      });

  std::fill(in.begin(), in.end(), C16{-400, 0});
  ok &= run_and_report(
      dut, "const_neg_tile", in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_dc_only(out, -400, tag);
      });

  for (int r = 0; r < kN; ++r) {
    for (int c = 0; c < kN; ++c) {
      in[r * kN + c] = (r & 1) ? C16{-300, 0} : C16{300, 0};
    }
  }
  ok &= run_and_report(
      dut, "row_alternating_nyquist",
      in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_single_bin_2d(out, kN / 2, 0, 300, 0, 2, tag);
      });

  for (int r = 0; r < kN; ++r) {
    for (int c = 0; c < kN; ++c) {
      in[r * kN + c] = (c & 1) ? C16{-280, 0} : C16{280, 0};
    }
  }
  ok &= run_and_report(
      dut, "col_alternating_nyquist",
      in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_single_bin_2d(out, 0, kN / 2, 280, 0, 2, tag);
      });

  for (int r = 0; r < kN; ++r) {
    for (int c = 0; c < kN; ++c) {
      in[r * kN + c] = ((r + c) & 1) ? C16{-200, 0} : C16{200, 0};
    }
  }
  ok &= run_and_report(
      dut, "checkerboard_nyquist_both",
      in,
      [](const std::vector<C16>& out, const std::string& tag) {
        return check_single_bin_2d(out, kN / 2, kN / 2, 200, 0, 2, tag);
      });

  // 3) Summary.
  if (!ok) {
    std::cerr << "[FAIL] fft2d bench failed\n";
    dut.final();
    return 1;
  }
  std::cout << "[PASS] fft2d bench complete\n";
  dut.final();
  return 0;
}

int run_file_mode(int argc, char** argv) {
  std::vector<C16> input_tile;
  if (!parse_txt_tile(argv[1], input_tile)) {
    std::cerr << "failed to parse input tile, expected " << kTile << " integers (one per line)\n";
    return 2;
  }

  Verilated::commandArgs(argc, argv);
  Vfft2d_core dut;
  reset_dut(dut);

  std::vector<C16> out_tile;
  uint64_t perf_cycles = 0;
  bool got_perf = false;
  int cyc = 0;
  if (!run_frame(dut, input_tile, out_tile, perf_cycles, got_perf, cyc)) {
    std::cerr << "simulation timeout before receiving full 2D FFT output\n";
    dut.final();
    return 1;
  }

  if (!write_txt_tile_complex(argv[2], out_tile)) {
    std::cerr << "failed to open output file: " << argv[2] << "\n";
    dut.final();
    return 2;
  }

  std::cout << "PERF_CFG N=" << kN << " LANES=" << kLanes << "\n";
  if (got_perf) std::cout << "PERF_CYCLES " << perf_cycles << "\n";
  std::cout << "SIM_CYCLES " << cyc << "\n";

  dut.final();
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc == 1) return run_unit_tests(argc, argv);
  if (argc == 3) return run_file_mode(argc, argv);

  std::cerr << "usage:\n"
            << "  " << argv[0] << "                       # run built-in unit tests\n"
            << "  " << argv[0] << " <input_tile.txt> <fft_output_hw.txt>\n";
  return 2;
}
