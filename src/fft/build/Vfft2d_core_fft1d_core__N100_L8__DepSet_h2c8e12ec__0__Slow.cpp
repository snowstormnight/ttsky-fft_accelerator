// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core_fft1d_core__N100_L8.h"

extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_hd83d655a_0;
extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_h6c2add80_0;

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    vlSelf->out_valid = (2U == (IData)(vlSelf->__PVT__state));
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
    __Vtableidx1 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx1];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx1];
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    }
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
}

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    CData/*7:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    vlSelf->out_valid = (2U == (IData)(vlSelf->__PVT__state));
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
    __Vtableidx2 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx2];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx2];
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    }
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
}

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    CData/*7:0*/ __Vtableidx3;
    __Vtableidx3 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    vlSelf->out_valid = (2U == (IData)(vlSelf->__PVT__state));
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
    __Vtableidx3 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx3];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx3];
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    }
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
}

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0\n"); );
    // Init
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_a;
    __PVT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ __PVT__u_bfly__DOT__mul_b;
    __PVT__u_bfly__DOT__mul_b = 0;
    CData/*7:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->__PVT__state));
    vlSelf->out_valid = (2U == (IData)(vlSelf->__PVT__state));
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
    __Vtableidx4 = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx4];
    vlSelf->__PVT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx4];
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    if ((2U == (IData)(vlSelf->__PVT__state))) {
        vlSelf->out_re = vlSelf->__PVT__mem_re[vlSelf->__PVT__out_ptr];
        vlSelf->out_im = vlSelf->__PVT__mem_im[vlSelf->__PVT__out_ptr];
    }
    vlSelf->__PVT__idx_a = (0xffU & ((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)));
    vlSelf->__PVT__u_ctrl__DOT__span = (0x1ffU & (0x100U 
                                                  >> (IData)(vlSelf->__PVT__u_ctrl__DOT__stage)));
    vlSelf->__PVT__u_ctrl__DOT__half = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->__PVT__u_ctrl__DOT__span), 1U));
    vlSelf->__PVT__idx_b = (0xffU & (((IData)(vlSelf->__PVT__u_ctrl__DOT__base) 
                                      + (IData)(vlSelf->__PVT__u_ctrl__DOT__j)) 
                                     + (IData)(vlSelf->__PVT__u_ctrl__DOT__half)));
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_a)), 
                                                    VL_EXTENDS_II(32,16, (IData)(__PVT__u_bfly__DOT__mul_b)));
    vlSelf->__Vcellinp__u_bfly__a_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__a_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_a];
    vlSelf->__Vcellinp__u_bfly__b_re = vlSelf->__PVT__mem_re
        [vlSelf->__PVT__idx_b];
    vlSelf->__Vcellinp__u_bfly__b_im = vlSelf->__PVT__mem_im
        [vlSelf->__PVT__idx_b];
}

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___ctor_var_reset(Vfft2d_core_fft1d_core__N100_L8* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vfft2d_core_fft1d_core__N100_L8___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->in_valid = VL_RAND_RESET_I(1);
    vlSelf->in_ready = VL_RAND_RESET_I(1);
    vlSelf->in_re = VL_RAND_RESET_I(16);
    vlSelf->in_im = VL_RAND_RESET_I(16);
    vlSelf->out_valid = VL_RAND_RESET_I(1);
    vlSelf->out_ready = VL_RAND_RESET_I(1);
    vlSelf->out_re = VL_RAND_RESET_I(16);
    vlSelf->out_im = VL_RAND_RESET_I(16);
    vlSelf->__PVT__state = VL_RAND_RESET_I(2);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->__PVT__mem_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->__PVT__mem_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->__PVT__load_ptr = VL_RAND_RESET_I(8);
    vlSelf->__PVT__out_ptr = VL_RAND_RESET_I(8);
    vlSelf->__PVT__ctrl_start = VL_RAND_RESET_I(1);
    vlSelf->__PVT__ctrl_done = VL_RAND_RESET_I(1);
    vlSelf->__PVT__bfly_start = VL_RAND_RESET_I(1);
    vlSelf->__PVT__bfly_done = VL_RAND_RESET_I(1);
    vlSelf->__PVT__idx_a = VL_RAND_RESET_I(8);
    vlSelf->__PVT__idx_b = VL_RAND_RESET_I(8);
    vlSelf->__PVT__w_re = VL_RAND_RESET_I(16);
    vlSelf->__PVT__w_im = VL_RAND_RESET_I(16);
    vlSelf->__PVT__bfly_a_re = VL_RAND_RESET_I(16);
    vlSelf->__PVT__bfly_a_im = VL_RAND_RESET_I(16);
    vlSelf->__PVT__bfly_b_re = VL_RAND_RESET_I(16);
    vlSelf->__PVT__bfly_b_im = VL_RAND_RESET_I(16);
    vlSelf->__Vcellinp__u_bfly__b_im = VL_RAND_RESET_I(16);
    vlSelf->__Vcellinp__u_bfly__b_re = VL_RAND_RESET_I(16);
    vlSelf->__Vcellinp__u_bfly__a_im = VL_RAND_RESET_I(16);
    vlSelf->__Vcellinp__u_bfly__a_re = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_ctrl__DOT__running = VL_RAND_RESET_I(1);
    vlSelf->__PVT__u_ctrl__DOT__waiting = VL_RAND_RESET_I(1);
    vlSelf->__PVT__u_ctrl__DOT__stage = VL_RAND_RESET_I(3);
    vlSelf->__PVT__u_ctrl__DOT__j = VL_RAND_RESET_I(8);
    vlSelf->__PVT__u_ctrl__DOT__base = VL_RAND_RESET_I(8);
    vlSelf->__PVT__u_ctrl__DOT__span = VL_RAND_RESET_I(9);
    vlSelf->__PVT__u_ctrl__DOT__half = VL_RAND_RESET_I(9);
    vlSelf->__PVT__u_bfly__DOT__state = VL_RAND_RESET_I(3);
    vlSelf->__PVT__u_bfly__DOT__sum_re_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__sum_im_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__diff_re_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__diff_im_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__w_re_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__w_im_r = VL_RAND_RESET_I(16);
    vlSelf->__PVT__u_bfly__DOT__m1_r = VL_RAND_RESET_I(32);
    vlSelf->__PVT__u_bfly__DOT__m2_r = VL_RAND_RESET_I(32);
    vlSelf->__PVT__u_bfly__DOT__m3_r = VL_RAND_RESET_I(32);
    vlSelf->__PVT__u_bfly__DOT__mul_p = VL_RAND_RESET_I(32);
}
