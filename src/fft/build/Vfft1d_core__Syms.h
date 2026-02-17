// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VFFT1D_CORE__SYMS_H_
#define VERILATED_VFFT1D_CORE__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vfft1d_core.h"

// INCLUDE MODULE CLASSES
#include "Vfft1d_core___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vfft1d_core__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vfft1d_core* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vfft1d_core___024root          TOP;

    // CONSTRUCTORS
    Vfft1d_core__Syms(VerilatedContext* contextp, const char* namep, Vfft1d_core* modelp);
    ~Vfft1d_core__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
