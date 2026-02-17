// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft1d_core.h for the primary calling header

#include "Vfft1d_core__pch.h"
#include "Vfft1d_core___024root.h"

VL_ATTR_COLD void Vfft1d_core___024root___eval_static(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vfft1d_core___024root___eval_initial(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vfft1d_core___024root___eval_final(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__stl(Vfft1d_core___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vfft1d_core___024root___eval_phase__stl(Vfft1d_core___024root* vlSelf);

VL_ATTR_COLD void Vfft1d_core___024root___eval_settle(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vfft1d_core___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("/home/george/project/ASIC_Hack/src/fft/fft1d_core.sv", 5, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vfft1d_core___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__stl(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

extern const VlUnpacked<SData/*15:0*/, 16> Vfft1d_core__ConstPool__TABLE_hd86c15f4_0;
extern const VlUnpacked<SData/*15:0*/, 16> Vfft1d_core__ConstPool__TABLE_h350e8472_0;

VL_ATTR_COLD void Vfft1d_core___024root___stl_sequent__TOP__0(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___stl_sequent__TOP__0\n"); );
    // Init
    SData/*15:0*/ fft1d_core__DOT__u_bfly__DOT__mul_a;
    fft1d_core__DOT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ fft1d_core__DOT__u_bfly__DOT__mul_b;
    fft1d_core__DOT__u_bfly__DOT__mul_b = 0;
    CData/*3:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->fft1d_core__DOT__state));
    if ((2U == (IData)(vlSelf->fft1d_core__DOT__state))) {
        vlSelf->out_valid = 1U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
        vlSelf->out_re = vlSelf->fft1d_core__DOT__mem_re
            [vlSelf->fft1d_core__DOT__out_ptr];
        vlSelf->out_im = vlSelf->fft1d_core__DOT__mem_im
            [vlSelf->fft1d_core__DOT__out_ptr];
    } else {
        vlSelf->out_valid = 0U;
        vlSelf->out_re = 0U;
        vlSelf->out_im = 0U;
    }
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
    __Vtableidx1 = (0xfU & ((IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__j) 
                            << (IData)(vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage)));
    vlSelf->fft1d_core__DOT__w_re = Vfft1d_core__ConstPool__TABLE_hd86c15f4_0
        [__Vtableidx1];
    vlSelf->fft1d_core__DOT__w_im = Vfft1d_core__ConstPool__TABLE_h350e8472_0
        [__Vtableidx1];
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
    vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p = VL_MULS_III(32, 
                                                              VL_EXTENDS_II(32,16, (IData)(fft1d_core__DOT__u_bfly__DOT__mul_a)), 
                                                              VL_EXTENDS_II(32,16, (IData)(fft1d_core__DOT__u_bfly__DOT__mul_b)));
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re 
        = vlSelf->fft1d_core__DOT__mem_re[vlSelf->fft1d_core__DOT__idx_a];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im 
        = vlSelf->fft1d_core__DOT__mem_im[vlSelf->fft1d_core__DOT__idx_a];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re 
        = vlSelf->fft1d_core__DOT__mem_re[vlSelf->fft1d_core__DOT__idx_b];
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im 
        = vlSelf->fft1d_core__DOT__mem_im[vlSelf->fft1d_core__DOT__idx_b];
}

VL_ATTR_COLD void Vfft1d_core___024root___eval_stl(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vfft1d_core___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vfft1d_core___024root___eval_triggers__stl(Vfft1d_core___024root* vlSelf);

VL_ATTR_COLD bool Vfft1d_core___024root___eval_phase__stl(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vfft1d_core___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vfft1d_core___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__act(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft1d_core___024root___dump_triggers__nba(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vfft1d_core___024root___ctor_var_reset(Vfft1d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft1d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft1d_core___024root___ctor_var_reset\n"); );
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
    vlSelf->fft1d_core__DOT__state = VL_RAND_RESET_I(2);
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->fft1d_core__DOT__mem_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 16; ++__Vi0) {
        vlSelf->fft1d_core__DOT__mem_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->fft1d_core__DOT__load_ptr = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__out_ptr = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__ctrl_start = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__ctrl_done = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__bfly_start = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__bfly_done = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__idx_a = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__idx_b = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__w_re = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__w_im = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__bfly_a_re = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__bfly_a_im = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__bfly_b_re = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__bfly_b_im = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_im = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__b_re = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_im = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT____Vcellinp__u_bfly__a_re = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__running = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__waiting = VL_RAND_RESET_I(1);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__stage = VL_RAND_RESET_I(2);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__j = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__base = VL_RAND_RESET_I(4);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__span = VL_RAND_RESET_I(5);
    vlSelf->fft1d_core__DOT__u_ctrl__DOT__half = VL_RAND_RESET_I(5);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__state = VL_RAND_RESET_I(3);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__sum_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__diff_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__w_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__w_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__m1_r = VL_RAND_RESET_I(32);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__m2_r = VL_RAND_RESET_I(32);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__m3_r = VL_RAND_RESET_I(32);
    vlSelf->fft1d_core__DOT__u_bfly__DOT__mul_p = VL_RAND_RESET_I(32);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
}
