// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vfft2d_core.h for the primary calling header

#ifndef VERILATED_VFFT2D_CORE___024ROOT_H_
#define VERILATED_VFFT2D_CORE___024ROOT_H_  // guard

#include "verilated.h"


class Vfft2d_core__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vfft2d_core___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        VL_IN8(in_valid,0,0);
        VL_OUT8(in_ready,0,0);
        VL_OUT8(out_valid,0,0);
        VL_IN8(out_ready,0,0);
        CData/*2:0*/ fft2d_core__DOT__state;
        CData/*7:0*/ fft2d_core__DOT__row_idx;
        CData/*7:0*/ fft2d_core__DOT__col_idx;
        CData/*7:0*/ fft2d_core__DOT__feed_cnt;
        CData/*7:0*/ fft2d_core__DOT__recv_cnt;
        CData/*0:0*/ fft2d_core__DOT__f_in_valid;
        CData/*0:0*/ fft2d_core__DOT__f_in_ready;
        CData/*0:0*/ fft2d_core__DOT__f_out_valid;
        CData/*0:0*/ fft2d_core__DOT__f_out_ready;
        CData/*1:0*/ fft2d_core__DOT__u_fft1d__DOT__state;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__load_ptr;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__out_ptr;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__ctrl_start;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__ctrl_done;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_start;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_done;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__idx_a;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__idx_b;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__running;
        CData/*0:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__waiting;
        CData/*2:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__stage;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__j;
        CData/*7:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__base;
        CData/*2:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__0__r;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__0__c;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__1__r;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__1__c;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__2__r;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__2__c;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__3__r;
        CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__3__c;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
        CData/*0:0*/ __VactContinue;
        VL_IN16(in_re,15,0);
        VL_IN16(in_im,15,0);
        VL_OUT16(out_re,15,0);
        VL_OUT16(out_im,15,0);
        SData/*15:0*/ fft2d_core__DOT__out_ptr;
        SData/*15:0*/ fft2d_core__DOT__load_ptr;
        SData/*15:0*/ fft2d_core__DOT__f_in_re;
        SData/*15:0*/ fft2d_core__DOT__f_in_im;
        SData/*15:0*/ fft2d_core__DOT__f_out_re;
        SData/*15:0*/ fft2d_core__DOT__f_out_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__w_re;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__w_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_a_re;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_a_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_b_re;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__bfly_b_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_re;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_im;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_re;
        SData/*8:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__span;
        SData/*8:0*/ fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__half;
    };
    struct {
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__sum_re_r;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__sum_im_r;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_re_r;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_im_r;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_re_r;
        SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_im_r;
        SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout;
        SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout;
        SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout;
        SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout;
        IData/*31:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m1_r;
        IData/*31:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m2_r;
        IData/*31:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m3_r;
        IData/*31:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_p;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem0_re;
        VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem0_im;
        VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem1_re;
        VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem1_im;
        VlUnpacked<SData/*15:0*/, 256> fft2d_core__DOT__u_fft1d__DOT__mem_re;
        VlUnpacked<SData/*15:0*/, 256> fft2d_core__DOT__u_fft1d__DOT__mem_im;
    };
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vfft2d_core__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vfft2d_core___024root(Vfft2d_core__Syms* symsp, const char* v__name);
    ~Vfft2d_core___024root();
    VL_UNCOPYABLE(Vfft2d_core___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
