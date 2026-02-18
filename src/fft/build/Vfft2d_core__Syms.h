// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VFFT2D_CORE__SYMS_H_
#define VERILATED_VFFT2D_CORE__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vfft2d_core.h"

// INCLUDE MODULE CLASSES
#include "Vfft2d_core___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vfft2d_core__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vfft2d_core* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vfft2d_core___024root          TOP;

    // CONSTRUCTORS
    Vfft2d_core__Syms(VerilatedContext* contextp, const char* namep, Vfft2d_core* modelp);
    ~Vfft2d_core__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
