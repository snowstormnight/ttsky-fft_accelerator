// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vfft2d_core__pch.h"
#include "Vfft2d_core.h"
#include "Vfft2d_core___024root.h"
#include "Vfft2d_core_fft1d_core__N100_L8.h"

// FUNCTIONS
Vfft2d_core__Syms::~Vfft2d_core__Syms()
{
}

Vfft2d_core__Syms::Vfft2d_core__Syms(VerilatedContext* contextp, const char* namep, Vfft2d_core* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
    , TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d{this, Verilated::catName(namep, "fft2d_core.g_fft[0].u_fft1d")}
    , TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d{this, Verilated::catName(namep, "fft2d_core.g_fft[1].u_fft1d")}
    , TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d{this, Verilated::catName(namep, "fft2d_core.g_fft[2].u_fft1d")}
    , TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d{this, Verilated::catName(namep, "fft2d_core.g_fft[3].u_fft1d")}
{
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    TOP.__PVT__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d = &TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d;
    TOP.__PVT__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d = &TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d;
    TOP.__PVT__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d = &TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d;
    TOP.__PVT__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d = &TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d;
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    TOP__fft2d_core__DOT__g_fft__BRA__0__KET____DOT__u_fft1d.__Vconfigure(true);
    TOP__fft2d_core__DOT__g_fft__BRA__1__KET____DOT__u_fft1d.__Vconfigure(false);
    TOP__fft2d_core__DOT__g_fft__BRA__2__KET____DOT__u_fft1d.__Vconfigure(false);
    TOP__fft2d_core__DOT__g_fft__BRA__3__KET____DOT__u_fft1d.__Vconfigure(false);
}
