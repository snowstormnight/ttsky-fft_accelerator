// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vfft2d_core.h for the primary calling header

#ifndef VERILATED_VFFT2D_CORE___024ROOT_H_
#define VERILATED_VFFT2D_CORE___024ROOT_H_  // guard

#include "verilated.h"
class Vfft2d_core_fft1d_core__N100_L8;


class Vfft2d_core__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vfft2d_core___024root final : public VerilatedModule {
  public:
    // CELLS
    Vfft2d_core_fft1d_core__N100_L8* __PVT__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d;
    Vfft2d_core_fft1d_core__N100_L8* __PVT__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d;
    Vfft2d_core_fft1d_core__N100_L8* __PVT__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d;
    Vfft2d_core_fft1d_core__N100_L8* __PVT__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(in_valid,0,0);
    VL_OUT8(in_ready,0,0);
    VL_OUT8(out_valid,0,0);
    VL_IN8(out_ready,0,0);
    VL_OUT8(perf_done,0,0);
    CData/*2:0*/ fft2d_core__DOT__state;
    CData/*7:0*/ fft2d_core__DOT__batch_base;
    CData/*7:0*/ fft2d_core__DOT__feed_cnt;
    CData/*7:0*/ fft2d_core__DOT__recv_cnt;
    CData/*3:0*/ fft2d_core__DOT__f_in_valid;
    CData/*3:0*/ fft2d_core__DOT__f_out_ready;
    CData/*0:0*/ fft2d_core__DOT__feed_fire;
    CData/*0:0*/ fft2d_core__DOT__recv_fire;
    CData/*0:0*/ fft2d_core__DOT__perf_active;
    CData/*7:0*/ fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
    CData/*7:0*/ fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
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
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout;
    IData/*31:0*/ __VactIterCount;
    VL_OUT64(perf_cycles,63,0);
    QData/*63:0*/ fft2d_core__DOT__perf_counter;
    VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem0_re;
    VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem0_im;
    VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem1_re;
    VlUnpacked<SData/*15:0*/, 65536> fft2d_core__DOT__mem1_im;
    VlUnpacked<SData/*15:0*/, 4> fft2d_core__DOT__f_in_re;
    VlUnpacked<SData/*15:0*/, 4> fft2d_core__DOT__f_in_im;
    VlUnpacked<SData/*15:0*/, 4> fft2d_core__DOT__f_out_re;
    VlUnpacked<SData/*15:0*/, 4> fft2d_core__DOT__f_out_im;
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
