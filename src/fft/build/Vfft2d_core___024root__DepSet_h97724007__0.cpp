// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core___024root.h"

void Vfft2d_core___024root___eval_act(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_act\n"); );
}

void Vfft2d_core___024root___eval_triggers__act(Vfft2d_core___024root* vlSelf);

bool Vfft2d_core___024root___eval_phase__act(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vfft2d_core___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vfft2d_core___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vfft2d_core___024root___eval_nba(Vfft2d_core___024root* vlSelf);

bool Vfft2d_core___024root___eval_phase__nba(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vfft2d_core___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__nba(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__act(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG

void Vfft2d_core___024root___eval(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vfft2d_core___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("SV_fft2d_core.sv", 19, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vfft2d_core___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("SV_fft2d_core.sv", 19, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vfft2d_core___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vfft2d_core___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vfft2d_core___024root___eval_debug_assertions(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_debug_assertions\n"); );
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
