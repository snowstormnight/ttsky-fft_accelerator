transcript on
if {[file exists work]} { vdel -lib work -all }
vlib work

# Compile DUT + TB
if {[catch {vlog -sv wiener_filter.v tb_FilterGen.v} msg]} {
    puts "Compile failed: $msg"
    quit -code 1
}

# Simulate
if {[catch {vsim -voptargs=+acc work.tb_FilterGen} msg]} {
    puts "Elaboration failed: $msg"
    quit -code 1
}
onfinish stop

# Helpful wave view
add wave -r sim:/tb_FilterGen/*
add wave -r sim:/tb_FilterGen/dut/*

run -all
puts "Simulation stopped. Inspect transcript/waves."
