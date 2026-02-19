// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core__Syms.h"
#include "Vfft2d_core_fft1d_core__N100_L8.h"

extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_hd83d655a_0;
extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_h6c2add80_0;

VL_INLINE_OPT void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__0__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__0__x;
    __Vfunc_u_bfly__DOT__sat16__0__x = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__1__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__1__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__1__x;
    __Vfunc_u_bfly__DOT__sat16__1__x = 0;
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*0:0*/ __Vdly__ctrl_start;
    __Vdly__ctrl_start = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v0;
    __Vdlyvdim0__mem_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v0;
    __Vdlyvval__mem_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v0;
    __Vdlyvset__mem_re__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v0;
    __Vdlyvdim0__mem_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v0;
    __Vdlyvval__mem_im__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v1;
    __Vdlyvdim0__mem_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v1;
    __Vdlyvval__mem_re__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v1;
    __Vdlyvdim0__mem_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v1;
    __Vdlyvval__mem_im__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v2;
    __Vdlyvdim0__mem_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v2;
    __Vdlyvval__mem_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v2;
    __Vdlyvset__mem_re__v2 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v2;
    __Vdlyvdim0__mem_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v2;
    __Vdlyvval__mem_im__v2 = 0;
    CData/*7:0*/ __Vdly__load_ptr;
    __Vdly__load_ptr = 0;
    CData/*1:0*/ __Vdly__state;
    __Vdly__state = 0;
    CData/*7:0*/ __Vdly__out_ptr;
    __Vdly__out_ptr = 0;
    CData/*0:0*/ __Vdly__bfly_start;
    __Vdly__bfly_start = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__running;
    __Vdly__u_ctrl__DOT__running = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__waiting = 0;
    CData/*2:0*/ __Vdly__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__stage = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__j;
    __Vdly__u_ctrl__DOT__j = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__base = 0;
    CData/*2:0*/ __Vdly__u_bfly__DOT__state;
    __Vdly__u_bfly__DOT__state = 0;
    // Body
    __Vdly__u_bfly__DOT__state = vlSelf->__PVT__u_bfly__DOT__state;
    __Vdly__u_ctrl__DOT__waiting = vlSelf->__PVT__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__running = vlSelf->__PVT__u_ctrl__DOT__running;
    __Vdly__bfly_start = vlSelf->__PVT__bfly_start;
    __Vdly__u_ctrl__DOT__stage = vlSelf->__PVT__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__base = vlSelf->__PVT__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__j = vlSelf->__PVT__u_ctrl__DOT__j;
    __Vdly__load_ptr = vlSelf->__PVT__load_ptr;
    __Vdly__ctrl_start = vlSelf->__PVT__ctrl_start;
    __Vdly__out_ptr = vlSelf->__PVT__out_ptr;
    __Vdlyvset__mem_re__v0 = 0U;
    __Vdlyvset__mem_re__v2 = 0U;
    __Vdly__state = vlSelf->__PVT__state;
    if (vlSymsp->TOP.rst_n) {
        __Vdly__ctrl_start = 0U;
        if (vlSelf->__PVT__bfly_done) {
            __Vdlyvval__mem_re__v0 = vlSelf->__PVT__bfly_a_re;
            __Vdlyvset__mem_re__v0 = 1U;
            __Vdlyvdim0__mem_re__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_im__v0 = vlSelf->__PVT__bfly_a_im;
            __Vdlyvdim0__mem_im__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_re__v1 = vlSelf->__PVT__bfly_b_re;
            __Vdlyvdim0__mem_re__v1 = vlSelf->__PVT__idx_b;
            __Vdlyvval__mem_im__v1 = vlSelf->__PVT__bfly_b_im;
            __Vdlyvdim0__mem_im__v1 = vlSelf->__PVT__idx_b;
        }
        if ((0U == (IData)(vlSelf->__PVT__state))) {
            if (((IData)(vlSymsp->TOP.fft2d_core__DOT__f_in_valid) 
                 & (IData)(vlSelf->in_ready))) {
                __Vdlyvval__mem_re__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_re
                    [0U];
                __Vdlyvset__mem_re__v2 = 1U;
                __Vdlyvdim0__mem_re__v2 = vlSelf->__PVT__load_ptr;
                __Vdlyvval__mem_im__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_im
                    [0U];
                __Vdlyvdim0__mem_im__v2 = vlSelf->__PVT__load_ptr;
                if ((0xffU == (IData)(vlSelf->__PVT__load_ptr))) {
                    __Vdly__load_ptr = 0U;
                    __Vdly__ctrl_start = 1U;
                    __Vdly__state = 1U;
                } else {
                    __Vdly__load_ptr = (0xffU & ((IData)(1U) 
                                                 + (IData)(vlSelf->__PVT__load_ptr)));
                }
            }
        } else if ((1U == (IData)(vlSelf->__PVT__state))) {
            if (vlSelf->__PVT__ctrl_done) {
                __Vdly__out_ptr = 0U;
                __Vdly__state = 2U;
            }
        } else if ((2U == (IData)(vlSelf->__PVT__state))) {
            if (((IData)(vlSelf->out_valid) & (IData)(vlSymsp->TOP.fft2d_core__DOT__f_out_ready))) {
                if ((0xffU == (IData)(vlSelf->__PVT__out_ptr))) {
                    __Vdly__out_ptr = 0U;
                    __Vdly__state = 0U;
                } else {
                    __Vdly__out_ptr = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->__PVT__out_ptr)));
                }
            }
        } else {
            __Vdly__state = 0U;
        }
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        if (((IData)(vlSelf->__PVT__ctrl_start) & (~ (IData)(vlSelf->__PVT__u_ctrl__DOT__running)))) {
            __Vdly__u_ctrl__DOT__running = 1U;
            __Vdly__u_ctrl__DOT__waiting = 0U;
            __Vdly__u_ctrl__DOT__stage = 0U;
            __Vdly__u_ctrl__DOT__j = 0U;
            __Vdly__u_ctrl__DOT__base = 0U;
        } else if (vlSelf->__PVT__u_ctrl__DOT__running) {
            if (vlSelf->__PVT__u_ctrl__DOT__waiting) {
                if (vlSelf->__PVT__bfly_done) {
                    __Vdly__u_ctrl__DOT__waiting = 0U;
                    if (((0x1ffU & ((IData)(1U) + (IData)(vlSelf->__PVT__u_ctrl__DOT__j))) 
                         < (IData)(vlSelf->__PVT__u_ctrl__DOT__half))) {
                        __Vdly__u_ctrl__DOT__j = (0xffU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
                    } else {
                        __Vdly__u_ctrl__DOT__j = 0U;
                        if ((0x100U > (0x1ffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                                 + (IData)(vlSelf->__PVT__u_ctrl__DOT__span))))) {
                            __Vdly__u_ctrl__DOT__base 
                                = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                            + (IData)(vlSelf->__PVT__u_ctrl__DOT__span)));
                        } else {
                            __Vdly__u_ctrl__DOT__base = 0U;
                            if ((8U > (0xfU & ((IData)(1U) 
                                               + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage))))) {
                                __Vdly__u_ctrl__DOT__stage 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
                            } else {
                                __Vdly__u_ctrl__DOT__running = 0U;
                                vlSelf->__PVT__ctrl_done = 1U;
                            }
                        }
                    }
                }
            } else {
                __Vdly__bfly_start = 1U;
                __Vdly__u_ctrl__DOT__waiting = 1U;
            }
        }
        vlSelf->__PVT__bfly_done = 0U;
        if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else {
                __Vfunc_u_bfly__DOT__sat16__0__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m1_r 
                                              - vlSelf->__PVT__u_bfly__DOT__m2_r), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__1__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m3_r 
                                              + vlSelf->__PVT__u_bfly__DOT__mul_p), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__0__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__0__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__0__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__0__x)));
                __Vfunc_u_bfly__DOT__sat16__1__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__1__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__1__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__1__x)));
                vlSelf->__PVT__bfly_a_re = vlSelf->__PVT__u_bfly__DOT__sum_re_r;
                vlSelf->__PVT__bfly_a_im = vlSelf->__PVT__u_bfly__DOT__sum_im_r;
                vlSelf->__PVT__bfly_b_re = __Vfunc_u_bfly__DOT__sat16__0__Vfuncout;
                vlSelf->__PVT__bfly_b_im = __Vfunc_u_bfly__DOT__sat16__1__Vfuncout;
                vlSelf->__PVT__bfly_done = 1U;
                __Vdly__u_bfly__DOT__state = 0U;
            }
        } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                vlSelf->__PVT__u_bfly__DOT__m3_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 4U;
            } else {
                vlSelf->__PVT__u_bfly__DOT__m2_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            vlSelf->__PVT__u_bfly__DOT__m1_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
            __Vdly__u_bfly__DOT__state = 2U;
        } else if (vlSelf->__PVT__bfly_start) {
            vlSelf->__PVT__u_bfly__DOT__sum_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__sum_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__w_re_r = vlSelf->__PVT__w_re;
            vlSelf->__PVT__u_bfly__DOT__w_im_r = vlSelf->__PVT__w_im;
            __Vdly__u_bfly__DOT__state = 1U;
        }
    } else {
        __Vdly__load_ptr = 0U;
        __Vdly__state = 0U;
        __Vdly__out_ptr = 0U;
        __Vdly__ctrl_start = 0U;
        __Vdly__u_ctrl__DOT__running = 0U;
        __Vdly__u_ctrl__DOT__waiting = 0U;
        __Vdly__u_ctrl__DOT__stage = 0U;
        __Vdly__u_ctrl__DOT__j = 0U;
        __Vdly__u_ctrl__DOT__base = 0U;
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        __Vdly__u_bfly__DOT__state = 0U;
        vlSelf->__PVT__bfly_done = 0U;
        vlSelf->__PVT__bfly_a_re = 0U;
        vlSelf->__PVT__bfly_a_im = 0U;
        vlSelf->__PVT__bfly_b_re = 0U;
        vlSelf->__PVT__bfly_b_im = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m1_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m2_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m3_r = 0U;
    }
    vlSelf->__PVT__load_ptr = __Vdly__load_ptr;
    vlSelf->__PVT__out_ptr = __Vdly__out_ptr;
    if (__Vdlyvset__mem_re__v0) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v0] 
            = __Vdlyvval__mem_im__v0;
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v1] 
            = __Vdlyvval__mem_im__v1;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v0] 
            = __Vdlyvval__mem_re__v0;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v1] 
            = __Vdlyvval__mem_re__v1;
    }
    if (__Vdlyvset__mem_re__v2) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v2] 
            = __Vdlyvval__mem_im__v2;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v2] 
            = __Vdlyvval__mem_re__v2;
    }
    vlSelf->__PVT__state = __Vdly__state;
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
    }
    vlSelf->__PVT__ctrl_start = __Vdly__ctrl_start;
    vlSelf->__PVT__u_ctrl__DOT__running = __Vdly__u_ctrl__DOT__running;
    vlSelf->__PVT__u_ctrl__DOT__waiting = __Vdly__u_ctrl__DOT__waiting;
    vlSelf->__PVT__u_ctrl__DOT__stage = __Vdly__u_ctrl__DOT__stage;
    vlSelf->__PVT__u_ctrl__DOT__base = __Vdly__u_ctrl__DOT__base;
    vlSelf->__PVT__u_ctrl__DOT__j = __Vdly__u_ctrl__DOT__j;
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__bfly_start = __Vdly__bfly_start;
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
    __Vtableidx1 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx1];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx1];
    vlSelf->__PVT__u_bfly__DOT__state = __Vdly__u_bfly__DOT__state;
    __PVT__u_bfly__DOT__mul_a = 0U;
    __PVT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->__PVT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->__PVT__u_bfly__DOT__state)))) {
                __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_im_r;
                __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))
                                      ? (IData)(vlSelf->__PVT__u_bfly__DOT__diff_re_r)
                                      : (IData)(vlSelf->__PVT__u_bfly__DOT__diff_im_r));
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_re_r;
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
    }
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
}

VL_INLINE_OPT void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__2__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__2__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__2__x;
    __Vfunc_u_bfly__DOT__sat16__2__x = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__3__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__3__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__3__x;
    __Vfunc_u_bfly__DOT__sat16__3__x = 0;
    CData/*7:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*0:0*/ __Vdly__ctrl_start;
    __Vdly__ctrl_start = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v0;
    __Vdlyvdim0__mem_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v0;
    __Vdlyvval__mem_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v0;
    __Vdlyvset__mem_re__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v0;
    __Vdlyvdim0__mem_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v0;
    __Vdlyvval__mem_im__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v1;
    __Vdlyvdim0__mem_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v1;
    __Vdlyvval__mem_re__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v1;
    __Vdlyvdim0__mem_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v1;
    __Vdlyvval__mem_im__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v2;
    __Vdlyvdim0__mem_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v2;
    __Vdlyvval__mem_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v2;
    __Vdlyvset__mem_re__v2 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v2;
    __Vdlyvdim0__mem_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v2;
    __Vdlyvval__mem_im__v2 = 0;
    CData/*7:0*/ __Vdly__load_ptr;
    __Vdly__load_ptr = 0;
    CData/*1:0*/ __Vdly__state;
    __Vdly__state = 0;
    CData/*7:0*/ __Vdly__out_ptr;
    __Vdly__out_ptr = 0;
    CData/*0:0*/ __Vdly__bfly_start;
    __Vdly__bfly_start = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__running;
    __Vdly__u_ctrl__DOT__running = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__waiting = 0;
    CData/*2:0*/ __Vdly__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__stage = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__j;
    __Vdly__u_ctrl__DOT__j = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__base = 0;
    CData/*2:0*/ __Vdly__u_bfly__DOT__state;
    __Vdly__u_bfly__DOT__state = 0;
    // Body
    __Vdly__u_bfly__DOT__state = vlSelf->__PVT__u_bfly__DOT__state;
    __Vdly__u_ctrl__DOT__waiting = vlSelf->__PVT__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__running = vlSelf->__PVT__u_ctrl__DOT__running;
    __Vdly__bfly_start = vlSelf->__PVT__bfly_start;
    __Vdly__u_ctrl__DOT__stage = vlSelf->__PVT__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__base = vlSelf->__PVT__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__j = vlSelf->__PVT__u_ctrl__DOT__j;
    __Vdly__load_ptr = vlSelf->__PVT__load_ptr;
    __Vdly__ctrl_start = vlSelf->__PVT__ctrl_start;
    __Vdly__out_ptr = vlSelf->__PVT__out_ptr;
    __Vdlyvset__mem_re__v0 = 0U;
    __Vdlyvset__mem_re__v2 = 0U;
    __Vdly__state = vlSelf->__PVT__state;
    if (vlSymsp->TOP.rst_n) {
        __Vdly__ctrl_start = 0U;
        if (vlSelf->__PVT__bfly_done) {
            __Vdlyvval__mem_re__v0 = vlSelf->__PVT__bfly_a_re;
            __Vdlyvset__mem_re__v0 = 1U;
            __Vdlyvdim0__mem_re__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_im__v0 = vlSelf->__PVT__bfly_a_im;
            __Vdlyvdim0__mem_im__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_re__v1 = vlSelf->__PVT__bfly_b_re;
            __Vdlyvdim0__mem_re__v1 = vlSelf->__PVT__idx_b;
            __Vdlyvval__mem_im__v1 = vlSelf->__PVT__bfly_b_im;
            __Vdlyvdim0__mem_im__v1 = vlSelf->__PVT__idx_b;
        }
        if ((0U == (IData)(vlSelf->__PVT__state))) {
            if ((((IData)(vlSymsp->TOP.fft2d_core__DOT__f_in_valid) 
                  >> 1U) & (IData)(vlSelf->in_ready))) {
                __Vdlyvval__mem_re__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_re
                    [1U];
                __Vdlyvset__mem_re__v2 = 1U;
                __Vdlyvdim0__mem_re__v2 = vlSelf->__PVT__load_ptr;
                __Vdlyvval__mem_im__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_im
                    [1U];
                __Vdlyvdim0__mem_im__v2 = vlSelf->__PVT__load_ptr;
                if ((0xffU == (IData)(vlSelf->__PVT__load_ptr))) {
                    __Vdly__load_ptr = 0U;
                    __Vdly__ctrl_start = 1U;
                    __Vdly__state = 1U;
                } else {
                    __Vdly__load_ptr = (0xffU & ((IData)(1U) 
                                                 + (IData)(vlSelf->__PVT__load_ptr)));
                }
            }
        } else if ((1U == (IData)(vlSelf->__PVT__state))) {
            if (vlSelf->__PVT__ctrl_done) {
                __Vdly__out_ptr = 0U;
                __Vdly__state = 2U;
            }
        } else if ((2U == (IData)(vlSelf->__PVT__state))) {
            if (((IData)(vlSelf->out_valid) & ((IData)(vlSymsp->TOP.fft2d_core__DOT__f_out_ready) 
                                               >> 1U))) {
                if ((0xffU == (IData)(vlSelf->__PVT__out_ptr))) {
                    __Vdly__out_ptr = 0U;
                    __Vdly__state = 0U;
                } else {
                    __Vdly__out_ptr = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->__PVT__out_ptr)));
                }
            }
        } else {
            __Vdly__state = 0U;
        }
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        if (((IData)(vlSelf->__PVT__ctrl_start) & (~ (IData)(vlSelf->__PVT__u_ctrl__DOT__running)))) {
            __Vdly__u_ctrl__DOT__running = 1U;
            __Vdly__u_ctrl__DOT__waiting = 0U;
            __Vdly__u_ctrl__DOT__stage = 0U;
            __Vdly__u_ctrl__DOT__j = 0U;
            __Vdly__u_ctrl__DOT__base = 0U;
        } else if (vlSelf->__PVT__u_ctrl__DOT__running) {
            if (vlSelf->__PVT__u_ctrl__DOT__waiting) {
                if (vlSelf->__PVT__bfly_done) {
                    __Vdly__u_ctrl__DOT__waiting = 0U;
                    if (((0x1ffU & ((IData)(1U) + (IData)(vlSelf->__PVT__u_ctrl__DOT__j))) 
                         < (IData)(vlSelf->__PVT__u_ctrl__DOT__half))) {
                        __Vdly__u_ctrl__DOT__j = (0xffU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
                    } else {
                        __Vdly__u_ctrl__DOT__j = 0U;
                        if ((0x100U > (0x1ffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                                 + (IData)(vlSelf->__PVT__u_ctrl__DOT__span))))) {
                            __Vdly__u_ctrl__DOT__base 
                                = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                            + (IData)(vlSelf->__PVT__u_ctrl__DOT__span)));
                        } else {
                            __Vdly__u_ctrl__DOT__base = 0U;
                            if ((8U > (0xfU & ((IData)(1U) 
                                               + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage))))) {
                                __Vdly__u_ctrl__DOT__stage 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
                            } else {
                                __Vdly__u_ctrl__DOT__running = 0U;
                                vlSelf->__PVT__ctrl_done = 1U;
                            }
                        }
                    }
                }
            } else {
                __Vdly__bfly_start = 1U;
                __Vdly__u_ctrl__DOT__waiting = 1U;
            }
        }
        vlSelf->__PVT__bfly_done = 0U;
        if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else {
                __Vfunc_u_bfly__DOT__sat16__2__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m1_r 
                                              - vlSelf->__PVT__u_bfly__DOT__m2_r), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__3__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m3_r 
                                              + vlSelf->__PVT__u_bfly__DOT__mul_p), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__2__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__2__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__2__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__2__x)));
                __Vfunc_u_bfly__DOT__sat16__3__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__3__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__3__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__3__x)));
                vlSelf->__PVT__bfly_a_re = vlSelf->__PVT__u_bfly__DOT__sum_re_r;
                vlSelf->__PVT__bfly_a_im = vlSelf->__PVT__u_bfly__DOT__sum_im_r;
                vlSelf->__PVT__bfly_b_re = __Vfunc_u_bfly__DOT__sat16__2__Vfuncout;
                vlSelf->__PVT__bfly_b_im = __Vfunc_u_bfly__DOT__sat16__3__Vfuncout;
                vlSelf->__PVT__bfly_done = 1U;
                __Vdly__u_bfly__DOT__state = 0U;
            }
        } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                vlSelf->__PVT__u_bfly__DOT__m3_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 4U;
            } else {
                vlSelf->__PVT__u_bfly__DOT__m2_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            vlSelf->__PVT__u_bfly__DOT__m1_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
            __Vdly__u_bfly__DOT__state = 2U;
        } else if (vlSelf->__PVT__bfly_start) {
            vlSelf->__PVT__u_bfly__DOT__sum_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__sum_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__w_re_r = vlSelf->__PVT__w_re;
            vlSelf->__PVT__u_bfly__DOT__w_im_r = vlSelf->__PVT__w_im;
            __Vdly__u_bfly__DOT__state = 1U;
        }
    } else {
        __Vdly__load_ptr = 0U;
        __Vdly__state = 0U;
        __Vdly__out_ptr = 0U;
        __Vdly__ctrl_start = 0U;
        __Vdly__u_ctrl__DOT__running = 0U;
        __Vdly__u_ctrl__DOT__waiting = 0U;
        __Vdly__u_ctrl__DOT__stage = 0U;
        __Vdly__u_ctrl__DOT__j = 0U;
        __Vdly__u_ctrl__DOT__base = 0U;
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        __Vdly__u_bfly__DOT__state = 0U;
        vlSelf->__PVT__bfly_done = 0U;
        vlSelf->__PVT__bfly_a_re = 0U;
        vlSelf->__PVT__bfly_a_im = 0U;
        vlSelf->__PVT__bfly_b_re = 0U;
        vlSelf->__PVT__bfly_b_im = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m1_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m2_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m3_r = 0U;
    }
    vlSelf->__PVT__load_ptr = __Vdly__load_ptr;
    vlSelf->__PVT__out_ptr = __Vdly__out_ptr;
    if (__Vdlyvset__mem_re__v0) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v0] 
            = __Vdlyvval__mem_im__v0;
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v1] 
            = __Vdlyvval__mem_im__v1;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v0] 
            = __Vdlyvval__mem_re__v0;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v1] 
            = __Vdlyvval__mem_re__v1;
    }
    if (__Vdlyvset__mem_re__v2) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v2] 
            = __Vdlyvval__mem_im__v2;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v2] 
            = __Vdlyvval__mem_re__v2;
    }
    vlSelf->__PVT__state = __Vdly__state;
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
    }
    vlSelf->__PVT__ctrl_start = __Vdly__ctrl_start;
    vlSelf->__PVT__u_ctrl__DOT__running = __Vdly__u_ctrl__DOT__running;
    vlSelf->__PVT__u_ctrl__DOT__waiting = __Vdly__u_ctrl__DOT__waiting;
    vlSelf->__PVT__u_ctrl__DOT__stage = __Vdly__u_ctrl__DOT__stage;
    vlSelf->__PVT__u_ctrl__DOT__base = __Vdly__u_ctrl__DOT__base;
    vlSelf->__PVT__u_ctrl__DOT__j = __Vdly__u_ctrl__DOT__j;
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__bfly_start = __Vdly__bfly_start;
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
    __Vtableidx2 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx2];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx2];
    vlSelf->__PVT__u_bfly__DOT__state = __Vdly__u_bfly__DOT__state;
    __PVT__u_bfly__DOT__mul_a = 0U;
    __PVT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->__PVT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->__PVT__u_bfly__DOT__state)))) {
                __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_im_r;
                __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))
                                      ? (IData)(vlSelf->__PVT__u_bfly__DOT__diff_re_r)
                                      : (IData)(vlSelf->__PVT__u_bfly__DOT__diff_im_r));
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_re_r;
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
    }
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
}

VL_INLINE_OPT void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__4__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__4__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__4__x;
    __Vfunc_u_bfly__DOT__sat16__4__x = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__5__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__5__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__5__x;
    __Vfunc_u_bfly__DOT__sat16__5__x = 0;
    CData/*7:0*/ __Vtableidx3;
    __Vtableidx3 = 0;
    CData/*0:0*/ __Vdly__ctrl_start;
    __Vdly__ctrl_start = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v0;
    __Vdlyvdim0__mem_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v0;
    __Vdlyvval__mem_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v0;
    __Vdlyvset__mem_re__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v0;
    __Vdlyvdim0__mem_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v0;
    __Vdlyvval__mem_im__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v1;
    __Vdlyvdim0__mem_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v1;
    __Vdlyvval__mem_re__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v1;
    __Vdlyvdim0__mem_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v1;
    __Vdlyvval__mem_im__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v2;
    __Vdlyvdim0__mem_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v2;
    __Vdlyvval__mem_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v2;
    __Vdlyvset__mem_re__v2 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v2;
    __Vdlyvdim0__mem_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v2;
    __Vdlyvval__mem_im__v2 = 0;
    CData/*7:0*/ __Vdly__load_ptr;
    __Vdly__load_ptr = 0;
    CData/*1:0*/ __Vdly__state;
    __Vdly__state = 0;
    CData/*7:0*/ __Vdly__out_ptr;
    __Vdly__out_ptr = 0;
    CData/*0:0*/ __Vdly__bfly_start;
    __Vdly__bfly_start = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__running;
    __Vdly__u_ctrl__DOT__running = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__waiting = 0;
    CData/*2:0*/ __Vdly__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__stage = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__j;
    __Vdly__u_ctrl__DOT__j = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__base = 0;
    CData/*2:0*/ __Vdly__u_bfly__DOT__state;
    __Vdly__u_bfly__DOT__state = 0;
    // Body
    __Vdly__u_bfly__DOT__state = vlSelf->__PVT__u_bfly__DOT__state;
    __Vdly__u_ctrl__DOT__waiting = vlSelf->__PVT__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__running = vlSelf->__PVT__u_ctrl__DOT__running;
    __Vdly__bfly_start = vlSelf->__PVT__bfly_start;
    __Vdly__u_ctrl__DOT__stage = vlSelf->__PVT__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__base = vlSelf->__PVT__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__j = vlSelf->__PVT__u_ctrl__DOT__j;
    __Vdly__load_ptr = vlSelf->__PVT__load_ptr;
    __Vdly__ctrl_start = vlSelf->__PVT__ctrl_start;
    __Vdly__out_ptr = vlSelf->__PVT__out_ptr;
    __Vdlyvset__mem_re__v0 = 0U;
    __Vdlyvset__mem_re__v2 = 0U;
    __Vdly__state = vlSelf->__PVT__state;
    if (vlSymsp->TOP.rst_n) {
        __Vdly__ctrl_start = 0U;
        if (vlSelf->__PVT__bfly_done) {
            __Vdlyvval__mem_re__v0 = vlSelf->__PVT__bfly_a_re;
            __Vdlyvset__mem_re__v0 = 1U;
            __Vdlyvdim0__mem_re__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_im__v0 = vlSelf->__PVT__bfly_a_im;
            __Vdlyvdim0__mem_im__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_re__v1 = vlSelf->__PVT__bfly_b_re;
            __Vdlyvdim0__mem_re__v1 = vlSelf->__PVT__idx_b;
            __Vdlyvval__mem_im__v1 = vlSelf->__PVT__bfly_b_im;
            __Vdlyvdim0__mem_im__v1 = vlSelf->__PVT__idx_b;
        }
        if ((0U == (IData)(vlSelf->__PVT__state))) {
            if ((((IData)(vlSymsp->TOP.fft2d_core__DOT__f_in_valid) 
                  >> 2U) & (IData)(vlSelf->in_ready))) {
                __Vdlyvval__mem_re__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_re
                    [2U];
                __Vdlyvset__mem_re__v2 = 1U;
                __Vdlyvdim0__mem_re__v2 = vlSelf->__PVT__load_ptr;
                __Vdlyvval__mem_im__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_im
                    [2U];
                __Vdlyvdim0__mem_im__v2 = vlSelf->__PVT__load_ptr;
                if ((0xffU == (IData)(vlSelf->__PVT__load_ptr))) {
                    __Vdly__load_ptr = 0U;
                    __Vdly__ctrl_start = 1U;
                    __Vdly__state = 1U;
                } else {
                    __Vdly__load_ptr = (0xffU & ((IData)(1U) 
                                                 + (IData)(vlSelf->__PVT__load_ptr)));
                }
            }
        } else if ((1U == (IData)(vlSelf->__PVT__state))) {
            if (vlSelf->__PVT__ctrl_done) {
                __Vdly__out_ptr = 0U;
                __Vdly__state = 2U;
            }
        } else if ((2U == (IData)(vlSelf->__PVT__state))) {
            if (((IData)(vlSelf->out_valid) & ((IData)(vlSymsp->TOP.fft2d_core__DOT__f_out_ready) 
                                               >> 2U))) {
                if ((0xffU == (IData)(vlSelf->__PVT__out_ptr))) {
                    __Vdly__out_ptr = 0U;
                    __Vdly__state = 0U;
                } else {
                    __Vdly__out_ptr = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->__PVT__out_ptr)));
                }
            }
        } else {
            __Vdly__state = 0U;
        }
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        if (((IData)(vlSelf->__PVT__ctrl_start) & (~ (IData)(vlSelf->__PVT__u_ctrl__DOT__running)))) {
            __Vdly__u_ctrl__DOT__running = 1U;
            __Vdly__u_ctrl__DOT__waiting = 0U;
            __Vdly__u_ctrl__DOT__stage = 0U;
            __Vdly__u_ctrl__DOT__j = 0U;
            __Vdly__u_ctrl__DOT__base = 0U;
        } else if (vlSelf->__PVT__u_ctrl__DOT__running) {
            if (vlSelf->__PVT__u_ctrl__DOT__waiting) {
                if (vlSelf->__PVT__bfly_done) {
                    __Vdly__u_ctrl__DOT__waiting = 0U;
                    if (((0x1ffU & ((IData)(1U) + (IData)(vlSelf->__PVT__u_ctrl__DOT__j))) 
                         < (IData)(vlSelf->__PVT__u_ctrl__DOT__half))) {
                        __Vdly__u_ctrl__DOT__j = (0xffU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
                    } else {
                        __Vdly__u_ctrl__DOT__j = 0U;
                        if ((0x100U > (0x1ffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                                 + (IData)(vlSelf->__PVT__u_ctrl__DOT__span))))) {
                            __Vdly__u_ctrl__DOT__base 
                                = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                            + (IData)(vlSelf->__PVT__u_ctrl__DOT__span)));
                        } else {
                            __Vdly__u_ctrl__DOT__base = 0U;
                            if ((8U > (0xfU & ((IData)(1U) 
                                               + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage))))) {
                                __Vdly__u_ctrl__DOT__stage 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
                            } else {
                                __Vdly__u_ctrl__DOT__running = 0U;
                                vlSelf->__PVT__ctrl_done = 1U;
                            }
                        }
                    }
                }
            } else {
                __Vdly__bfly_start = 1U;
                __Vdly__u_ctrl__DOT__waiting = 1U;
            }
        }
        vlSelf->__PVT__bfly_done = 0U;
        if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else {
                __Vfunc_u_bfly__DOT__sat16__4__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m1_r 
                                              - vlSelf->__PVT__u_bfly__DOT__m2_r), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__5__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m3_r 
                                              + vlSelf->__PVT__u_bfly__DOT__mul_p), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__4__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__4__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__4__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__4__x)));
                __Vfunc_u_bfly__DOT__sat16__5__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__5__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__5__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__5__x)));
                vlSelf->__PVT__bfly_a_re = vlSelf->__PVT__u_bfly__DOT__sum_re_r;
                vlSelf->__PVT__bfly_a_im = vlSelf->__PVT__u_bfly__DOT__sum_im_r;
                vlSelf->__PVT__bfly_b_re = __Vfunc_u_bfly__DOT__sat16__4__Vfuncout;
                vlSelf->__PVT__bfly_b_im = __Vfunc_u_bfly__DOT__sat16__5__Vfuncout;
                vlSelf->__PVT__bfly_done = 1U;
                __Vdly__u_bfly__DOT__state = 0U;
            }
        } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                vlSelf->__PVT__u_bfly__DOT__m3_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 4U;
            } else {
                vlSelf->__PVT__u_bfly__DOT__m2_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            vlSelf->__PVT__u_bfly__DOT__m1_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
            __Vdly__u_bfly__DOT__state = 2U;
        } else if (vlSelf->__PVT__bfly_start) {
            vlSelf->__PVT__u_bfly__DOT__sum_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__sum_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__w_re_r = vlSelf->__PVT__w_re;
            vlSelf->__PVT__u_bfly__DOT__w_im_r = vlSelf->__PVT__w_im;
            __Vdly__u_bfly__DOT__state = 1U;
        }
    } else {
        __Vdly__load_ptr = 0U;
        __Vdly__state = 0U;
        __Vdly__out_ptr = 0U;
        __Vdly__ctrl_start = 0U;
        __Vdly__u_ctrl__DOT__running = 0U;
        __Vdly__u_ctrl__DOT__waiting = 0U;
        __Vdly__u_ctrl__DOT__stage = 0U;
        __Vdly__u_ctrl__DOT__j = 0U;
        __Vdly__u_ctrl__DOT__base = 0U;
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        __Vdly__u_bfly__DOT__state = 0U;
        vlSelf->__PVT__bfly_done = 0U;
        vlSelf->__PVT__bfly_a_re = 0U;
        vlSelf->__PVT__bfly_a_im = 0U;
        vlSelf->__PVT__bfly_b_re = 0U;
        vlSelf->__PVT__bfly_b_im = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m1_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m2_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m3_r = 0U;
    }
    vlSelf->__PVT__load_ptr = __Vdly__load_ptr;
    vlSelf->__PVT__out_ptr = __Vdly__out_ptr;
    if (__Vdlyvset__mem_re__v0) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v0] 
            = __Vdlyvval__mem_im__v0;
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v1] 
            = __Vdlyvval__mem_im__v1;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v0] 
            = __Vdlyvval__mem_re__v0;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v1] 
            = __Vdlyvval__mem_re__v1;
    }
    if (__Vdlyvset__mem_re__v2) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v2] 
            = __Vdlyvval__mem_im__v2;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v2] 
            = __Vdlyvval__mem_re__v2;
    }
    vlSelf->__PVT__state = __Vdly__state;
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
    }
    vlSelf->__PVT__ctrl_start = __Vdly__ctrl_start;
    vlSelf->__PVT__u_ctrl__DOT__running = __Vdly__u_ctrl__DOT__running;
    vlSelf->__PVT__u_ctrl__DOT__waiting = __Vdly__u_ctrl__DOT__waiting;
    vlSelf->__PVT__u_ctrl__DOT__stage = __Vdly__u_ctrl__DOT__stage;
    vlSelf->__PVT__u_ctrl__DOT__base = __Vdly__u_ctrl__DOT__base;
    vlSelf->__PVT__u_ctrl__DOT__j = __Vdly__u_ctrl__DOT__j;
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__bfly_start = __Vdly__bfly_start;
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
    __Vtableidx3 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx3];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx3];
    vlSelf->__PVT__u_bfly__DOT__state = __Vdly__u_bfly__DOT__state;
    __PVT__u_bfly__DOT__mul_a = 0U;
    __PVT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->__PVT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->__PVT__u_bfly__DOT__state)))) {
                __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_im_r;
                __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))
                                      ? (IData)(vlSelf->__PVT__u_bfly__DOT__diff_re_r)
                                      : (IData)(vlSelf->__PVT__u_bfly__DOT__diff_im_r));
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_re_r;
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
    }
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
}

VL_INLINE_OPT void Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___nba_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__6__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__6__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__6__x;
    __Vfunc_u_bfly__DOT__sat16__6__x = 0;
    SData/*15:0*/ __Vfunc_u_bfly__DOT__sat16__7__Vfuncout;
    __Vfunc_u_bfly__DOT__sat16__7__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_u_bfly__DOT__sat16__7__x;
    __Vfunc_u_bfly__DOT__sat16__7__x = 0;
    CData/*7:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    CData/*0:0*/ __Vdly__ctrl_start;
    __Vdly__ctrl_start = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v0;
    __Vdlyvdim0__mem_re__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v0;
    __Vdlyvval__mem_re__v0 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v0;
    __Vdlyvset__mem_re__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v0;
    __Vdlyvdim0__mem_im__v0 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v0;
    __Vdlyvval__mem_im__v0 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v1;
    __Vdlyvdim0__mem_re__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v1;
    __Vdlyvval__mem_re__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v1;
    __Vdlyvdim0__mem_im__v1 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v1;
    __Vdlyvval__mem_im__v1 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_re__v2;
    __Vdlyvdim0__mem_re__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_re__v2;
    __Vdlyvval__mem_re__v2 = 0;
    CData/*0:0*/ __Vdlyvset__mem_re__v2;
    __Vdlyvset__mem_re__v2 = 0;
    CData/*7:0*/ __Vdlyvdim0__mem_im__v2;
    __Vdlyvdim0__mem_im__v2 = 0;
    SData/*15:0*/ __Vdlyvval__mem_im__v2;
    __Vdlyvval__mem_im__v2 = 0;
    CData/*7:0*/ __Vdly__load_ptr;
    __Vdly__load_ptr = 0;
    CData/*1:0*/ __Vdly__state;
    __Vdly__state = 0;
    CData/*7:0*/ __Vdly__out_ptr;
    __Vdly__out_ptr = 0;
    CData/*0:0*/ __Vdly__bfly_start;
    __Vdly__bfly_start = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__running;
    __Vdly__u_ctrl__DOT__running = 0;
    CData/*0:0*/ __Vdly__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__waiting = 0;
    CData/*2:0*/ __Vdly__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__stage = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__j;
    __Vdly__u_ctrl__DOT__j = 0;
    CData/*7:0*/ __Vdly__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__base = 0;
    CData/*2:0*/ __Vdly__u_bfly__DOT__state;
    __Vdly__u_bfly__DOT__state = 0;
    // Body
    __Vdly__u_bfly__DOT__state = vlSelf->__PVT__u_bfly__DOT__state;
    __Vdly__u_ctrl__DOT__waiting = vlSelf->__PVT__u_ctrl__DOT__waiting;
    __Vdly__u_ctrl__DOT__running = vlSelf->__PVT__u_ctrl__DOT__running;
    __Vdly__bfly_start = vlSelf->__PVT__bfly_start;
    __Vdly__u_ctrl__DOT__stage = vlSelf->__PVT__u_ctrl__DOT__stage;
    __Vdly__u_ctrl__DOT__base = vlSelf->__PVT__u_ctrl__DOT__base;
    __Vdly__u_ctrl__DOT__j = vlSelf->__PVT__u_ctrl__DOT__j;
    __Vdly__load_ptr = vlSelf->__PVT__load_ptr;
    __Vdly__ctrl_start = vlSelf->__PVT__ctrl_start;
    __Vdly__out_ptr = vlSelf->__PVT__out_ptr;
    __Vdlyvset__mem_re__v0 = 0U;
    __Vdlyvset__mem_re__v2 = 0U;
    __Vdly__state = vlSelf->__PVT__state;
    if (vlSymsp->TOP.rst_n) {
        __Vdly__ctrl_start = 0U;
        if (vlSelf->__PVT__bfly_done) {
            __Vdlyvval__mem_re__v0 = vlSelf->__PVT__bfly_a_re;
            __Vdlyvset__mem_re__v0 = 1U;
            __Vdlyvdim0__mem_re__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_im__v0 = vlSelf->__PVT__bfly_a_im;
            __Vdlyvdim0__mem_im__v0 = vlSelf->__PVT__idx_a;
            __Vdlyvval__mem_re__v1 = vlSelf->__PVT__bfly_b_re;
            __Vdlyvdim0__mem_re__v1 = vlSelf->__PVT__idx_b;
            __Vdlyvval__mem_im__v1 = vlSelf->__PVT__bfly_b_im;
            __Vdlyvdim0__mem_im__v1 = vlSelf->__PVT__idx_b;
        }
        if ((0U == (IData)(vlSelf->__PVT__state))) {
            if ((((IData)(vlSymsp->TOP.fft2d_core__DOT__f_in_valid) 
                  >> 3U) & (IData)(vlSelf->in_ready))) {
                __Vdlyvval__mem_re__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_re
                    [3U];
                __Vdlyvset__mem_re__v2 = 1U;
                __Vdlyvdim0__mem_re__v2 = vlSelf->__PVT__load_ptr;
                __Vdlyvval__mem_im__v2 = vlSymsp->TOP.fft2d_core__DOT__f_in_im
                    [3U];
                __Vdlyvdim0__mem_im__v2 = vlSelf->__PVT__load_ptr;
                if ((0xffU == (IData)(vlSelf->__PVT__load_ptr))) {
                    __Vdly__load_ptr = 0U;
                    __Vdly__ctrl_start = 1U;
                    __Vdly__state = 1U;
                } else {
                    __Vdly__load_ptr = (0xffU & ((IData)(1U) 
                                                 + (IData)(vlSelf->__PVT__load_ptr)));
                }
            }
        } else if ((1U == (IData)(vlSelf->__PVT__state))) {
            if (vlSelf->__PVT__ctrl_done) {
                __Vdly__out_ptr = 0U;
                __Vdly__state = 2U;
            }
        } else if ((2U == (IData)(vlSelf->__PVT__state))) {
            if (((IData)(vlSelf->out_valid) & ((IData)(vlSymsp->TOP.fft2d_core__DOT__f_out_ready) 
                                               >> 3U))) {
                if ((0xffU == (IData)(vlSelf->__PVT__out_ptr))) {
                    __Vdly__out_ptr = 0U;
                    __Vdly__state = 0U;
                } else {
                    __Vdly__out_ptr = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->__PVT__out_ptr)));
                }
            }
        } else {
            __Vdly__state = 0U;
        }
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        if (((IData)(vlSelf->__PVT__ctrl_start) & (~ (IData)(vlSelf->__PVT__u_ctrl__DOT__running)))) {
            __Vdly__u_ctrl__DOT__running = 1U;
            __Vdly__u_ctrl__DOT__waiting = 0U;
            __Vdly__u_ctrl__DOT__stage = 0U;
            __Vdly__u_ctrl__DOT__j = 0U;
            __Vdly__u_ctrl__DOT__base = 0U;
        } else if (vlSelf->__PVT__u_ctrl__DOT__running) {
            if (vlSelf->__PVT__u_ctrl__DOT__waiting) {
                if (vlSelf->__PVT__bfly_done) {
                    __Vdly__u_ctrl__DOT__waiting = 0U;
                    if (((0x1ffU & ((IData)(1U) + (IData)(vlSelf->__PVT__u_ctrl__DOT__j))) 
                         < (IData)(vlSelf->__PVT__u_ctrl__DOT__half))) {
                        __Vdly__u_ctrl__DOT__j = (0xffU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
                    } else {
                        __Vdly__u_ctrl__DOT__j = 0U;
                        if ((0x100U > (0x1ffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                                 + (IData)(vlSelf->__PVT__u_ctrl__DOT__span))))) {
                            __Vdly__u_ctrl__DOT__base 
                                = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                            + (IData)(vlSelf->__PVT__u_ctrl__DOT__span)));
                        } else {
                            __Vdly__u_ctrl__DOT__base = 0U;
                            if ((8U > (0xfU & ((IData)(1U) 
                                               + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage))))) {
                                __Vdly__u_ctrl__DOT__stage 
                                    = (7U & ((IData)(1U) 
                                             + (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
                            } else {
                                __Vdly__u_ctrl__DOT__running = 0U;
                                vlSelf->__PVT__ctrl_done = 1U;
                            }
                        }
                    }
                }
            } else {
                __Vdly__bfly_start = 1U;
                __Vdly__u_ctrl__DOT__waiting = 1U;
            }
        }
        vlSelf->__PVT__bfly_done = 0U;
        if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                __Vdly__u_bfly__DOT__state = 0U;
            } else {
                __Vfunc_u_bfly__DOT__sat16__6__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m1_r 
                                              - vlSelf->__PVT__u_bfly__DOT__m2_r), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__7__x = 
                    VL_SHIFTRS_III(32,32,32, (vlSelf->__PVT__u_bfly__DOT__m3_r 
                                              + vlSelf->__PVT__u_bfly__DOT__mul_p), 0xfU);
                __Vfunc_u_bfly__DOT__sat16__6__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__6__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__6__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__6__x)));
                __Vfunc_u_bfly__DOT__sat16__7__Vfuncout 
                    = (VL_LTS_III(32, 0x7fffU, __Vfunc_u_bfly__DOT__sat16__7__x)
                        ? 0x7fffU : (VL_GTS_III(32, 0xffff8000U, __Vfunc_u_bfly__DOT__sat16__7__x)
                                      ? 0x8000U : (0xffffU 
                                                   & __Vfunc_u_bfly__DOT__sat16__7__x)));
                vlSelf->__PVT__bfly_a_re = vlSelf->__PVT__u_bfly__DOT__sum_re_r;
                vlSelf->__PVT__bfly_a_im = vlSelf->__PVT__u_bfly__DOT__sum_im_r;
                vlSelf->__PVT__bfly_b_re = __Vfunc_u_bfly__DOT__sat16__6__Vfuncout;
                vlSelf->__PVT__bfly_b_im = __Vfunc_u_bfly__DOT__sat16__7__Vfuncout;
                vlSelf->__PVT__bfly_done = 1U;
                __Vdly__u_bfly__DOT__state = 0U;
            }
        } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
                vlSelf->__PVT__u_bfly__DOT__m3_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 4U;
            } else {
                vlSelf->__PVT__u_bfly__DOT__m2_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
                __Vdly__u_bfly__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
            vlSelf->__PVT__u_bfly__DOT__m1_r = vlSelf->__PVT__u_bfly__DOT__mul_p;
            __Vdly__u_bfly__DOT__state = 2U;
        } else if (vlSelf->__PVT__bfly_start) {
            vlSelf->__PVT__u_bfly__DOT__sum_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__sum_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              + ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_re_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_re) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_re)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_re) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_re)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__diff_im_r = 
                (0xffffU & VL_SHIFTRS_III(17,17,32, 
                                          (0x1ffffU 
                                           & (((0x10000U 
                                                & ((IData)(vlSelf->__Vcellinp__u_bfly__a_im) 
                                                   << 1U)) 
                                               | (IData)(vlSelf->__Vcellinp__u_bfly__a_im)) 
                                              - ((0x10000U 
                                                  & ((IData)(vlSelf->__Vcellinp__u_bfly__b_im) 
                                                     << 1U)) 
                                                 | (IData)(vlSelf->__Vcellinp__u_bfly__b_im)))), 1U));
            vlSelf->__PVT__u_bfly__DOT__w_re_r = vlSelf->__PVT__w_re;
            vlSelf->__PVT__u_bfly__DOT__w_im_r = vlSelf->__PVT__w_im;
            __Vdly__u_bfly__DOT__state = 1U;
        }
    } else {
        __Vdly__load_ptr = 0U;
        __Vdly__state = 0U;
        __Vdly__out_ptr = 0U;
        __Vdly__ctrl_start = 0U;
        __Vdly__u_ctrl__DOT__running = 0U;
        __Vdly__u_ctrl__DOT__waiting = 0U;
        __Vdly__u_ctrl__DOT__stage = 0U;
        __Vdly__u_ctrl__DOT__j = 0U;
        __Vdly__u_ctrl__DOT__base = 0U;
        __Vdly__bfly_start = 0U;
        vlSelf->__PVT__ctrl_done = 0U;
        __Vdly__u_bfly__DOT__state = 0U;
        vlSelf->__PVT__bfly_done = 0U;
        vlSelf->__PVT__bfly_a_re = 0U;
        vlSelf->__PVT__bfly_a_im = 0U;
        vlSelf->__PVT__bfly_b_re = 0U;
        vlSelf->__PVT__bfly_b_im = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__sum_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__diff_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_re_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__w_im_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m1_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m2_r = 0U;
        vlSelf->__PVT__u_bfly__DOT__m3_r = 0U;
    }
    vlSelf->__PVT__load_ptr = __Vdly__load_ptr;
    vlSelf->__PVT__out_ptr = __Vdly__out_ptr;
    if (__Vdlyvset__mem_re__v0) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v0] 
            = __Vdlyvval__mem_im__v0;
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v1] 
            = __Vdlyvval__mem_im__v1;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v0] 
            = __Vdlyvval__mem_re__v0;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v1] 
            = __Vdlyvval__mem_re__v1;
    }
    if (__Vdlyvset__mem_re__v2) {
        vlSelf->__PVT__mem_im[__Vdlyvdim0__mem_im__v2] 
            = __Vdlyvval__mem_im__v2;
        vlSelf->__PVT__mem_re[__Vdlyvdim0__mem_re__v2] 
            = __Vdlyvval__mem_re__v2;
    }
    vlSelf->__PVT__state = __Vdly__state;
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
    }
    vlSelf->__PVT__ctrl_start = __Vdly__ctrl_start;
    vlSelf->__PVT__u_ctrl__DOT__running = __Vdly__u_ctrl__DOT__running;
    vlSelf->__PVT__u_ctrl__DOT__waiting = __Vdly__u_ctrl__DOT__waiting;
    vlSelf->__PVT__u_ctrl__DOT__stage = __Vdly__u_ctrl__DOT__stage;
    vlSelf->__PVT__u_ctrl__DOT__base = __Vdly__u_ctrl__DOT__base;
    vlSelf->__PVT__u_ctrl__DOT__j = __Vdly__u_ctrl__DOT__j;
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__bfly_start = __Vdly__bfly_start;
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
    __Vtableidx4 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx4];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx4];
    vlSelf->__PVT__u_bfly__DOT__state = __Vdly__u_bfly__DOT__state;
    __PVT__u_bfly__DOT__mul_a = 0U;
    __PVT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->__PVT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->__PVT__u_bfly__DOT__state)))) {
                __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_im_r;
                __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))
                                      ? (IData)(vlSelf->__PVT__u_bfly__DOT__diff_re_r)
                                      : (IData)(vlSelf->__PVT__u_bfly__DOT__diff_im_r));
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->__PVT__u_bfly__DOT__state))) {
        __PVT__u_bfly__DOT__mul_a = vlSelf->__PVT__u_bfly__DOT__diff_re_r;
        __PVT__u_bfly__DOT__mul_b = vlSelf->__PVT__u_bfly__DOT__w_re_r;
    }
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
}
