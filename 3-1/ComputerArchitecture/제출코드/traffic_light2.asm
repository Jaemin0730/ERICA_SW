# Traffic Light Control System - MIPS Assembly (Function Decomposition/Expanded Version)
# Author: Jaemin Kim (2021050300)
# MARS 4.5 Compatible

# [Register Usage]
# $s0: Current state (0~3, state transitions with wrap-around)
# $s1: Timer counter (not used / reserved for future use)
# $s2: Total signal cycles (count)
# $s3: System mode (1: Normal, 2: Night, 3: Emergency)
# $t0: Local cycle counter (per mode)
# $t1: Temp for time calculation
# $t2: Temp for argument passing

.data
    title:      .asciiz "\n=== Traffic Light Control System ===\n"
    author:     .asciiz "Author: Jaemin Kim (2021050300)\n"
    divider:    .asciiz "===========================\n\n"
    ns_green_ew_red:    .asciiz "[NS: GREEN  | EW: RED   ] - North-South Traffic\n"
    ns_yellow_ew_red:   .asciiz "[NS: YELLOW | EW: RED   ] - North-South Prepare\n"
    ns_red_ew_green:    .asciiz "[NS: RED    | EW: GREEN ] - East-West Traffic\n"
    ns_red_ew_yellow:   .asciiz "[NS: RED    | EW: YELLOW] - East-West Prepare\n"
    ns_red_ew_red:      .asciiz "[NS: RED    | EW: RED   ] - Emergency Situation\n"
    pedestrian_ns:      .asciiz "[Pedestrian: Cross North-South]\n"
    pedestrian_ew:      .asciiz "[Pedestrian: Cross East-West]\n"
    pedestrian_wait:    .asciiz "[Pedestrian: Wait (Red Light)]\n"
    elapsed_msg:        .asciiz "Elapsed Time: "
    seconds_msg:        .asciiz "sec\n"
    total_cycles:       .asciiz "Total Signal Cycles: "
    runtime_msg:        .asciiz "Total Runtime: "
    normal_mode:        .asciiz "*** Normal Mode ***\n"
    night_mode:         .asciiz "*** Night Mode (Flashing) ***\n"
    emergency_mode:     .asciiz "*** Emergency Mode ***\n"
    night_flash_ns:     .asciiz "[NS: FLASH  | EW: RED   ] - Night Flash\n"
    night_flash_ew:     .asciiz "[NS: RED    | EW: FLASH ] - Night Flash\n"
    mode_prompt:        .asciiz "\nSelect Mode (1:Normal, 2:Night, 3:Emergency, 0:Exit): "
    continue_prompt:    .asciiz "Continue? (1:Yes, 0:No): "
    invalid_input:      .asciiz "Invalid input. Please try again.\n"
    goodbye_msg:        .asciiz "\nExiting Traffic Light Control System.\n"
    GREEN_TIME:         .word 5000
    YELLOW_TIME:        .word 2000
    RED_TIME:           .word 5000
    FLASH_TIME:         .word 1000
    total_seconds:      .word 0

.text
.globl main

main:
    jal enter_main
    jal initialize_system
    jal main_loop
    jal exit_main
    jr $ra

#-------------------------------------------------
# Main Function Entrance and Exit
enter_main:  
    jr $ra

exit_main:   
    jr $ra

#-------------------------------------------------
# System Initialization
initialize_system:
    li $s0, 0       # Current State
    li $s1, 0       # Timer counter (unused)
    li $s2, 0       # Total signal cycles
    li $s3, 1       # Default mode: Normal
    li $t0, 0
    sw $t0, total_seconds
    jr $ra

#-------------------------------------------------
# Main Loop
main_loop:
    jal enter_main_loop
    jal print_header
    jal get_user_mode
    jal select_mode
    jal exit_main_loop
    jr $ra

enter_main_loop:
    jr $ra

exit_main_loop:
    jr $ra

#-------------------------------------------------
# Select Mode Branch
select_mode:
    beq $v0, 0, do_exit_system
    beq $v0, 1, normal_mode_exec
    beq $v0, 2, night_mode_exec
    beq $v0, 3, emergency_mode_exec
    jal print_invalid_input
    jal main_loop
    jr $ra

do_exit_system:
    jal print_statistics
    jal print_goodbye
    li $v0, 10
    syscall
    jr $ra

#-------------------------------------------------
# Normal Mode
normal_mode_exec:
    jal enter_normal_mode
    jal execute_normal_mode
    jal exit_normal_mode
    jal check_continue
    jr $ra

enter_normal_mode:
    li $v0, 4
    la $a0, normal_mode
    syscall
    jr $ra

exit_normal_mode:
    jr $ra

execute_normal_mode:
    li $t0, 0
    jal normal_mode_cycle
    jr $ra

normal_mode_cycle:
    jal enter_normal_mode_cycle
    jal normal_cycle_state_0
    jal normal_cycle_state_1
    jal normal_cycle_state_2
    jal normal_cycle_state_3
    addi $t0, $t0, 1
    blt $t0, 3, normal_mode_cycle
    jal exit_normal_mode_cycle
    jr $ra

enter_normal_mode_cycle:
    jr $ra

exit_normal_mode_cycle:
    jr $ra

normal_cycle_state_0:
    jal enter_state_0
    jal print_ns_green_ew_red
    jal print_pedestrian_ew
    jal state_0_time
    jal exit_state_0
    jr $ra

state_0_time:
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_state_0: jr $ra
exit_state_0: jr $ra

normal_cycle_state_1:
    jal enter_state_1
    jal print_ns_yellow_ew_red
    jal print_pedestrian_wait
    jal state_1_time
    jal exit_state_1
    jr $ra

state_1_time:
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_state_1: jr $ra
exit_state_1: jr $ra

normal_cycle_state_2:
    jal enter_state_2
    jal print_ns_red_ew_green
    jal print_pedestrian_ns
    jal state_2_time
    jal exit_state_2
    jr $ra

state_2_time:
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_state_2: jr $ra
exit_state_2: jr $ra

normal_cycle_state_3:
    jal enter_state_3
    jal print_ns_red_ew_yellow
    jal print_pedestrian_wait
    jal state_3_time
    jal exit_state_3
    jr $ra

state_3_time:
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_state_3: jr $ra
exit_state_3: jr $ra

#-------------------------------------------------
# Night Mode
night_mode_exec:
    jal enter_night_mode
    jal execute_night_mode
    jal exit_night_mode
    jal check_continue
    jr $ra

enter_night_mode:
    li $v0, 4
    la $a0, night_mode
    syscall
    jr $ra

exit_night_mode:
    jr $ra

execute_night_mode:
    li $t0, 0
    jal night_mode_cycle
    jr $ra

night_mode_cycle:
    jal enter_night_mode_cycle
    jal night_cycle_state_ns
    jal night_cycle_state_ew
    addi $t0, $t0, 1
    blt $t0, 10, night_mode_cycle
    jal exit_night_mode_cycle
    jr $ra

enter_night_mode_cycle: jr $ra
exit_night_mode_cycle: jr $ra

night_cycle_state_ns:
    jal enter_night_state_ns
    jal print_night_flash_ns
    jal night_ns_time
    jal exit_night_state_ns
    jr $ra

night_ns_time:
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_night_state_ns: jr $ra
exit_night_state_ns: jr $ra

night_cycle_state_ew:
    jal enter_night_state_ew
    jal print_night_flash_ew
    jal night_ew_time
    jal exit_night_state_ew
    jr $ra

night_ew_time:
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_night_state_ew: jr $ra
exit_night_state_ew: jr $ra

#-------------------------------------------------
# Emergency Mode
emergency_mode_exec:
    jal enter_emergency_mode
    jal execute_emergency_mode
    jal exit_emergency_mode
    jal check_continue
    jr $ra

enter_emergency_mode:
    li $v0, 4
    la $a0, emergency_mode
    syscall
    jr $ra

exit_emergency_mode:
    jr $ra

execute_emergency_mode:
    li $t0, 0
    jal emergency_mode_cycle
    jr $ra

emergency_mode_cycle:
    jal enter_emergency_mode_cycle
    jal emergency_cycle_state
    addi $t0, $t0, 1
    blt $t0, 5, emergency_mode_cycle
    jal exit_emergency_mode_cycle
    jr $ra

enter_emergency_mode_cycle: jr $ra
exit_emergency_mode_cycle: jr $ra

emergency_cycle_state:
    jal enter_emergency_state
    jal print_ns_red_ew_red
    jal emergency_time
    jal exit_emergency_state
    jr $ra

emergency_time:
    lw $a0, RED_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter
    jr $ra

enter_emergency_state: jr $ra
exit_emergency_state: jr $ra

#-------------------------------------------------
# Print Functions (With Entry/Exit)
print_header:
    jal enter_print_header
    li $v0, 4
    la $a0, title
    syscall
    la $a0, author
    syscall
    la $a0, divider
    syscall
    jal exit_print_header
    jr $ra

enter_print_header: jr $ra
exit_print_header: jr $ra

print_ns_green_ew_red:
    jal enter_print_ns_green_ew_red
    li $v0, 4
    la $a0, ns_green_ew_red
    syscall
    jal print_time_info
    jal exit_print_ns_green_ew_red
    jr $ra

enter_print_ns_green_ew_red: jr $ra
exit_print_ns_green_ew_red: jr $ra

print_ns_yellow_ew_red:
    jal enter_print_ns_yellow_ew_red
    li $v0, 4
    la $a0, ns_yellow_ew_red
    syscall
    jal print_time_info
    jal exit_print_ns_yellow_ew_red
    jr $ra

enter_print_ns_yellow_ew_red: jr $ra
exit_print_ns_yellow_ew_red: jr $ra

print_ns_red_ew_green:
    jal enter_print_ns_red_ew_green
    li $v0, 4
    la $a0, ns_red_ew_green
    syscall
    jal print_time_info
    jal exit_print_ns_red_ew_green
    jr $ra

enter_print_ns_red_ew_green: jr $ra
exit_print_ns_red_ew_green: jr $ra

print_ns_red_ew_yellow:
    jal enter_print_ns_red_ew_yellow
    li $v0, 4
    la $a0, ns_red_ew_yellow
    syscall
    jal print_time_info
    jal exit_print_ns_red_ew_yellow
    jr $ra

enter_print_ns_red_ew_yellow: jr $ra
exit_print_ns_red_ew_yellow: jr $ra

print_night_flash_ns:
    jal enter_print_night_flash_ns
    li $v0, 4
    la $a0, night_flash_ns
    syscall
    jal exit_print_night_flash_ns
    jr $ra

enter_print_night_flash_ns: jr $ra
exit_print_night_flash_ns: jr $ra

print_night_flash_ew:
    jal enter_print_night_flash_ew
    li $v0, 4
    la $a0, night_flash_ew
    syscall
    jal exit_print_night_flash_ew
    jr $ra

enter_print_night_flash_ew: jr $ra
exit_print_night_flash_ew: jr $ra

print_ns_red_ew_red:
    jal enter_print_ns_red_ew_red
    li $v0, 4
    la $a0, ns_red_ew_red
    syscall
    jal print_time_info
    jal exit_print_ns_red_ew_red
    jr $ra

enter_print_ns_red_ew_red: jr $ra
exit_print_ns_red_ew_red: jr $ra

print_pedestrian_ns:
    jal enter_print_pedestrian_ns
    li $v0, 4
    la $a0, pedestrian_ns
    syscall
    jal exit_print_pedestrian_ns
    jr $ra

enter_print_pedestrian_ns: jr $ra
exit_print_pedestrian_ns: jr $ra

print_pedestrian_ew:
    jal enter_print_pedestrian_ew
    li $v0, 4
    la $a0, pedestrian_ew
    syscall
    jal exit_print_pedestrian_ew
    jr $ra

enter_print_pedestrian_ew: jr $ra
exit_print_pedestrian_ew: jr $ra

print_pedestrian_wait:
    jal enter_print_pedestrian_wait
    li $v0, 4
    la $a0, pedestrian_wait
    syscall
    jal exit_print_pedestrian_wait
    jr $ra

enter_print_pedestrian_wait: jr $ra
exit_print_pedestrian_wait: jr $ra

print_time_info:
    jal enter_print_time_info
    li $v0, 4
    la $a0, elapsed_msg
    syscall
    lw $t1, total_seconds
    move $a0, $t1
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, seconds_msg
    syscall
    jal exit_print_time_info
    jr $ra

enter_print_time_info: jr $ra
exit_print_time_info: jr $ra

print_invalid_input:
    jal enter_print_invalid_input
    li $v0, 4
    la $a0, invalid_input
    syscall
    jal exit_print_invalid_input
    jr $ra

enter_print_invalid_input: jr $ra
exit_print_invalid_input: jr $ra

#-------------------------------------------------
# Input & User Functions
get_user_mode:
    jal enter_get_user_mode
    li $v0, 4
    la $a0, mode_prompt
    syscall
    li $v0, 5
    syscall
    jal exit_get_user_mode
    jr $ra

enter_get_user_mode: jr $ra
exit_get_user_mode: jr $ra

ask_continue:
    jal enter_ask_continue
    li $v0, 4
    la $a0, continue_prompt
    syscall
    li $v0, 5
    syscall
    jal exit_ask_continue
    jr $ra

enter_ask_continue: jr $ra
exit_ask_continue: jr $ra

check_continue:
    beq $v0, 1, main_loop
    jr $ra

#-------------------------------------------------
# Time/State/Counter Update Functions
update_cycle_counter:
    jal enter_update_cycle_counter
    addi $s2, $s2, 1
    lw $t1, total_seconds
    add $t1, $t1, $a1
    sw $t1, total_seconds
    jal exit_update_cycle_counter
    jr $ra

enter_update_cycle_counter: jr $ra
exit_update_cycle_counter: jr $ra

delay_ms:
    jal enter_delay_ms
    li $v0, 32
    syscall
    jal exit_delay_ms
    jr $ra

enter_delay_ms: jr $ra
exit_delay_ms: jr $ra

#-------------------------------------------------
# Statistics and End Print
print_statistics:
    jal enter_print_statistics
    li $v0, 4
    la $a0, divider
    syscall
    li $v0, 4
    la $a0, total_cycles
    syscall
    li $v0, 1
    move $a0, $s2
    syscall
    li $v0, 4
    la $a0, seconds_msg
    syscall
    li $v0, 4
    la $a0, runtime_msg
    syscall
    lw $t1, total_seconds
    move $a0, $t1
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, seconds_msg
    syscall
    jal exit_print_statistics
    jr $ra

enter_print_statistics: jr $ra
exit_print_statistics: jr $ra

print_goodbye:
    li $v0, 4
    la $a0, goodbye_msg
    syscall
