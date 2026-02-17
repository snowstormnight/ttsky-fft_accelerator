// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft1d_core.h for the primary calling header

#include "Vfft1d_core__pch.h"
#include "Vfft1d_core___024root.h"

void Vfft1d_core___024root___eval_act(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_act\n"); );
}

extern const VlUnpacked<SData/*15:0*/, 16> Vfft1d_core__ConstPool__TABLE_hd86c15f4_0;
extern const VlUnpacked<SData/*15:0*/, 16> Vfft1d_core__ConstPool__TABLE_h350e8472_0;

VL_INLINE_OPT void Vfft1d_core___024root___nba_sequent__TOP__0(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___nba_sequent__TOP__0\n"); );
    // Init
    SData/*15:0*/ fft1d_core__DOT__u_bfly__DOT__mul_a;
    fft1d_core__DOT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ fft1d_core__DOT__u_bfly__DOT__mul_b;
    fft1d_core__DOT__u_bfly__DOT__mul_b = 0;
    SData/*15:0*/ __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__Vfuncout;
    __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x;
    __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x = 0;
    SData/*15:0*/ __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__Vfuncout;
    __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x;
    __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x = 0;
    CData/*3:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*0:0*/ __Vdly__fft1d_core__DOT__ctrl_start;
    __Vdly__fft1d_core__DOT__ctrl_start = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_re__v0;
    __Vdlyvdim0__fft1d_core__DOT__mem_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_re__v0;
    __Vdlyvval__fft1d_core__DOT__mem_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__fft1d_core__DOT__mem_re__v0;
    __Vdlyvset__fft1d_core__DOT__mem_re__v0 = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_im__v0;
    __Vdlyvdim0__fft1d_core__DOT__mem_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_im__v0;
    __Vdlyvval__fft1d_core__DOT__mem_im__v0 = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_re__v1;
    __Vdlyvdim0__fft1d_core__DOT__mem_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_re__v1;
    __Vdlyvval__fft1d_core__DOT__mem_re__v1 = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_im__v1;
    __Vdlyvdim0__fft1d_core__DOT__mem_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_im__v1;
    __Vdlyvval__fft1d_core__DOT__mem_im__v1 = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_re__v2;
    __Vdlyvdim0__fft1d_core__DOT__mem_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_re__v2;
    __Vdlyvval__fft1d_core__DOT__mem_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__fft1d_core__DOT__mem_re__v2;
    __Vdlyvset__fft1d_core__DOT__mem_re__v2 = 0;
    CData/*3:0*/ __Vdlyvdim0__fft1d_core__DOT__mem_im__v2;
    __Vdlyvdim0__fft1d_core__DOT__mem_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__fft1d_core__DOT__mem_im__v2;
    __Vdlyvval__fft1d_core__DOT__mem_im__v2 = 0;
    CData/*3:0*/ __Vdly__fft1d_core__DOT__load_ptr;
    __Vdly__fft1d_core__DOT__load_ptr = 0;
    CData/*1:0*/ __Vdly__fft1d_core__DOT__state;
    __Vdly__fft1d_core__DOT__state = 0;
    CData/*3:0*/ __Vdly__fft1d_core__DOT__out_ptr;
    __Vdly__fft1d_core__DOT__out_ptr = 0;
    CData/*0:0*/ __Vdly__fft1d_core__DOT__bfly_start;
    __Vdly__fft1d_core__DOT__bfly_start = 0;
    CData/*0:0*/ __Vdly__fft1d_core__DOT__u_ctrl__DOT__running;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__running = 0;
    CData/*0:0*/ __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = 0;
    CData/*1:0*/ __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage = 0;
    CData/*3:0*/ __Vdly__fft1d_core__DOT__u_ctrl__DOT__j;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__j = 0;
    CData/*3:0*/ __Vdly__fft1d_core__DOT__u_ctrl__DOT__base;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__base = 0;
    CData/*2:0*/ __Vdly__fft1d_core__DOT__u_bfly__DOT__state;
    __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 0;
    // Body
    __Vdly__fft1d_core__DOT__u_bfly__DOT__state = vlSelf->fft1d_core__DOT__u_bfly__DOT__state;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = vlSelf->fft1d_core__DOT__u_ctrl__DOT__waiting;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__running = vlSelf->fft1d_core__DOT__u_ctrl__DOT__running;
    __Vdly__fft1d_core__DOT__bfly_start = vlSelf->fft1d_core__DOT__bfly_start;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage = vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__base = vlSelf->fft1d_core__DOT__u_ctrl__DOT__base;
    __Vdly__fft1d_core__DOT__u_ctrl__DOT__j = vlSelf->fft1d_core__DOT__u_ctrl__DOT__j;
    __Vdly__fft1d_core__DOT__load_ptr = vlSelf->fft1d_core__DOT__load_ptr;
    __Vdly__fft1d_core__DOT__ctrl_start = vlSelf->fft1d_core__DOT__ctrl_start;
    __Vdly__fft1d_core__DOT__out_ptr = vlSelf->fft1d_core__DOT__out_ptr;
    __Vdly__fft1d_core__DOT__state = vlSelf->fft1d_core__DOT__state;
    __Vdlyvset__fft1d_core__DOT__mem_re__v0 = 0U;
    __Vdlyvset__fft1d_core__DOT__mem_re__v2 = 0U;
    if (vlSelf->rst_n) {
        __Vdly__fft1d_core__DOT__ctrl_start = 0U;
        if (vlSelf->fft1d_core__DOT__bfly_done) {
            __Vdlyvval__fft1d_core__DOT__mem_re__v0 
                = vlSelf->fft1d_core__DOT__bfly_a_re;
            __Vdlyvset__fft1d_core__DOT__mem_re__v0 = 1U;
            __Vdlyvdim0__fft1d_core__DOT__mem_re__v0 
                = vlSelf->fft1d_core__DOT__idx_a;
            __Vdlyvval__fft1d_core__DOT__mem_im__v0 
                = vlSelf->fft1d_core__DOT__bfly_a_im;
            __Vdlyvdim0__fft1d_core__DOT__mem_im__v0 
                = vlSelf->fft1d_core__DOT__idx_a;
            __Vdlyvval__fft1d_core__DOT__mem_re__v1 
                = vlSelf->fft1d_core__DOT__bfly_b_re;
            __Vdlyvdim0__fft1d_core__DOT__mem_re__v1 
                = vlSelf->fft1d_core__DOT__idx_b;
            __Vdlyvval__fft1d_core__DOT__mem_im__v1 
                = vlSelf->fft1d_core__DOT__bfly_b_im;
            __Vdlyvdim0__fft1d_core__DOT__mem_im__v1 
                = vlSelf->fft1d_core__DOT__idx_b;
        }
        if ((0U == (IData)(vlSelf->fft1d_core__DOT__state))) {
            if (((IData)(vlSelf->in_valid) & (IData)(vlSelf->in_ready))) {
                __Vdlyvval__fft1d_core__DOT__mem_re__v2 
                    = vlSelf->in_re;
                __Vdlyvset__fft1d_core__DOT__mem_re__v2 = 1U;
                __Vdlyvdim0__fft1d_core__DOT__mem_re__v2 
                    = vlSelf->fft1d_core__DOT__load_ptr;
                __Vdlyvval__fft1d_core__DOT__mem_im__v2 
                    = vlSelf->in_im;
                __Vdlyvdim0__fft1d_core__DOT__mem_im__v2 
                    = vlSelf->fft1d_core__DOT__load_ptr;
                if ((0xfU == (IData)(vlSelf->fft1d_core__DOT__load_ptr))) {
                    __Vdly__fft1d_core__DOT__load_ptr = 0U;
                    __Vdly__fft1d_core__DOT__ctrl_start = 1U;
                    __Vdly__fft1d_core__DOT__state = 1U;
                } else {
                    __Vdly__fft1d_core__DOT__load_ptr 
                        = (0xfU & ((IData)(1U) + (IData)(vlSelf->fft1d_core__DOT__load_ptr)));
                }
            }
        } else if ((1U == (IData)(vlSelf->fft1d_core__DOT__state))) {
            if (vlSelf->fft1d_core__DOT__ctrl_done) {
                __Vdly__fft1d_core__DOT__out_ptr = 0U;
                __Vdly__fft1d_core__DOT__state = 2U;
            }
        } else if ((2U == (IData)(vlSelf->fft1d_core__DOT__state))) {
            if (((IData)(vlSelf->out_valid) & (IData)(vlSelf->out_ready))) {
                if ((0xfU == (IData)(vlSelf->fft1d_core__DOT__out_ptr))) {
                    __Vdly__fft1d_core__DOT__out_ptr = 0U;
                    __Vdly__fft1d_core__DOT__state = 0U;
                } else {
                    __Vdly__fft1d_core__DOT__out_ptr 
                        = (0xfU & ((IData)(1U) + (IData)(vlSelf->fft1d_core__DOT__out_ptr)));
                }
            }
        } else {
            __Vdly__fft1d_core__DOT__state = 0U;
        }
        __Vdly__fft1d_core__DOT__bfly_start = 0U;
        vlSelf->fft1d_core__DOT__ctrl_done = 0U;
        if (((IData)(vlSelf->fft1d_core__DOT__ctrl_start) 
             & (~ (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__running)))) {
            __Vdly__fft1d_core__DOT__u_ctrl__DOT__running = 1U;
            __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = 0U;
            __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage = 0U;
            __Vdly__fft1d_core__DOT__u_ctrl__DOT__j = 0U;
            __Vdly__fft1d_core__DOT__u_ctrl__DOT__base = 0U;
        } else if (vlSelf->fft1d_core__DOT__u_ctrl__DOT__running) {
            if (vlSelf->fft1d_core__DOT__u_ctrl__DOT__waiting) {
                if (vlSelf->fft1d_core__DOT__bfly_done) {
                    __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = 0U;
                    if (((0x1fU & ((IData)(1U) + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j))) 
                         < (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__half))) {
                        __Vdly__fft1d_core__DOT__u_ctrl__DOT__j 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j)));
                    } else {
                        __Vdly__fft1d_core__DOT__u_ctrl__DOT__j = 0U;
                        if ((0x10U > (0x1fU & ((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__base) 
                                               + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__span))))) {
                            __Vdly__fft1d_core__DOT__u_ctrl__DOT__base 
                                = (0xfU & ((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__base) 
                                           + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__span)));
                        } else {
                            __Vdly__fft1d_core__DOT__u_ctrl__DOT__base = 0U;
                            if ((4U > (7U & ((IData)(1U) 
                                             + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage))))) {
                                __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage 
                                    = (3U & ((IData)(1U) 
                                             + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage)));
                            } else {
                                __Vdly__fft1d_core__DOT__u_ctrl__DOT__running = 0U;
                                vlSelf->fft1d_core__DOT__ctrl_done = 1U;
                            }
                        }
                    }
                }
            } else {
                __Vdly__fft1d_core__DOT__bfly_start = 1U;
                __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = 1U;
            }
        }
        vlSelf->fft1d_core__DOT__bfly_done = 0U;
        if ((4U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
            if ((2U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
                __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
                __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 0U;
            } else {
                __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x 
                    = VL_SHIFTRS_III(32,32,32, (vlSelf->fft1d_core__DOT__u_bfly__DOT__m1_r 
                                                - vlSelf->fft1d_core__DOT__u_bfly__DOT__m2_r), 0xfU);
                __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x 
                    = VL_SHIFTRS_III(32,32,32, (vlSelf->fft1d_core__DOT__u_bfly__DOT__m3_r 
                                                + vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p), 0xfU);
                __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__x)));
                __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__x)));
                vlSelf->fft1d_core__DOT__bfly_a_re 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_re_r;
                vlSelf->fft1d_core__DOT__bfly_a_im 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_im_r;
                vlSelf->fft1d_core__DOT__bfly_b_re 
                    = __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__0__Vfuncout;
                vlSelf->fft1d_core__DOT__bfly_b_im 
                    = __Vfunc_fft1d_core__DOT__u_bfly__DOT__sat16__1__Vfuncout;
                vlSelf->fft1d_core__DOT__bfly_done = 1U;
                __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 0U;
            }
        } else if ((2U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
            if ((1U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
                vlSelf->fft1d_core__DOT__u_bfly__DOT__m3_r 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p;
                __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 4U;
            } else {
                vlSelf->fft1d_core__DOT__u_bfly__DOT__m2_r 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p;
                __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
            vlSelf->fft1d_core__DOT__u_bfly__DOT__m1_r 
                = vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p;
            __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 2U;
        } else if (vlSelf->fft1d_core__DOT__bfly_start) {
            vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_re_r 
                = (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                            (0x1ffffU 
                                             & (((0x10000U 
                                                  & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re)) 
                                                + (
                                                   (0x10000U 
                                                    & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re) 
                                                       << 1U)) 
                                                   | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_im_r 
                = (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                            (0x1ffffU 
                                             & (((0x10000U 
                                                  & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im)) 
                                                + (
                                                   (0x10000U 
                                                    & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im) 
                                                       << 1U)) 
                                                   | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_re_r 
                = (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                            (0x1ffffU 
                                             & (((0x10000U 
                                                  & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re)) 
                                                - (
                                                   (0x10000U 
                                                    & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re) 
                                                       << 1U)) 
                                                   | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_im_r 
                = (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                            (0x1ffffU 
                                             & (((0x10000U 
                                                  & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im)) 
                                                - (
                                                   (0x10000U 
                                                    & ((IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im) 
                                                       << 1U)) 
                                                   | (IData)(vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->fft1d_core__DOT__u_bfly__DOT__w_re_r 
                = vlSelf->fft1d_core__DOT__w_re;
            vlSelf->fft1d_core__DOT__u_bfly__DOT__w_im_r 
                = vlSelf->fft1d_core__DOT__w_im;
            __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 1U;
        }
    } else {
        __Vdly__fft1d_core__DOT__load_ptr = 0U;
        __Vdly__fft1d_core__DOT__state = 0U;
        __Vdly__fft1d_core__DOT__out_ptr = 0U;
        __Vdly__fft1d_core__DOT__ctrl_start = 0U;
        __Vdly__fft1d_core__DOT__u_ctrl__DOT__running = 0U;
        __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting = 0U;
        __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage = 0U;
        __Vdly__fft1d_core__DOT__u_ctrl__DOT__j = 0U;
        __Vdly__fft1d_core__DOT__u_ctrl__DOT__base = 0U;
        __Vdly__fft1d_core__DOT__bfly_start = 0U;
        vlSelf->fft1d_core__DOT__ctrl_done = 0U;
        __Vdly__fft1d_core__DOT__u_bfly__DOT__state = 0U;
        vlSelf->fft1d_core__DOT__bfly_done = 0U;
        vlSelf->fft1d_core__DOT__bfly_a_re = 0U;
        vlSelf->fft1d_core__DOT__bfly_a_im = 0U;
        vlSelf->fft1d_core__DOT__bfly_b_re = 0U;
        vlSelf->fft1d_core__DOT__bfly_b_im = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_re_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_im_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_re_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_im_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__w_re_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__w_im_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__m1_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__m2_r = 0U;
        vlSelf->fft1d_core__DOT__u_bfly__DOT__m3_r = 0U;
    }
    vlSelf->fft1d_core__DOT__load_ptr = __Vdly__fft1d_core__DOT__load_ptr;
    vlSelf->fft1d_core__DOT__out_ptr = __Vdly__fft1d_core__DOT__out_ptr;
    vlSelf->fft1d_core__DOT__state = __Vdly__fft1d_core__DOT__state;
    if (__Vdlyvset__fft1d_core__DOT__mem_re__v0) {
        vlSelf->fft1d_core__DOT__mem_im[__Vdlyvdim0__fft1d_core__DOT__mem_im__v0] 
            = __Vdlyvval__fft1d_core__DOT__mem_im__v0;
        vlSelf->fft1d_core__DOT__mem_im[__Vdlyvdim0__fft1d_core__DOT__mem_im__v1] 
            = __Vdlyvval__fft1d_core__DOT__mem_im__v1;
        vlSelf->fft1d_core__DOT__mem_re[__Vdlyvdim0__fft1d_core__DOT__mem_re__v0] 
            = __Vdlyvval__fft1d_core__DOT__mem_re__v0;
        vlSelf->fft1d_core__DOT__mem_re[__Vdlyvdim0__fft1d_core__DOT__mem_re__v1] 
            = __Vdlyvval__fft1d_core__DOT__mem_re__v1;
    }
    if (__Vdlyvset__fft1d_core__DOT__mem_re__v2) {
        vlSelf->fft1d_core__DOT__mem_im[__Vdlyvdim0__fft1d_core__DOT__mem_im__v2] 
            = __Vdlyvval__fft1d_core__DOT__mem_im__v2;
        vlSelf->fft1d_core__DOT__mem_re[__Vdlyvdim0__fft1d_core__DOT__mem_re__v2] 
            = __Vdlyvval__fft1d_core__DOT__mem_re__v2;
    }
    vlSelf->in_ready = (0U == (IData)(vlSelf->fft1d_core__DOT__state));
    if ((2U == (IData)(vlSelf->fft1d_core__DOT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = vlSelf->fft1d_core__DOT__mem_im
            [vlSelf->fft1d_core__DOT__out_ptr];
        vlSelf->out_re = vlSelf->fft1d_core__DOT__mem_re
            [vlSelf->fft1d_core__DOT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = 0U;
    }
    vlSelf->fft1d_core__DOT__ctrl_start = __Vdly__fft1d_core__DOT__ctrl_start;
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__running = __Vdly__fft1d_core__DOT__u_ctrl__DOT__running;
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__waiting = __Vdly__fft1d_core__DOT__u_ctrl__DOT__waiting;
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage = __Vdly__fft1d_core__DOT__u_ctrl__DOT__stage;
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__base = __Vdly__fft1d_core__DOT__u_ctrl__DOT__base;
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__j = __Vdly__fft1d_core__DOT__u_ctrl__DOT__j;
    vlSelf->fft1d_core__DOT__idx_a = (0xfU & ((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__base) 
                                              + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j)));
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__span = (0x1fU 
                                                  & (0x10U 
                                                     >> (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage)));
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__half = (0x1fU 
                                                  & VL_SHIFTR_III(5,5,32, (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__span), 1U));
    vlSelf->fft1d_core__DOT__idx_b = (0xfU & (((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__base) 
                                               + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j)) 
                                              + (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__half)));
    vlSelf->fft1d_core__DOT__bfly_start = __Vdly__fft1d_core__DOT__bfly_start;
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re 
        = vlSelf->fft1d_core__DOT__mem_re[vlSelf->fft1d_core__DOT__idx_a];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re 
        = vlSelf->fft1d_core__DOT__mem_re[vlSelf->fft1d_core__DOT__idx_b];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im 
        = vlSelf->fft1d_core__DOT__mem_im[vlSelf->fft1d_core__DOT__idx_a];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im 
        = vlSelf->fft1d_core__DOT__mem_im[vlSelf->fft1d_core__DOT__idx_b];
    __Vtableidx1 = (0xfU & ((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j) 
                            << (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage)));
    vlSelf->fft1d_core__DOT__w_re = Vfft1d_core__ConstPool__TABLE_hd86c15f4_0
        [__Vtableidx1];
    vlSelf->fft1d_core__DOT__w_im = Vfft1d_core__ConstPool__TABLE_h350e8472_0
        [__Vtableidx1];
    vlSelf->fft1d_core__DOT__u_bfly__DOT__state = __Vdly__fft1d_core__DOT__u_bfly__DOT__state;
    fft1d_core__DOT__u_bfly__DOT__mul_a = 0U;
    fft1d_core__DOT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state)))) {
                fft1d_core__DOT__u_bfly__DOT__mul_a 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_im_r;
                fft1d_core__DOT__u_bfly__DOT__mul_b 
                    = vlSelf->fft1d_core__DOT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
        fft1d_core__DOT__u_bfly__DOT__mul_a = ((1U 
                                                & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))
                                                ? (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_re_r)
                                                : (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_im_r));
        fft1d_core__DOT__u_bfly__DOT__mul_b = vlSelf->fft1d_core__DOT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->fft1d_core__DOT__u_bfly__DOT__state))) {
        fft1d_core__DOT__u_bfly__DOT__mul_a = vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_re_r;
        fft1d_core__DOT__u_bfly__DOT__mul_b = vlSelf->fft1d_core__DOT__u_bfly__DOT__w_re_r;
    }
    vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                              VL_EXTENDS_II(32,16, (IData)(fft1d_core__DOT__u_bfly__DOT__mul_a)), 
                                                              VL_EXTENDS_II(32,16, (IData)(fft1d_core__DOT__u_bfly__DOT__mul_b)));
}

void Vfft1d_core___024root___eval_nba(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vfft1d_core___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vfft1d_core___024root___eval_triggers__act(Vfft1d_core___024root* vlSelf);

bool Vfft1d_core___024root___eval_phase__act(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vfft1d_core___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vfft1d_core___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vfft1d_core___024root___eval_phase__nba(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vfft1d_core___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__nba(Vfft1d_core___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__act(Vfft1d_core___024root* vlSelf);
#endif  // VL_DEBUG

void Vfft1d_core___024root___eval(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vfft1d_core___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("/home/george/project/ASIC_Hack/src/fft/fft1d_core.sv", 5, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vfft1d_core___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("/home/george/project/ASIC_Hack/src/fft/fft1d_core.sv", 5, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vfft1d_core___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vfft1d_core___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vfft1d_core___024root___eval_debug_assertions(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((vlSelf->in_valid & 0xfeU))) {
        Verilated::overWidthError("in_valid");}
    if (VL_UNLIKELY((vlSelf->out_ready & 0xfeU))) {
        Verilated::overWidthError("out_ready");}
}
#endif  // VL_DEBUG
