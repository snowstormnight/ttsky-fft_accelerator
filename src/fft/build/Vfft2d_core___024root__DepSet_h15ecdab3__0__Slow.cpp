// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vfft2d_core.h for the primary calling header

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core__Syms.h"
#include "Vfft2d_core___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vfft2d_core___024root___dump_triggers__stl(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vfft2d_core___024root___eval_triggers__stl(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vfft2d_core___024root___dump_triggers__stl(vlSelf);
    }
#endif
}

VL_ATTR_COLD void Vfft2d_core___024root___stl_sequent__TOP__0(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___stl_sequent__TOP__0\n"); );
    // Init
    CData/*3:0*/ fft2d_core__DOT__f_in_ready;
    fft2d_core__DOT__f_in_ready = 0;
    CData/*3:0*/ fft2d_core__DOT__f_out_valid;
    fft2d_core__DOT__f_out_valid = 0;
    // Body
    vlSelf->in_ready = (0U == (IData)(vlSelf->fft2d_core__DOT__state));
    vlSelf->out_valid = (5U == (IData)(vlSelf->fft2d_core__DOT__state));
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

VL_ATTR_COLD void Vfft2d_core___024root___stl_sequent__TOP__1(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___stl_sequent__TOP__1\n"); );
    // Body
    vlSelf->fft2d_core__DOT__f_out_re[0U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_im[0U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_re[1U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_im[1U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_re[2U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_im[2U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.out_im;
    vlSelf->fft2d_core__DOT__f_out_re[3U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.out_re;
    vlSelf->fft2d_core__DOT__f_out_im[3U] = vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.out_im;
}

VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);
VL_ATTR_COLD void Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0(Vfft2d_core_fft1d_core__N100_L8* vlSelf);

VL_ATTR_COLD void Vfft2d_core___024root___eval_stl(Vfft2d_core___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vfft2d_core__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vfft2d_core___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vfft2d_core___024root___stl_sequent__TOP__0(vlSelf);
        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d));
        Vfft2d_core_fft1d_core__N100_L8___stl_sequent__TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d__0((&vlSymsp->TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d));
        Vfft2d_core___024root___stl_sequent__TOP__1(vlSelf);
    }
}
