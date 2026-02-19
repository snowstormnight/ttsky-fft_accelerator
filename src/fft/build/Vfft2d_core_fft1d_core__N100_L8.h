// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vfft2d_core.h for the primary calling header

#ifndef VERILATED_VFFT2D_CORE_FFT1D_CORE__N100_L8_H_
#define VERILATED_VFFT2D_CORE_FFT1D_CORE__N100_L8_H_  // guard

#include "verilated.h"


class Vfft2d_core__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vfft2d_core_fft1d_core__N100_L8 final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(in_valid,0,0);
    VL_OUT8(in_ready,0,0);
    VL_OUT8(out_valid,0,0);
    VL_IN8(out_ready,0,0);
    CData/*1:0*/ __PVT__state;
    CData/*7:0*/ __PVT__load_ptr;
    CData/*7:0*/ __PVT__out_ptr;
    CData/*0:0*/ __PVT__ctrl_start;
    CData/*0:0*/ __PVT__ctrl_done;
    CData/*0:0*/ __PVT__bfly_start;
    CData/*0:0*/ __PVT__bfly_done;
    CData/*7:0*/ __PVT__idx_a;
    CData/*7:0*/ __PVT__idx_b;
    CData/*0:0*/ __PVT__u_ctrl__DOT__running;
    CData/*0:0*/ __PVT__u_ctrl__DOT__waiting;
    CData/*2:0*/ __PVT__u_ctrl__DOT__stage;
    CData/*7:0*/ __PVT__u_ctrl__DOT__j;
    CData/*7:0*/ __PVT__u_ctrl__DOT__base;
    CData/*2:0*/ __PVT__u_bfly__DOT__state;
    VL_IN16(in_re,15,0);
    VL_IN16(in_im,15,0);
    VL_OUT16(out_re,15,0);
    VL_OUT16(out_im,15,0);
    SData/*15:0*/ __PVT__w_re;
    SData/*15:0*/ __PVT__w_im;
    SData/*15:0*/ __PVT__bfly_a_re;
    SData/*15:0*/ __PVT__bfly_a_im;
    SData/*15:0*/ __PVT__bfly_b_re;
    SData/*15:0*/ __PVT__bfly_b_im;
    SData/*15:0*/ __Vcellinp__u_bfly__b_im;
    SData/*15:0*/ __Vcellinp__u_bfly__b_re;
    SData/*15:0*/ __Vcellinp__u_bfly__a_im;
    SData/*15:0*/ __Vcellinp__u_bfly__a_re;
    SData/*8:0*/ __PVT__u_ctrl__DOT__span;
    SData/*8:0*/ __PVT__u_ctrl__DOT__half;
    SData/*15:0*/ __PVT__u_bfly__DOT__sum_re_r;
    SData/*15:0*/ __PVT__u_bfly__DOT__sum_im_r;
    SData/*15:0*/ __PVT__u_bfly__DOT__diff_re_r;
    SData/*15:0*/ __PVT__u_bfly__DOT__diff_im_r;
    SData/*15:0*/ __PVT__u_bfly__DOT__w_re_r;
    SData/*15:0*/ __PVT__u_bfly__DOT__w_im_r;
    IData/*31:0*/ __PVT__u_bfly__DOT__m1_r;
    IData/*31:0*/ __PVT__u_bfly__DOT__m2_r;
    IData/*31:0*/ __PVT__u_bfly__DOT__m3_r;
    IData/*31:0*/ __PVT__u_bfly__DOT__mul_p;
    VlUnpacked<SData/*15:0*/, 256> __PVT__mem_re;
    VlUnpacked<SData/*15:0*/, 256> __PVT__mem_im;

    // INTERNAL VARIABLES
    Vfft2d_core__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vfft2d_core_fft1d_core__N100_L8(Vfft2d_core__Syms* symsp, const char* v__name);
    ~Vfft2d_core_fft1d_core__N100_L8();
    VL_UNCOPYABLE(Vfft2d_core_fft1d_core__N100_L8);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
