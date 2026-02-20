transcript on

if {[file exists work]} {
    catch {vdel -lib work -all}
}

vlib work
vmap work work

vlog -sv ifft_core.sv tb_ifft_core.sv
vsim -voptargs=+acc tb_ifft_core

add wave -r /*
add wave -radix signed sim:/tb_ifft_core/exp_re
add wave -radix signed sim:/tb_ifft_core/exp_im
add wave sim:/tb_ifft_core/err_count
run -all

