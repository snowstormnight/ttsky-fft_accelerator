// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core__Syms.h"
#include "Vfft2d_core___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__act(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG

void Vfft2d_core___024root___eval_triggers__act(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.set(0U, (((IData)(vlSelf->clk) 
                                      & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__clk__0))) 
                                     | ((~ (IData)(vlSelf->rst_n)) 
                                        & (IData)(vlSelf->__Vtrigprevexpr___TOP__rst_n__0))));
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vfft2d_core___024root___dump_triggers__act(vlSelf);
    }
#endif
}

VL_INLINE_OPT void Vfft2d_core___024root___nba_sequent__TOP__0(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*3:0*/ fft2d_core__DOT__f_in_ready;
    fft2d_core__DOT__f_in_ready = 0;
    CData/*3:0*/ fft2d_core__DOT__f_out_valid;
    fft2d_core__DOT__f_out_valid = 0;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout;
    __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__4__r;
    __Vfunc_fft2d_core__DOT__idx2d__4__r = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__4__c;
    __Vfunc_fft2d_core__DOT__idx2d__4__c = 0;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout;
    __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__5__r;
    __Vfunc_fft2d_core__DOT__idx2d__5__r = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__5__c;
    __Vfunc_fft2d_core__DOT__idx2d__5__c = 0;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout;
    __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__6__r;
    __Vfunc_fft2d_core__DOT__idx2d__6__r = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__6__c;
    __Vfunc_fft2d_core__DOT__idx2d__6__c = 0;
    SData/*15:0*/ __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout;
    __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__7__r;
    __Vfunc_fft2d_core__DOT__idx2d__7__r = 0;
    CData/*7:0*/ __Vfunc_fft2d_core__DOT__idx2d__7__c;
    __Vfunc_fft2d_core__DOT__idx2d__7__c = 0;
    CData/*0:0*/ __Vdly__fft2d_core__DOT__perf_active;
    __Vdly__fft2d_core__DOT__perf_active = 0;
    QData/*63:0*/ __Vdly__fft2d_core__DOT__perf_counter;
    __Vdly__fft2d_core__DOT__perf_counter = 0;
    CData/*2:0*/ __Vdly__fft2d_core__DOT__state;
    __Vdly__fft2d_core__DOT__state = 0;
    SData/*15:0*/ __Vdly__fft2d_core__DOT__out_ptr;
    __Vdly__fft2d_core__DOT__out_ptr = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_re__v0;
    __Vdlyvdim0__fft2d_core__DOT__mem0_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_re__v0;
    __Vdlyvval__fft2d_core__DOT__mem0_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem0_re__v0;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v0 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_im__v0;
    __Vdlyvdim0__fft2d_core__DOT__mem0_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_im__v0;
    __Vdlyvval__fft2d_core__DOT__mem0_im__v0 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_re__v1;
    __Vdlyvdim0__fft2d_core__DOT__mem0_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_re__v1;
    __Vdlyvval__fft2d_core__DOT__mem0_re__v1 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem0_re__v1;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v1 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_im__v1;
    __Vdlyvdim0__fft2d_core__DOT__mem0_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_im__v1;
    __Vdlyvval__fft2d_core__DOT__mem0_im__v1 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_re__v2;
    __Vdlyvdim0__fft2d_core__DOT__mem0_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_re__v2;
    __Vdlyvval__fft2d_core__DOT__mem0_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem0_re__v2;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v2 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_im__v2;
    __Vdlyvdim0__fft2d_core__DOT__mem0_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_im__v2;
    __Vdlyvval__fft2d_core__DOT__mem0_im__v2 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_re__v3;
    __Vdlyvdim0__fft2d_core__DOT__mem0_re__v3 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_re__v3;
    __Vdlyvval__fft2d_core__DOT__mem0_re__v3 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem0_re__v3;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v3 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_im__v3;
    __Vdlyvdim0__fft2d_core__DOT__mem0_im__v3 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_im__v3;
    __Vdlyvval__fft2d_core__DOT__mem0_im__v3 = 0;
    CData/*7:0*/ __Vdly__fft2d_core__DOT__recv_cnt;
    __Vdly__fft2d_core__DOT__recv_cnt = 0;
    CData/*7:0*/ __Vdly__fft2d_core__DOT__batch_base;
    __Vdly__fft2d_core__DOT__batch_base = 0;
    CData/*7:0*/ __Vdly__fft2d_core__DOT__feed_cnt;
    __Vdly__fft2d_core__DOT__feed_cnt = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_re__v0;
    __Vdlyvdim0__fft2d_core__DOT__mem1_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_re__v0;
    __Vdlyvval__fft2d_core__DOT__mem1_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem1_re__v0;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v0 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_im__v0;
    __Vdlyvdim0__fft2d_core__DOT__mem1_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_im__v0;
    __Vdlyvval__fft2d_core__DOT__mem1_im__v0 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_re__v1;
    __Vdlyvdim0__fft2d_core__DOT__mem1_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_re__v1;
    __Vdlyvval__fft2d_core__DOT__mem1_re__v1 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem1_re__v1;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v1 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_im__v1;
    __Vdlyvdim0__fft2d_core__DOT__mem1_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_im__v1;
    __Vdlyvval__fft2d_core__DOT__mem1_im__v1 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_re__v2;
    __Vdlyvdim0__fft2d_core__DOT__mem1_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_re__v2;
    __Vdlyvval__fft2d_core__DOT__mem1_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem1_re__v2;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v2 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_im__v2;
    __Vdlyvdim0__fft2d_core__DOT__mem1_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_im__v2;
    __Vdlyvval__fft2d_core__DOT__mem1_im__v2 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_re__v3;
    __Vdlyvdim0__fft2d_core__DOT__mem1_re__v3 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_re__v3;
    __Vdlyvval__fft2d_core__DOT__mem1_re__v3 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem1_re__v3;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v3 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem1_im__v3;
    __Vdlyvdim0__fft2d_core__DOT__mem1_im__v3 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem1_im__v3;
    __Vdlyvval__fft2d_core__DOT__mem1_im__v3 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_re__v4;
    __Vdlyvdim0__fft2d_core__DOT__mem0_re__v4 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_re__v4;
    __Vdlyvval__fft2d_core__DOT__mem0_re__v4 = 0;
    CData/*0:0*/ __Vdlyvset__fft2d_core__DOT__mem0_re__v4;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v4 = 0;
    SData/*15:0*/ __Vdlyvdim0__fft2d_core__DOT__mem0_im__v4;
    __Vdlyvdim0__fft2d_core__DOT__mem0_im__v4 = 0;
    SData/*15:0*/ __Vdlyvval__fft2d_core__DOT__mem0_im__v4;
    __Vdlyvval__fft2d_core__DOT__mem0_im__v4 = 0;
    SData/*15:0*/ __Vdly__fft2d_core__DOT__load_ptr;
    __Vdly__fft2d_core__DOT__load_ptr = 0;
    // Body
    __Vdly__fft2d_core__DOT__load_ptr = vlSelf->fft2d_core__DOT__load_ptr;
    __Vdly__fft2d_core__DOT__recv_cnt = vlSelf->fft2d_core__DOT__recv_cnt;
    __Vdly__fft2d_core__DOT__perf_counter = vlSelf->fft2d_core__DOT__perf_counter;
    __Vdly__fft2d_core__DOT__perf_active = vlSelf->fft2d_core__DOT__perf_active;
    __Vdly__fft2d_core__DOT__feed_cnt = vlSelf->fft2d_core__DOT__feed_cnt;
    __Vdly__fft2d_core__DOT__batch_base = vlSelf->fft2d_core__DOT__batch_base;
    __Vdly__fft2d_core__DOT__out_ptr = vlSelf->fft2d_core__DOT__out_ptr;
    __Vdly__fft2d_core__DOT__state = vlSelf->fft2d_core__DOT__state;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v0 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v1 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v2 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem1_re__v3 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v0 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v1 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v2 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v3 = 0U;
    __Vdlyvset__fft2d_core__DOT__mem0_re__v4 = 0U;
    fft2d_core__DOT__f_in_ready = (((0U == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.__PVT__state)) 
                                    << 3U) | (((0U 
                                                == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.__PVT__state)) 
                                               << 2U) 
                                              | (((0U 
                                                   == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.__PVT__state)) 
                                                  << 1U) 
                                                 | (0U 
                                                    == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.__PVT__state)))));
    fft2d_core__DOT__f_out_valid = (((2U == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.__PVT__state)) 
                                     << 3U) | (((2U 
                                                 == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.__PVT__state)) 
                                                << 2U) 
                                               | (((2U 
                                                    == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.__PVT__state)) 
                                                   << 1U) 
                                                  | (2U 
                                                     == (IData)(vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.__PVT__state)))));
    if (vlSelf->rst_n) {
        vlSelf->perf_done = 0U;
        if ((((((~ (IData)(vlSelf->fft2d_core__DOT__perf_active)) 
                & (0U == (IData)(vlSelf->fft2d_core__DOT__state))) 
               & (IData)(vlSelf->in_valid)) & (IData)(vlSelf->in_ready)) 
             & (0U == (IData)(vlSelf->fft2d_core__DOT__load_ptr)))) {
            __Vdly__fft2d_core__DOT__perf_active = 1U;
            __Vdly__fft2d_core__DOT__perf_counter = 1ULL;
        } else if (vlSelf->fft2d_core__DOT__perf_active) {
            __Vdly__fft2d_core__DOT__perf_counter = 
                (1ULL + vlSelf->fft2d_core__DOT__perf_counter);
        }
        if ((4U & (IData)(vlSelf->fft2d_core__DOT__state))) {
            if ((2U & (IData)(vlSelf->fft2d_core__DOT__state))) {
                __Vdly__fft2d_core__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
                if (((IData)(vlSelf->out_valid) & (IData)(vlSelf->out_ready))) {
                    if ((0xffffU == (IData)(vlSelf->fft2d_core__DOT__out_ptr))) {
                        if (vlSelf->fft2d_core__DOT__perf_active) {
                            vlSelf->perf_done = 1U;
                            vlSelf->perf_cycles = vlSelf->fft2d_core__DOT__perf_counter;
                            __Vdly__fft2d_core__DOT__perf_active = 0U;
                        }
                        __Vdly__fft2d_core__DOT__out_ptr = 0U;
                        __Vdly__fft2d_core__DOT__state = 0U;
                    } else {
                        __Vdly__fft2d_core__DOT__out_ptr 
                            = (0xffffU & ((IData)(1U) 
                                          + (IData)(vlSelf->fft2d_core__DOT__out_ptr)));
                    }
                }
            } else if (vlSelf->fft2d_core__DOT__recv_fire) {
                vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l 
                    = vlSelf->fft2d_core__DOT__batch_base;
                __Vdlyvval__fft2d_core__DOT__mem0_re__v0 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [0U];
                __Vdlyvset__fft2d_core__DOT__mem0_re__v0 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem0_re__v0 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__4__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__4__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__4__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem0_im__v0 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [0U];
                __Vdlyvdim0__fft2d_core__DOT__mem0_im__v0 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__5__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__5__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__5__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l 
                    = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem0_re__v1 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [1U];
                __Vdlyvset__fft2d_core__DOT__mem0_re__v1 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem0_re__v1 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__4__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__4__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__4__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem0_im__v1 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [1U];
                __Vdlyvdim0__fft2d_core__DOT__mem0_im__v1 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__5__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__5__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__5__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l 
                    = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem0_re__v2 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [2U];
                __Vdlyvset__fft2d_core__DOT__mem0_re__v2 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem0_re__v2 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__4__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__4__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__4__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem0_im__v2 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [2U];
                __Vdlyvdim0__fft2d_core__DOT__mem0_im__v2 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__5__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__5__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__5__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l 
                    = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem0_re__v3 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [3U];
                __Vdlyvset__fft2d_core__DOT__mem0_re__v3 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem0_re__v3 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__4__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__4__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__4__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__4__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem0_im__v3 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [3U];
                __Vdlyvdim0__fft2d_core__DOT__mem0_im__v3 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__5__c 
                            = vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l;
                        __Vfunc_fft2d_core__DOT__idx2d__5__r 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__5__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__5__Vfuncout));
                if ((0xffU == (IData)(vlSelf->fft2d_core__DOT__recv_cnt))) {
                    __Vdly__fft2d_core__DOT__recv_cnt = 0U;
                    if ((0xfcU == (IData)(vlSelf->fft2d_core__DOT__batch_base))) {
                        __Vdly__fft2d_core__DOT__out_ptr = 0U;
                        __Vdly__fft2d_core__DOT__state = 5U;
                    } else {
                        __Vdly__fft2d_core__DOT__batch_base 
                            = (0xffU & ((IData)(4U) 
                                        + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                        __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                        __Vdly__fft2d_core__DOT__state = 3U;
                    }
                } else {
                    __Vdly__fft2d_core__DOT__recv_cnt 
                        = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__recv_cnt)));
                }
            }
        } else if ((2U & (IData)(vlSelf->fft2d_core__DOT__state))) {
            if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
                if (vlSelf->fft2d_core__DOT__feed_fire) {
                    if ((0xffU == (IData)(vlSelf->fft2d_core__DOT__feed_cnt))) {
                        __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                        __Vdly__fft2d_core__DOT__recv_cnt = 0U;
                        __Vdly__fft2d_core__DOT__state = 4U;
                    } else {
                        __Vdly__fft2d_core__DOT__feed_cnt 
                            = (0xffU & ((IData)(1U) 
                                        + (IData)(vlSelf->fft2d_core__DOT__feed_cnt)));
                    }
                }
            } else if (vlSelf->fft2d_core__DOT__recv_fire) {
                vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l 
                    = vlSelf->fft2d_core__DOT__batch_base;
                __Vdlyvval__fft2d_core__DOT__mem1_re__v0 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [0U];
                __Vdlyvset__fft2d_core__DOT__mem1_re__v0 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem1_re__v0 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__6__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__6__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__6__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem1_im__v0 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [0U];
                __Vdlyvdim0__fft2d_core__DOT__mem1_im__v0 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__7__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__7__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__7__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l 
                    = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem1_re__v1 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [1U];
                __Vdlyvset__fft2d_core__DOT__mem1_re__v1 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem1_re__v1 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__6__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__6__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__6__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem1_im__v1 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [1U];
                __Vdlyvdim0__fft2d_core__DOT__mem1_im__v1 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__7__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__7__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__7__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l 
                    = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem1_re__v2 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [2U];
                __Vdlyvset__fft2d_core__DOT__mem1_re__v2 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem1_re__v2 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__6__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__6__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__6__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem1_im__v2 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [2U];
                __Vdlyvdim0__fft2d_core__DOT__mem1_im__v2 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__7__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__7__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__7__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout));
                vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l 
                    = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                __Vdlyvval__fft2d_core__DOT__mem1_re__v3 
                    = vlSelf->fft2d_core__DOT__f_out_re
                    [3U];
                __Vdlyvset__fft2d_core__DOT__mem1_re__v3 = 1U;
                __Vdlyvdim0__fft2d_core__DOT__mem1_re__v3 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__6__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__6__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__6__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__6__Vfuncout));
                __Vdlyvval__fft2d_core__DOT__mem1_im__v3 
                    = vlSelf->fft2d_core__DOT__f_out_im
                    [3U];
                __Vdlyvdim0__fft2d_core__DOT__mem1_im__v3 
                    = ([&]() {
                        __Vfunc_fft2d_core__DOT__idx2d__7__c 
                            = vlSelf->fft2d_core__DOT__recv_cnt;
                        __Vfunc_fft2d_core__DOT__idx2d__7__r 
                            = vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l;
                        __Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout 
                            = (((IData)(__Vfunc_fft2d_core__DOT__idx2d__7__r) 
                                << 8U) | (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__c));
                    }(), (IData)(__Vfunc_fft2d_core__DOT__idx2d__7__Vfuncout));
                if ((0xffU == (IData)(vlSelf->fft2d_core__DOT__recv_cnt))) {
                    __Vdly__fft2d_core__DOT__recv_cnt = 0U;
                    if ((0xfcU == (IData)(vlSelf->fft2d_core__DOT__batch_base))) {
                        __Vdly__fft2d_core__DOT__batch_base = 0U;
                        __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                        __Vdly__fft2d_core__DOT__state = 3U;
                    } else {
                        __Vdly__fft2d_core__DOT__batch_base 
                            = (0xffU & ((IData)(4U) 
                                        + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                        __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                        __Vdly__fft2d_core__DOT__state = 1U;
                    }
                } else {
                    __Vdly__fft2d_core__DOT__recv_cnt 
                        = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__recv_cnt)));
                }
            }
        } else if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
            if (vlSelf->fft2d_core__DOT__feed_fire) {
                if ((0xffU == (IData)(vlSelf->fft2d_core__DOT__feed_cnt))) {
                    __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                    __Vdly__fft2d_core__DOT__recv_cnt = 0U;
                    __Vdly__fft2d_core__DOT__state = 2U;
                } else {
                    __Vdly__fft2d_core__DOT__feed_cnt 
                        = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__feed_cnt)));
                }
            }
        } else if (((IData)(vlSelf->in_valid) & (IData)(vlSelf->in_ready))) {
            __Vdlyvval__fft2d_core__DOT__mem0_re__v4 
                = vlSelf->in_re;
            __Vdlyvset__fft2d_core__DOT__mem0_re__v4 = 1U;
            __Vdlyvdim0__fft2d_core__DOT__mem0_re__v4 
                = vlSelf->fft2d_core__DOT__load_ptr;
            __Vdlyvval__fft2d_core__DOT__mem0_im__v4 
                = vlSelf->in_im;
            __Vdlyvdim0__fft2d_core__DOT__mem0_im__v4 
                = vlSelf->fft2d_core__DOT__load_ptr;
            if ((0xffffU == (IData)(vlSelf->fft2d_core__DOT__load_ptr))) {
                __Vdly__fft2d_core__DOT__batch_base = 0U;
                __Vdly__fft2d_core__DOT__load_ptr = 0U;
                __Vdly__fft2d_core__DOT__feed_cnt = 0U;
                __Vdly__fft2d_core__DOT__recv_cnt = 0U;
                __Vdly__fft2d_core__DOT__state = 1U;
            } else {
                __Vdly__fft2d_core__DOT__load_ptr = 
                    (0xffffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__load_ptr)));
            }
        }
    } else {
        __Vdly__fft2d_core__DOT__batch_base = 0U;
        __Vdly__fft2d_core__DOT__load_ptr = 0U;
        __Vdly__fft2d_core__DOT__state = 0U;
        __Vdly__fft2d_core__DOT__feed_cnt = 0U;
        __Vdly__fft2d_core__DOT__recv_cnt = 0U;
        __Vdly__fft2d_core__DOT__out_ptr = 0U;
        vlSelf->perf_done = 0U;
        vlSelf->perf_cycles = 0ULL;
        __Vdly__fft2d_core__DOT__perf_active = 0U;
        __Vdly__fft2d_core__DOT__perf_counter = 0ULL;
    }
    vlSelf->fft2d_core__DOT__perf_active = __Vdly__fft2d_core__DOT__perf_active;
    vlSelf->fft2d_core__DOT__perf_counter = __Vdly__fft2d_core__DOT__perf_counter;
    vlSelf->fft2d_core__DOT__recv_cnt = __Vdly__fft2d_core__DOT__recv_cnt;
    vlSelf->fft2d_core__DOT__load_ptr = __Vdly__fft2d_core__DOT__load_ptr;
    vlSelf->fft2d_core__DOT__f_out_re[0U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_re[1U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_re[2U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_re[3U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_im[0U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_im[1U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_im[2U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_im[3U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__out_ptr = __Vdly__fft2d_core__DOT__out_ptr;
    vlSelf->fft2d_core__DOT__batch_base = __Vdly__fft2d_core__DOT__batch_base;
    vlSelf->fft2d_core__DOT__feed_cnt = __Vdly__fft2d_core__DOT__feed_cnt;
    vlSelf->fft2d_core__DOT__state = __Vdly__fft2d_core__DOT__state;
    if (__Vdlyvset__fft2d_core__DOT__mem1_re__v0) {
        vlSelf->fft2d_core__DOT__mem1_im[__Vdlyvdim0__fft2d_core__DOT__mem1_im__v0] 
            = __Vdlyvval__fft2d_core__DOT__mem1_im__v0;
        vlSelf->fft2d_core__DOT__mem1_re[__Vdlyvdim0__fft2d_core__DOT__mem1_re__v0] 
            = __Vdlyvval__fft2d_core__DOT__mem1_re__v0;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem1_re__v1) {
        vlSelf->fft2d_core__DOT__mem1_im[__Vdlyvdim0__fft2d_core__DOT__mem1_im__v1] 
            = __Vdlyvval__fft2d_core__DOT__mem1_im__v1;
        vlSelf->fft2d_core__DOT__mem1_re[__Vdlyvdim0__fft2d_core__DOT__mem1_re__v1] 
            = __Vdlyvval__fft2d_core__DOT__mem1_re__v1;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem1_re__v2) {
        vlSelf->fft2d_core__DOT__mem1_im[__Vdlyvdim0__fft2d_core__DOT__mem1_im__v2] 
            = __Vdlyvval__fft2d_core__DOT__mem1_im__v2;
        vlSelf->fft2d_core__DOT__mem1_re[__Vdlyvdim0__fft2d_core__DOT__mem1_re__v2] 
            = __Vdlyvval__fft2d_core__DOT__mem1_re__v2;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem1_re__v3) {
        vlSelf->fft2d_core__DOT__mem1_im[__Vdlyvdim0__fft2d_core__DOT__mem1_im__v3] 
            = __Vdlyvval__fft2d_core__DOT__mem1_im__v3;
        vlSelf->fft2d_core__DOT__mem1_re[__Vdlyvdim0__fft2d_core__DOT__mem1_re__v3] 
            = __Vdlyvval__fft2d_core__DOT__mem1_re__v3;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem0_re__v0) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vdlyvdim0__fft2d_core__DOT__mem0_im__v0] 
            = __Vdlyvval__fft2d_core__DOT__mem0_im__v0;
        vlSelf->fft2d_core__DOT__mem0_re[__Vdlyvdim0__fft2d_core__DOT__mem0_re__v0] 
            = __Vdlyvval__fft2d_core__DOT__mem0_re__v0;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem0_re__v1) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vdlyvdim0__fft2d_core__DOT__mem0_im__v1] 
            = __Vdlyvval__fft2d_core__DOT__mem0_im__v1;
        vlSelf->fft2d_core__DOT__mem0_re[__Vdlyvdim0__fft2d_core__DOT__mem0_re__v1] 
            = __Vdlyvval__fft2d_core__DOT__mem0_re__v1;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem0_re__v2) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vdlyvdim0__fft2d_core__DOT__mem0_im__v2] 
            = __Vdlyvval__fft2d_core__DOT__mem0_im__v2;
        vlSelf->fft2d_core__DOT__mem0_re[__Vdlyvdim0__fft2d_core__DOT__mem0_re__v2] 
            = __Vdlyvval__fft2d_core__DOT__mem0_re__v2;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem0_re__v3) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vdlyvdim0__fft2d_core__DOT__mem0_im__v3] 
            = __Vdlyvval__fft2d_core__DOT__mem0_im__v3;
        vlSelf->fft2d_core__DOT__mem0_re[__Vdlyvdim0__fft2d_core__DOT__mem0_re__v3] 
            = __Vdlyvval__fft2d_core__DOT__mem0_re__v3;
    }
    if (__Vdlyvset__fft2d_core__DOT__mem0_re__v4) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vdlyvdim0__fft2d_core__DOT__mem0_im__v4] 
            = __Vdlyvval__fft2d_core__DOT__mem0_im__v4;
        vlSelf->fft2d_core__DOT__mem0_re[__Vdlyvdim0__fft2d_core__DOT__mem0_re__v4] 
            = __Vdlyvval__fft2d_core__DOT__mem0_re__v4;
    }
    vlSelf->in_ready = (0U == (IData)(vlSelf->fft2d_core__DOT__state));
    vlSelf->out_valid = (5U == (IData)(vlSelf->fft2d_core__DOT__state));
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    vlSelf->fft2d_core__DOT__f_in_valid = (0xeU & (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
    vlSelf->fft2d_core__DOT__f_in_re[0U] = 0U;
    vlSelf->fft2d_core__DOT__f_in_im[0U] = 0U;
    vlSelf->fft2d_core__DOT__f_out_ready = (0xeU & (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
    vlSelf->fft2d_core__DOT__f_in_valid = (0xdU & (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
    vlSelf->fft2d_core__DOT__f_in_re[1U] = 0U;
    vlSelf->fft2d_core__DOT__f_in_im[1U] = 0U;
    vlSelf->fft2d_core__DOT__f_out_ready = (0xdU & (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
    vlSelf->fft2d_core__DOT__f_in_valid = (0xbU & (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
    vlSelf->fft2d_core__DOT__f_in_re[2U] = 0U;
    vlSelf->fft2d_core__DOT__f_in_im[2U] = 0U;
    vlSelf->fft2d_core__DOT__f_out_ready = (0xbU & (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
    vlSelf->fft2d_core__DOT__f_in_valid = (7U & (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
    vlSelf->fft2d_core__DOT__f_in_re[3U] = 0U;
    vlSelf->fft2d_core__DOT__f_in_im[3U] = 0U;
    vlSelf->fft2d_core__DOT__f_out_ready = (7U & (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
    vlSelf->fft2d_core__DOT__feed_fire = 0U;
    vlSelf->fft2d_core__DOT__recv_fire = 0U;
    if ((4U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->fft2d_core__DOT__state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
                vlSelf->out_re = vlSelf->fft2d_core__DOT__mem0_re
                    [vlSelf->fft2d_core__DOT__out_ptr];
                vlSelf->out_im = vlSelf->fft2d_core__DOT__mem0_im
                    [vlSelf->fft2d_core__DOT__out_ptr];
            } else {
                vlSelf->fft2d_core__DOT__f_out_ready 
                    = (1U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
                vlSelf->fft2d_core__DOT__recv_fire 
                    = (1U & (IData)(fft2d_core__DOT__f_out_valid));
                vlSelf->fft2d_core__DOT__f_out_ready 
                    = (2U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
                vlSelf->fft2d_core__DOT__recv_fire 
                    = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                       & ((IData)(fft2d_core__DOT__f_out_valid) 
                          >> 1U));
                vlSelf->fft2d_core__DOT__f_out_ready 
                    = (4U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
                vlSelf->fft2d_core__DOT__recv_fire 
                    = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                       & ((IData)(fft2d_core__DOT__f_out_valid) 
                          >> 2U));
                vlSelf->fft2d_core__DOT__f_out_ready 
                    = (8U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
                vlSelf->fft2d_core__DOT__recv_fire 
                    = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                       & ((IData)(fft2d_core__DOT__f_out_valid) 
                          >> 3U));
            }
        }
    } else if ((2U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
            vlSelf->fft2d_core__DOT__f_in_valid = (1U 
                                                   | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
            vlSelf->fft2d_core__DOT__f_in_re[0U] = 
                vlSelf->fft2d_core__DOT__mem1_re[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c 
                        = vlSelf->fft2d_core__DOT__batch_base;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout))];
            vlSelf->fft2d_core__DOT__feed_fire = (1U 
                                                  & (IData)(fft2d_core__DOT__f_in_ready));
            vlSelf->fft2d_core__DOT__f_in_valid = (2U 
                                                   | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
            vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                     >> 1U));
            vlSelf->fft2d_core__DOT__f_in_im[0U] = 
                vlSelf->fft2d_core__DOT__mem1_im[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c 
                        = vlSelf->fft2d_core__DOT__batch_base;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_valid = (4U 
                                                   | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
            vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                     >> 2U));
            vlSelf->fft2d_core__DOT__f_in_re[1U] = 
                vlSelf->fft2d_core__DOT__mem1_re[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c 
                        = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_valid = (8U 
                                                   | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
            vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                     >> 3U));
            vlSelf->fft2d_core__DOT__f_in_im[1U] = 
                vlSelf->fft2d_core__DOT__mem1_im[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c 
                        = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_re[2U] = 
                vlSelf->fft2d_core__DOT__mem1_re[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c 
                        = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_im[2U] = 
                vlSelf->fft2d_core__DOT__mem1_im[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c 
                        = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_re[3U] = 
                vlSelf->fft2d_core__DOT__mem1_re[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c 
                        = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_im[3U] = 
                vlSelf->fft2d_core__DOT__mem1_im[([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c 
                        = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout))];
        } else {
            vlSelf->fft2d_core__DOT__f_out_ready = 
                (1U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
            vlSelf->fft2d_core__DOT__recv_fire = (1U 
                                                  & (IData)(fft2d_core__DOT__f_out_valid));
            vlSelf->fft2d_core__DOT__f_out_ready = 
                (2U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
            vlSelf->fft2d_core__DOT__recv_fire = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_out_valid) 
                                                     >> 1U));
            vlSelf->fft2d_core__DOT__f_out_ready = 
                (4U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
            vlSelf->fft2d_core__DOT__recv_fire = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_out_valid) 
                                                     >> 2U));
            vlSelf->fft2d_core__DOT__f_out_ready = 
                (8U | (IData)(vlSelf->fft2d_core__DOT__f_out_ready));
            vlSelf->fft2d_core__DOT__recv_fire = ((IData)(vlSelf->fft2d_core__DOT__recv_fire) 
                                                  & ((IData)(fft2d_core__DOT__f_out_valid) 
                                                     >> 3U));
        }
    } else if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        vlSelf->fft2d_core__DOT__f_in_valid = (1U | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
        vlSelf->fft2d_core__DOT__f_in_re[0U] = vlSelf->fft2d_core__DOT__mem0_re
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r 
                    = vlSelf->fft2d_core__DOT__batch_base;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout))];
        vlSelf->fft2d_core__DOT__feed_fire = (1U & (IData)(fft2d_core__DOT__f_in_ready));
        vlSelf->fft2d_core__DOT__f_in_im[0U] = vlSelf->fft2d_core__DOT__mem0_im
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r 
                    = vlSelf->fft2d_core__DOT__batch_base;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_valid = (2U | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
        vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                              & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                 >> 1U));
        vlSelf->fft2d_core__DOT__f_in_re[1U] = vlSelf->fft2d_core__DOT__mem0_re
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r 
                    = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_valid = (4U | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
        vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                              & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                 >> 2U));
        vlSelf->fft2d_core__DOT__f_in_im[1U] = vlSelf->fft2d_core__DOT__mem0_im
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r 
                    = (0xffU & ((IData)(1U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_valid = (8U | (IData)(vlSelf->fft2d_core__DOT__f_in_valid));
        vlSelf->fft2d_core__DOT__feed_fire = ((IData)(vlSelf->fft2d_core__DOT__feed_fire) 
                                              & ((IData)(fft2d_core__DOT__f_in_ready) 
                                                 >> 3U));
        vlSelf->fft2d_core__DOT__f_in_re[2U] = vlSelf->fft2d_core__DOT__mem0_re
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r 
                    = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_im[2U] = vlSelf->fft2d_core__DOT__mem0_im
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r 
                    = (0xffU & ((IData)(2U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_re[3U] = vlSelf->fft2d_core__DOT__mem0_re
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r 
                    = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_im[3U] = vlSelf->fft2d_core__DOT__mem0_im
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r 
                    = (0xffU & ((IData)(3U) + (IData)(vlSelf->fft2d_core__DOT__batch_base)));
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout))];
    }
}

void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);

void Vfft2d_core___024root___eval_nba(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d));
        Vfft2d_core___024root___nba_sequent__TOP__0(vlSelf);
    }
}
