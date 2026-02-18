// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core___024root.h"

VL_ATTR_COLD void Vfft2d_core___024root___eval_static(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vfft2d_core___024root___eval_initial(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vfft2d_core___024root___eval_final(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__stl(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vfft2d_core___024root___eval_phase__stl(Vfft2d_core___024root* vlSelf);

VL_ATTR_COLD void Vfft2d_core___024root___eval_settle(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_settle\n"); );
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
            Vfft2d_core___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("SV_fft2d_core.sv", 17, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vfft2d_core___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__stl(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_hd83d655a_0;
extern const VlUnpacked<SData/*15:0*/, 256> Vfft2d_core__ConstPool__TABLE_h6c2add80_0;

VL_ATTR_COLD void Vfft2d_core___024root___stl_sequent__TOP__0(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___stl_sequent__TOP__0\n"); );
    // Init
    SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a;
    fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a = 0;
    SData/*15:0*/ fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b;
    fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b = 0;
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->fft2d_core__DOT__state));
    vlSelf->out_valid = (5U == (IData)(vlSelf->fft2d_core__DOT__state));
    vlSelf->fft2d_core__DOT__f_in_ready = (0U == (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__state));
    if ((2U == (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__state))) {
        vlSelf->fft2d_core__DOT__f_out_valid = 1U;
        vlSelf->fft2d_core__DOT__f_out_re = 0U;
        vlSelf->fft2d_core__DOT__f_out_im = 0U;
        vlSelf->fft2d_core__DOT__f_out_re = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_re
            [vlSelf->fft2d_core__DOT__u_fft1d__DOT__out_ptr];
        vlSelf->fft2d_core__DOT__f_out_im = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_im
            [vlSelf->fft2d_core__DOT__u_fft1d__DOT__out_ptr];
    } else {
        vlSelf->fft2d_core__DOT__f_out_valid = 0U;
        vlSelf->fft2d_core__DOT__f_out_re = 0U;
        vlSelf->fft2d_core__DOT__f_out_im = 0U;
    }
    fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a = 0U;
    fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b = 0U;
    if ((4U & (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state)))) {
                fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a 
                    = vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_im_r;
                fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b 
                    = vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_re_r;
            }
        }
    } else if ((2U & (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state))) {
        fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a 
            = ((1U & (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state))
                ? (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_re_r)
                : (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_im_r));
        fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b 
            = vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_im_r;
    } else if ((1U & (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state))) {
        fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a 
            = vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_re_r;
        fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b 
            = vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_re_r;
    }
    __Vtableidx1 = (0xffU & ((IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__j) 
                             << (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__stage)));
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__w_re = Vfft2d_core__ConstPool__TABLE_hd83d655a_0
        [__Vtableidx1];
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__w_im = Vfft2d_core__ConstPool__TABLE_h6c2add80_0
        [__Vtableidx1];
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_a = 
        (0xffU & ((IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__base) 
                  + (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__j)));
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__span 
        = (0x1ffU & (0x100U >> (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__stage)));
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__half 
        = (0x1ffU & VL_SHIFTR_III(9,9,32, (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__span), 1U));
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_b = 
        (0xffU & (((IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__base) 
                   + (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__j)) 
                  + (IData)(vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__half)));
    vlSelf->fft2d_core__DOT__f_in_valid = 0U;
    vlSelf->fft2d_core__DOT__f_in_re = 0U;
    vlSelf->fft2d_core__DOT__f_in_im = 0U;
    vlSelf->fft2d_core__DOT__f_out_ready = 0U;
    vlSelf->out_re = 0U;
    vlSelf->out_im = 0U;
    if ((4U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        if ((1U & (~ ((IData)(vlSelf->fft2d_core__DOT__state) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
                vlSelf->out_re = vlSelf->fft2d_core__DOT__mem0_re
                    [vlSelf->fft2d_core__DOT__out_ptr];
                vlSelf->out_im = vlSelf->fft2d_core__DOT__mem0_im
                    [vlSelf->fft2d_core__DOT__out_ptr];
            } else {
                vlSelf->fft2d_core__DOT__f_out_ready = 1U;
            }
        }
    } else if ((2U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
            vlSelf->fft2d_core__DOT__f_in_valid = 1U;
            vlSelf->fft2d_core__DOT__f_in_re = vlSelf->fft2d_core__DOT__mem1_re
                [([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c 
                        = vlSelf->fft2d_core__DOT__col_idx;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout))];
            vlSelf->fft2d_core__DOT__f_in_im = vlSelf->fft2d_core__DOT__mem1_im
                [([&]() {
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c 
                        = vlSelf->fft2d_core__DOT__col_idx;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r 
                        = vlSelf->fft2d_core__DOT__feed_cnt;
                    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout 
                        = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r) 
                            << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c));
                }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout))];
        } else {
            vlSelf->fft2d_core__DOT__f_out_ready = 1U;
        }
    } else if ((1U & (IData)(vlSelf->fft2d_core__DOT__state))) {
        vlSelf->fft2d_core__DOT__f_in_valid = 1U;
        vlSelf->fft2d_core__DOT__f_in_re = vlSelf->fft2d_core__DOT__mem0_re
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r 
                    = vlSelf->fft2d_core__DOT__row_idx;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout))];
        vlSelf->fft2d_core__DOT__f_in_im = vlSelf->fft2d_core__DOT__mem0_im
            [([&]() {
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c 
                    = vlSelf->fft2d_core__DOT__feed_cnt;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r 
                    = vlSelf->fft2d_core__DOT__row_idx;
                vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout 
                    = (((IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r) 
                        << 8U) | (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c));
            }(), (IData)(vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout))];
    }
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_p 
        = VL_MULS_III(32, VL_EXTENDS_II(32,16, (IData)(fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_a)), 
                      VL_EXTENDS_II(32,16, (IData)(fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_b)));
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_re 
        = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_re
        [vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_a];
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_im 
        = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_im
        [vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_a];
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_re 
        = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_re
        [vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_b];
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_im 
        = vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_im
        [vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_b];
}

VL_ATTR_COLD void Vfft2d_core___024root___eval_stl(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vfft2d_core___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vfft2d_core___024root___eval_triggers__stl(Vfft2d_core___024root* vlSelf);

VL_ATTR_COLD bool Vfft2d_core___024root___eval_phase__stl(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vfft2d_core___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vfft2d_core___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__act(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___dump_triggers__act\n"); );
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
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__nba(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vfft2d_core___024root___ctor_var_reset(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___ctor_var_reset\n"); );
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
    vlSelf->perf_done = VL_RAND_RESET_I(1);
    vlSelf->perf_cycles = VL_RAND_RESET_Q(64);
    vlSelf->fft2d_core__DOT__state = VL_RAND_RESET_I(3);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->fft2d_core__DOT__mem0_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->fft2d_core__DOT__mem0_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->fft2d_core__DOT__mem1_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->fft2d_core__DOT__mem1_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->fft2d_core__DOT__row_idx = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__col_idx = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__feed_cnt = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__recv_cnt = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__out_ptr = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__load_ptr = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__f_in_valid = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__f_in_ready = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__f_out_valid = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__f_out_ready = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__f_in_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__f_in_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__f_out_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__f_out_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__perf_active = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__perf_counter = VL_RAND_RESET_Q(64);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__state = VL_RAND_RESET_I(2);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->fft2d_core__DOT__u_fft1d__DOT__mem_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__load_ptr = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__out_ptr = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__ctrl_start = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__ctrl_done = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_start = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_done = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_a = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__idx_b = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__w_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__w_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_a_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_a_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_b_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__bfly_b_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__b_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_im = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT____Vcellinp__u_bfly__a_re = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__running = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__waiting = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__stage = VL_RAND_RESET_I(3);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__j = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__base = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__span = VL_RAND_RESET_I(9);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_ctrl__DOT__half = VL_RAND_RESET_I(9);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__state = VL_RAND_RESET_I(3);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__sum_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__sum_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__diff_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_re_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__w_im_r = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m1_r = VL_RAND_RESET_I(32);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m2_r = VL_RAND_RESET_I(32);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__m3_r = VL_RAND_RESET_I(32);
    vlSelf->fft2d_core__DOT__u_fft1d__DOT__u_bfly__DOT__mul_p = VL_RAND_RESET_I(32);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__Vfuncout = VL_RAND_RESET_I(16);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__r = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__0__c = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__Vfuncout = VL_RAND_RESET_I(16);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__r = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__1__c = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__Vfuncout = VL_RAND_RESET_I(16);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__r = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__2__c = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__Vfuncout = VL_RAND_RESET_I(16);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__r = VL_RAND_RESET_I(8);
    vlSelf->__Vfunc_fft2d_core__DOT__idx2d__3__c = VL_RAND_RESET_I(8);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
}
