#include "Vcomplex_mult.h"
#include "verilated.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>

namespace {

constexpr int IN_W = 16;
constexpr int OUT_W = 16;
constexpr int FRAC = 8;
constexpr int SUM_W = IN_W + 1;
constexpr int MUL_W = 2 * IN_W;

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

int64_t round_shift(int64_t x) {
  if (FRAC == 0) return x;
  const int64_t bias = int64_t(1) << (FRAC - 1);
  const int64_t xb = (x >= 0) ? (x + bias) : (x - bias);
  return xb >> FRAC;
}

int32_t sat16(int64_t x) {
  if (x > 32767) return 32767;
  if (x < -32768) return -32768;
  return static_cast<int32_t>(x);
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

struct Cx {
  int32_t re;
  int32_t im;
};

Cx golden(const Cx& a, const Cx& b, bool use_gauss, bool round_en, bool sat_en) {
  int64_t real_full = 0;
  int64_t imag_full = 0;

  if (use_gauss) {
    const int64_t ar_p_ai = sx_w(int64_t(a.re) + int64_t(a.im), SUM_W);
    const int64_t br_p_bi = sx_w(int64_t(b.re) + int64_t(b.im), SUM_W);

    // Match RTL truncation into MUL_W products.
    const int64_t k1 = sx_w(int64_t(a.re) * int64_t(b.re), MUL_W);
    const int64_t k2 = sx_w(int64_t(a.im) * int64_t(b.im), MUL_W);
    const int64_t k3 = sx_w(ar_p_ai * br_p_bi, MUL_W);
    real_full = k1 - k2;
    imag_full = k3 - k1 - k2;
  } else {
    const int64_t m1 = sx_w(int64_t(a.re) * int64_t(b.re), MUL_W);
    const int64_t m2 = sx_w(int64_t(a.im) * int64_t(b.im), MUL_W);
    const int64_t m3 = sx_w(int64_t(a.re) * int64_t(b.im), MUL_W);
    const int64_t m4 = sx_w(int64_t(a.im) * int64_t(b.re), MUL_W);
    real_full = m1 - m2;
    imag_full = m3 + m4;
  }

  int64_t real_s = real_full;
  int64_t imag_s = imag_full;
  if (FRAC > 0) {
    if (round_en) {
      const int64_t bias = int64_t(1) << (FRAC - 1);
      real_s = (real_s >= 0) ? ((real_s + bias) >> FRAC) : ((real_s - bias) >> FRAC);
      imag_s = (imag_s >= 0) ? ((imag_s + bias) >> FRAC) : ((imag_s - bias) >> FRAC);
    } else {
      real_s = real_s >> FRAC;
      imag_s = imag_s >> FRAC;
    }
  }

  Cx y{};
  if (sat_en) {
    y.re = sat16(real_s);
    y.im = sat16(imag_s);
  } else {
    y.re = sx(pack(static_cast<int32_t>(real_s), OUT_W), OUT_W);
    y.im = sx(pack(static_cast<int32_t>(imag_s), OUT_W), OUT_W);
  }
  return y;
}

bool run_case(bool use_gauss, bool round_en, bool sat_en, bool pipe1, int iters) {
  Vcomplex_mult dut;
  dut.rst_n = 0;
  dut.in_valid = 0;
  dut.out_ready = 1;
  dut.a_re = 0;
  dut.a_im = 0;
  dut.b_re = 0;
  dut.b_im = 0;

  for (int i = 0; i < 3; ++i) tick(dut);
  dut.rst_n = 1;

  std::mt19937 rng(12345 + use_gauss * 17 + round_en * 31 + sat_en * 47 + pipe1 * 59);
  std::uniform_int_distribution<int32_t> dist(-32768, 32767);

  int sent = 0;
  int got = 0;
  Cx exp_q[4096];

  while (got < iters) {
    bool can_send = (sent < iters) && dut.in_ready;

    if (can_send) {
      Cx a{dist(rng), dist(rng)};
      Cx b{dist(rng), dist(rng)};
      exp_q[sent] = golden(a, b, use_gauss, round_en, sat_en);

      dut.in_valid = 1;
      dut.a_re = pack(a.re, IN_W);
      dut.a_im = pack(a.im, IN_W);
      dut.b_re = pack(b.re, IN_W);
      dut.b_im = pack(b.im, IN_W);
    } else {
      dut.in_valid = 0;
    }

    dut.eval();
    const bool take_in = dut.in_valid && dut.in_ready;
    const bool take_out = dut.out_valid && dut.out_ready;

    const int32_t yre = sx(static_cast<uint32_t>(dut.y_re), OUT_W);
    const int32_t yim = sx(static_cast<uint32_t>(dut.y_im), OUT_W);

    tick(dut);

    if (take_in) ++sent;
    if (take_out) {
      const Cx e = exp_q[got];
      if (yre != e.re || yim != e.im) {
        std::cerr << "Mismatch cfg[g=" << use_gauss << " r=" << round_en
                  << " s=" << sat_en << " p=" << pipe1 << "] idx=" << got
                  << " got=(" << yre << "," << yim << ") exp=(" << e.re
                  << "," << e.im << ")\n";
        return false;
      }
      ++got;
    }
  }

  std::cout << "PASS cfg[g=" << use_gauss << " r=" << round_en
            << " s=" << sat_en << " p=" << pipe1 << "] iters=" << iters << "\n";
  return true;
}

}  // namespace

int main() {
  Verilated::randReset(2);

  const bool use_gauss =
#ifdef TB_USE_GAUSS
      (TB_USE_GAUSS != 0);
#else
      true;
#endif
  const bool round_en =
#ifdef TB_ROUND
      (TB_ROUND != 0);
#else
      true;
#endif
  const bool sat_en =
#ifdef TB_SAT
      (TB_SAT != 0);
#else
      false;
#endif
  const bool pipe1 =
#ifdef TB_PIPE1
      (TB_PIPE1 != 0);
#else
      true;
#endif

  const int iters = 2000;
  if (!run_case(use_gauss, round_en, sat_en, pipe1, iters)) return 1;
  return 0;
}
