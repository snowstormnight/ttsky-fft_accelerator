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
            VL_FATAL_MT("SV_fft2d_core.sv", 19, "", "Settle region did not converge.");
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

VL_ATTR_COLD void Vfft2d_core___024root___eval_triggers__stl(Vfft2d_core___024root* vlSelf);
VL_ATTR_COLD void Vfft2d_core___024root___eval_stl(Vfft2d_core___024root* vlSelf);

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
    vlSelf->fft2d_core__DOT__batch_base = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__feed_cnt = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__recv_cnt = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__out_ptr = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__load_ptr = VL_RAND_RESET_I(16);
    vlSelf->fft2d_core__DOT__f_in_valid = VL_RAND_RESET_I(4);
    vlSelf->fft2d_core__DOT__f_out_ready = VL_RAND_RESET_I(4);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->fft2d_core__DOT__f_in_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->fft2d_core__DOT__f_in_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->fft2d_core__DOT__f_out_re[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->fft2d_core__DOT__f_out_im[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->fft2d_core__DOT__feed_fire = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__recv_fire = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__perf_active = VL_RAND_RESET_I(1);
    vlSelf->fft2d_core__DOT__perf_counter = VL_RAND_RESET_Q(64);
    vlSelf->fft2d_core__DOT__unnamedblk6__DOT__unnamedblk7__DOT__row_l = VL_RAND_RESET_I(8);
    vlSelf->fft2d_core__DOT__unnamedblk8__DOT__unnamedblk9__DOT__col_l = VL_RAND_RESET_I(8);
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
