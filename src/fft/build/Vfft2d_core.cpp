// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vfft2d_core__pch.h"

//============================================================
// Constructors

Vfft2d_core::Vfft2d_core(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vfft2d_core__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , in_valid{vlSymsp->TOP.in_valid}
    , in_ready{vlSymsp->TOP.in_ready}
    , out_valid{vlSymsp->TOP.out_valid}
    , out_ready{vlSymsp->TOP.out_ready}
    , perf_done{vlSymsp->TOP.perf_done}
    , in_re{vlSymsp->TOP.in_re}
    , in_im{vlSymsp->TOP.in_im}
    , out_re{vlSymsp->TOP.out_re}
    , out_im{vlSymsp->TOP.out_im}
    , perf_cycles{vlSymsp->TOP.perf_cycles}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vfft2d_core::Vfft2d_core(const char* _vcname__)
    : Vfft2d_core(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vfft2d_core::~Vfft2d_core() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vfft2d_core___024root___eval_debug_assertions(Vfft2d_core___024root* vlSelf);
#endif  // VL_DEBUG
void Vfft2d_core___024root___eval_static(Vfft2d_core___024root* vlSelf);
void Vfft2d_core___024root___eval_initial(Vfft2d_core___024root* vlSelf);
void Vfft2d_core___024root___eval_settle(Vfft2d_core___024root* vlSelf);
void Vfft2d_core___024root___eval(Vfft2d_core___024root* vlSelf);

void Vfft2d_core::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vfft2d_core::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vfft2d_core___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vfft2d_core___024root___eval_static(&(vlSymsp->TOP));
        Vfft2d_core___024root___eval_initial(&(vlSymsp->TOP));
        Vfft2d_core___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vfft2d_core___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vfft2d_core::eventsPending() { return false; }

uint64_t Vfft2d_core::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vfft2d_core::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vfft2d_core___024root___eval_final(Vfft2d_core___024root* vlSelf);

VL_ATTR_COLD void Vfft2d_core::final() {
    Vfft2d_core___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vfft2d_core::hierName() const { return vlSymsp->name(); }
const char* Vfft2d_core::modelName() const { return "Vfft2d_core"; }
unsigned Vfft2d_core::threads() const { return 1; }
void Vfft2d_core::prepareClone() const { contextp()->prepareClone(); }
void Vfft2d_core::atClone() const {
    contextp()->threadPoolpOnClone();
}

//============================================================
// Trace configuration

VL_ATTR_COLD void Vfft2d_core::trace(VerilatedVcdC* tfp, int levels, int options) {
    vl_fatal(__FILE__, __LINE__, __FILE__,"'Vfft2d_core::trace()' called on model that was Verilated without --trace option");
}
