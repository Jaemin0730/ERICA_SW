# Traffic Light Control System - MIPS Assembly (All Modes 누적 초 표시 통합본)
# Author: Jaemin Kim (2021050300) + ChatGPT 개선
# MARS 4.5 Compatible

# [Register Usage]
# $s0: Current state (0~3, state transitions with wrap-around)
# $s1: (Unused)
# $s2: Total signal cycles (count)
# $s3: System mode (1: Normal, 2: Night, 3: Emergency)
# $t0: Local cycle counter (per mode)
# $t1: Temp for time calculation

.data
    # System Title and Info
    title:      .asciiz "\n=== Traffic Light Control System ===\n"
    author:     .asciiz "Author: Jaemin Kim (2021050300)\n"
    divider:    .asciiz "===========================\n\n"

    # Traffic Light Status Messages
    ns_green_ew_red:    .asciiz "[NS: GREEN  | EW: RED   ] - North-South Traffic\n"
    ns_yellow_ew_red:   .asciiz "[NS: YELLOW | EW: RED   ] - North-South Prepare\n"
    ns_red_ew_green:    .asciiz "[NS: RED    | EW: GREEN ] - East-West Traffic\n"
    ns_red_ew_yellow:   .asciiz "[NS: RED    | EW: YELLOW] - East-West Prepare\n"
    ns_red_ew_red:      .asciiz "[NS: RED    | EW: RED   ] - Emergency Situation\n"

    # Pedestrian Signal Messages
    pedestrian_ns:      .asciiz "[Pedestrian: Cross North-South]\n"
    pedestrian_ew:      .asciiz "[Pedestrian: Cross East-West]\n"
    pedestrian_wait:    .asciiz "[Pedestrian: Wait (Red Light)]\n"

    # Time Info Messages
    elapsed_msg:        .asciiz "State Duration: +"
    s_msg:              .asciiz "s (Total: "
    close_msg:          .asciiz "s)\n"
    total_cycles:       .asciiz "Total Signal Cycles: "
    cycles_msg:         .asciiz "cycles\n"
    runtime_msg:        .asciiz "Total Runtime: "
    seconds_msg:        .asciiz "sec\n"

    # System Mode Messages
    normal_mode:        .asciiz "*** Normal Mode ***\n"
    night_mode:         .asciiz "*** Night Mode (Flashing) ***\n"
    emergency_mode:     .asciiz "*** Emergency Mode ***\n"

    # Night Mode Messages
    night_flash_ns:     .asciiz "[NS: FLASH  | EW: RED   ] - Night Flash\n"
    night_flash_ew:     .asciiz "[NS: RED    | EW: FLASH ] - Night Flash\n"

    # User Input Messages
    mode_prompt:        .asciiz "\nSelect Mode (1:Normal, 2:Night, 3:Emergency, 0:Exit): "
    continue_prompt:    .asciiz "Continue? (1:Yes, 0:No): "

    # Error Message
    invalid_input:      .asciiz "Invalid input. Please try again.\n"

    # Exit Message
    goodbye_msg:        .asciiz "\nExiting Traffic Light Control System.\n"

    # Constants
    GREEN_TIME:         .word 5000    # Green light time (5 sec)
    YELLOW_TIME:        .word 2000    # Yellow light time (2 sec)
    RED_TIME:           .word 5000    # Red light time (5 sec)
    FLASH_TIME:         .word 1000    # Flash time (1 sec)

    # Time accumulator (in seconds)
    total_seconds:      .word 0       # Total elapsed seconds

.text
.globl main

main:
    # Initialization
    jal initialize_system

    # Main Loop
main_loop:
    # Print System Header
    jal print_header

    # Get User Mode
    jal get_user_mode

    # Mode Selection
    beq $v0, 0, exit_system     # 0: Exit
    beq $v0, 1, normal_mode_exec # 1: Normal Mode
    beq $v0, 2, night_mode_exec  # 2: Night Mode
    beq $v0, 3, emergency_mode_exec # 3: Emergency Mode

    # Invalid Input
    jal print_invalid_input
    j main_loop

normal_mode_exec:
    jal execute_normal_mode
    j check_continue

night_mode_exec:
    jal execute_night_mode
    j check_continue

emergency_mode_exec:
    jal execute_emergency_mode
    j check_continue

check_continue:
    jal ask_continue
    beq $v0, 1, main_loop

exit_system:
    jal print_statistics
    jal print_goodbye
    li $v0, 10      # Exit System
    syscall

# System Initialization Function
initialize_system:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $s0, 0       # Current State
    li $s1, 0       # (Unused)
    li $s2, 0       # Total cycles
    li $s3, 1       # System mode (default Normal)
    li $t0, 0       # Local cycle counter
    li $t1, 0
    sw $t1, total_seconds
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Print Header Function
print_header:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, title
    syscall
    la $a0, author
    syscall
    la $a0, divider
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# User Mode Selection Function
get_user_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, mode_prompt
    syscall
    li $v0, 5
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Normal Mode Execution Function
execute_normal_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, normal_mode
    syscall
    li $t0, 0       # Local Counter Initialization

normal_cycle:
    # State 0: NS_GREEN, EW_RED (5s)
    jal print_ns_green_ew_red
    jal print_pedestrian_ew
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter

    # State 1: NS_YELLOW, EW_RED (2s)
    jal print_ns_yellow_ew_red
    jal print_pedestrian_wait
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms
    jal update_cycle_counter

    # State 2: NS_RED, EW_GREEN (5s)
    jal print_ns_red_ew_green
    jal print_pedestrian_ns
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter

    # State 3: NS_RED, EW_YELLOW (2s)
    jal print_ns_red_ew_yellow
    jal print_pedestrian_wait
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms
    jal update_cycle_counter

    addi $t0, $t0, 1
    blt $t0, 3, normal_cycle

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Night Mode Execution Function (각 점멸마다 누적 초/싸이클 표시)
execute_night_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, night_mode
    syscall
    li $t0, 0
night_cycle:
    # NS Flashing (1s)
    jal print_night_flash_ns
    li $a1, 1
    jal print_time_info
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms
    jal update_cycle_counter

    # EW Flashing (1s)
    jal print_night_flash_ew
    li $a1, 1
    jal print_time_info
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms
    jal update_cycle_counter

    addi $t0, $t0, 1
    blt $t0, 10, night_cycle
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Emergency Mode Execution Function (싸이클마다 누적 초/상태 시간 출력)
execute_emergency_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, emergency_mode
    syscall
    li $t0, 0
emergency_cycle:
    li $v0, 4
    la $a0, ns_red_ew_red
    syscall
    li $a1, 5
    jal print_time_info
    lw $a0, RED_TIME
    li $a1, 5
    jal delay_ms
    jal update_cycle_counter
    addi $t0, $t0, 1
    blt $t0, 5, emergency_cycle
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Individual State Print Functions (with per-state seconds)
print_ns_green_ew_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_green_ew_red
    syscall
    li $a1, 5
    jal print_time_info
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_yellow_ew_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_yellow_ew_red
    syscall
    li $a1, 2
    jal print_time_info
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_red_ew_green:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_red_ew_green
    syscall
    li $a1, 5
    jal print_time_info
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_red_ew_yellow:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_red_ew_yellow
    syscall
    li $a1, 2
    jal print_time_info
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_night_flash_ns:
    li $v0, 4
    la $a0, night_flash_ns
    syscall
    jr $ra

print_night_flash_ew:
    li $v0, 4
    la $a0, night_flash_ew
    syscall
    jr $ra

# Pedestrian Signal Print Functions
print_pedestrian_ns:
    li $v0, 4
    la $a0, pedestrian_ns
    syscall
    jr $ra

print_pedestrian_ew:
    li $v0, 4
    la $a0, pedestrian_ew
    syscall
    jr $ra

print_pedestrian_wait:
    li $v0, 4
    la $a0, pedestrian_wait
    syscall
    jr $ra

# Time Info Print Function: per-state and cumulative seconds
# $a1: 이번 상태의 초(초단위, 예: 5 또는 2)
print_time_info:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # Print "State Duration: +Xs (Total: Ys)"
    li $v0, 4
    la $a0, elapsed_msg    # "State Duration: +"
    syscall
    move $a0, $a1          # X (이번 상태 초)
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, s_msg          # "s (Total: "
    syscall
    lw $t1, total_seconds  # 누적초(이 상태 전까지)
    move $a0, $t1
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, close_msg      # "s)\n"
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Delay Function (in milliseconds)
delay_ms:
    li $v0, 32      # sleep syscall
    syscall
    jr $ra

# Cycle Counter Update (and accumulate seconds)
# $a1: 이번 상태의 초
update_cycle_counter:
    addi $s2, $s2, 1           # cycle count
    lw $t1, total_seconds
    add $t1, $t1, $a1          # $a1 has the current state's seconds
    sw $t1, total_seconds
    jr $ra

# Continue Check
ask_continue:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, continue_prompt
    syscall
    li $v0, 5
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Invalid Input Print
print_invalid_input:
    li $v0, 4
    la $a0, invalid_input
    syscall
    jr $ra

# Statistics Print
print_statistics:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
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
    la $a0, cycles_msg      # "cycles"로 출력
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
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Exit Message Print
print_goodbye:
    li $v0, 4
    la $a0, goodbye_msg
    syscall
    jr $ra