# Traffic Light Control System - MIPS Assembly (누적초 정확 표시 통합본 + 좌회전 추가)
# Author: Jaemin Kim (2021050300)
# MARS 4.5 Compatible

# [Register Usage]
# $s0: Current state (0~3, state transitions with wrap-around) - (사용 빈도 낮아짐, execute_normal_mode 내에서 순차 진행)
# $s1: (Unused)
# $s2: Total signal cycles (각 상태 변경 시 카운트)
# $s3: System mode (1: Normal, 2: Night, 3: Emergency)
# $t0: Local cycle counter (per mode)
# $t1: Temp for time calculation / total_seconds 로드용
# $t2: print_time_info 내 임시 누적 시간 계산용
# $t3: execute_normal_mode 내 ms 시간 로드용
# $t4: execute_normal_mode 내 초 변환용 (사용하지 않고 직접 초 단위 입력으로 변경)

.data
    # System Title and Info
    title:      .asciiz "\n=== Traffic Light Control System (with Left Turns) ===\n"
    author:     .asciiz "Author: Jaemin Kim (2021050300)\n"
    divider:    .asciiz "==================================================\n\n"

    # Traffic Light Status Messages
    ns_green_ew_red:    .asciiz "[NS: GREEN  | EW: RED   ] - North-South Traffic\n"
    ns_yellow_ew_red:   .asciiz "[NS: YELLOW | EW: RED   ] - North-South Prepare\n"
    ns_red_ew_green:    .asciiz "[NS: RED    | EW: GREEN ] - East-West Traffic\n"
    ns_red_ew_yellow:   .asciiz "[NS: RED    | EW: YELLOW] - East-West Prepare\n"
    ns_red_ew_red:      .asciiz "[NS: RED    | EW: RED   ] - Emergency Situation\n"
    # NEW Left Turn Messages
    ns_left_ew_red:     .asciiz "[NS: LEFT   | EW: RED   ] - North-South Left Turn\n"
    ew_left_ns_red:     .asciiz "[NS: RED    | EW: LEFT  ] - East-West Left Turn\n"


    # Pedestrian Signal Messages
    pedestrian_ns:      .asciiz "[Pedestrian: Cross North-South]\n"
    pedestrian_ew:      .asciiz "[Pedestrian: Cross East-West]\n"
    pedestrian_wait:    .asciiz "[Pedestrian: Wait (Red Light)]\n"

    # Time Info Messages
    elapsed_msg:        .asciiz "State Duration: +"
    s_msg:              .asciiz "s (Total: "
    close_msg:          .asciiz "s)\n"
    total_cycles:       .asciiz "Total Signal States Processed: " # "Cycle"의 의미를 상태 변경 횟수로 명확히 함
    cycles_msg:         .asciiz " states\n"
    runtime_msg:        .asciiz "Total Runtime: "
    seconds_msg:        .asciiz "sec\n"

    # System Mode Messages
    normal_mode:        .asciiz "*** Normal Mode (with Left Turns) ***\n"
    night_mode:         .asciiz "*** Night Mode (Flashing) ***\n"
    emergency_mode:     .asciiz "*** Emergency Mode ***\n"

    # Night Mode Messages
    night_flash_ns:     .asciiz "[NS: FLASH  | EW: RED   ] - Night Flash\n"
    night_flash_ew:     .asciiz "[NS: RED    | EW: FLASH ] - Night Flash\n"

    # User Input Messages
    mode_prompt:        .asciiz "\nSelect Mode (1:Normal with Left Turns, 2:Night, 3:Emergency, 0:Exit): "
    continue_prompt:    .asciiz "Continue? (1:Yes, 0:No): "

    # Error Message
    invalid_input:      .asciiz "Invalid input. Please try again.\n"

    # Exit Message
    goodbye_msg:        .asciiz "\nExiting Traffic Light Control System.\n"

    # Constants (in milliseconds)
    GREEN_TIME:         .word 5000    # Green light time (5 sec)
    YELLOW_TIME:        .word 2000    # Yellow light time (2 sec)
    RED_TIME:           .word 5000    # Red light time (5 sec) - Emergency Mode 용
    FLASH_TIME:         .word 1000    # Flash time (1 sec)
    LEFT_TURN_TIME:     .word 3000    # NEW: Left turn time (3 sec)

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
    beq $v0, 1, normal_mode_exec # 1: Normal Mode with Left Turns
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
    # if 0, fall through to exit_system

exit_system:
    jal print_statistics
    jal print_goodbye
    li $v0, 10      # Exit System
    syscall

# System Initialization Function
initialize_system:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $s0, 0       # Current State (less used now, sequence is fixed in normal mode)
    li $s1, 0       # (Unused)
    li $s2, 0       # Total signal states processed
    li $s3, 1       # System mode (default Normal)
    li $t0, 0       # Local cycle counter (for full sequences)
    sw $zero, total_seconds # Clears total_seconds
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

# Normal Mode Execution Function (with Left Turns)
execute_normal_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, normal_mode
    syscall
    li $t0, 0       # Local Counter for full 6-phase cycles initialization

normal_full_cycle_loop: # Renamed for clarity
    # Phase 1: NS_LEFT, EW_RED (3s)
    jal print_ns_left_ew_red
    jal print_pedestrian_wait     # Pedestrians wait
    lw $a2, total_seconds        
    li $a1, 3                     
    jal print_time_info
    li $a1, 3                   
    jal update_cycle_counter
    lw $a0, LEFT_TURN_TIME        
    li $a1, 3                     
    jal delay_ms

    # Phase 2: NS_GREEN, EW_RED (5s)
    jal print_ns_green_ew_red
    jal print_pedestrian_ns       # NS Pedestrians cross
    lw $a2, total_seconds
    li $a1, 5
    jal print_time_info
    li $a1, 5
    jal update_cycle_counter
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms

    # Phase 3: NS_YELLOW, EW_RED (2s)
    jal print_ns_yellow_ew_red
    jal print_pedestrian_wait     # Pedestrians wait
    lw $a2, total_seconds
    li $a1, 2
    jal print_time_info
    li $a1, 2
    jal update_cycle_counter
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms

    # Phase 4: EW_LEFT, NS_RED (3s)
    jal print_ew_left_ns_red
    jal print_pedestrian_wait     # Pedestrians wait
    lw $a2, total_seconds
    li $a1, 3
    jal print_time_info
    li $a1, 3
    jal update_cycle_counter
    lw $a0, LEFT_TURN_TIME
    li $a1, 3
    jal delay_ms

    # Phase 5: NS_RED, EW_GREEN (5s)
    jal print_ns_red_ew_green
    jal print_pedestrian_ew       # EW Pedestrians cross
    lw $a2, total_seconds
    li $a1, 5
    jal print_time_info
    li $a1, 5
    jal update_cycle_counter
    lw $a0, GREEN_TIME
    li $a1, 5
    jal delay_ms

    # Phase 6: NS_RED, EW_YELLOW (2s)
    jal print_ns_red_ew_yellow
    jal print_pedestrian_wait     # Pedestrians wait
    lw $a2, total_seconds
    li $a1, 2
    jal print_time_info
    li $a1, 2
    jal update_cycle_counter
    lw $a0, YELLOW_TIME
    li $a1, 2
    jal delay_ms

    addi $t0, $t0, 1              # Increment full cycle counter
    blt $t0, 3, normal_full_cycle_loop # Loop for 3 full 6-phase cycles

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
night_cycle_loop: # Renamed for clarity
    # NS Flashing (1s)
    jal print_night_flash_ns
    lw $a2, total_seconds
    li $a1, 1
    jal print_time_info
    li $a1, 1
    jal update_cycle_counter
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms

    # EW Flashing (1s)
    jal print_night_flash_ew
    lw $a2, total_seconds
    li $a1, 1
    jal print_time_info
    li $a1, 1
    jal update_cycle_counter
    lw $a0, FLASH_TIME
    li $a1, 1
    jal delay_ms

    addi $t0, $t0, 1
    blt $t0, 10, night_cycle_loop # Loop for 10 flash pairs
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Emergency Mode Execution Function
execute_emergency_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, emergency_mode
    syscall
    li $t0, 0
emergency_cycle_loop: # Renamed for clarity
    li $v0, 4
    la $a0, ns_red_ew_red
    syscall
    lw $a2, total_seconds
    li $a1, 5
    jal print_time_info
    li $a1, 5
    jal update_cycle_counter
    lw $a0, RED_TIME # Using RED_TIME constant
    li $a1, 5
    jal delay_ms
    addi $t0, $t0, 1
    blt $t0, 5, emergency_cycle_loop # Loop for 5 emergency phases
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Individual State Print Functions
print_ns_green_ew_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_green_ew_red
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_yellow_ew_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_yellow_ew_red
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_red_ew_green:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_red_ew_green
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ns_red_ew_yellow:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_red_ew_yellow
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# NEW Left Turn Print Functions
print_ns_left_ew_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ns_left_ew_red
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

print_ew_left_ns_red:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, ew_left_ns_red
    syscall
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
# End of NEW Left Turn Print Functions

print_night_flash_ns:
    li $v0, 4
    la $a0, night_flash_ns
    syscall
    jr $ra # Assuming $ra is managed by caller if needed for deeper calls

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
# $a2: 진입 전 누적 초 (total_seconds 값)
print_time_info:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    # $a1 = current state duration in seconds
    # $a2 = total seconds before this state

    li $v0, 4
    la $a0, elapsed_msg    # "State Duration: +"
    syscall

    move $a0, $a1          # X (이번 상태 초)
    li $v0, 1
    syscall

    li $v0, 4
    la $a0, s_msg          # "s (Total: "
    syscall

    addu $t2, $a1, $a2     # $t2 = 이전 누적 + 이번 초 (새로운 총 누적 시간)
    move $a0, $t2
    li $v0, 1
    syscall

    li $v0, 4
    la $a0, close_msg      # "s)\n"
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Delay Function (in milliseconds)
# $a0: milliseconds to delay
delay_ms:
    # $a0 already contains milliseconds
    li $v0, 32      # sleep syscall
    syscall
    jr $ra

# Cycle Counter Update (and accumulate seconds)
# $a1: 이번 상태의 초 (seconds for current state)
update_cycle_counter:
    addi $s2, $s2, 1           # Increment total states processed count
    lw $t1, total_seconds
    add $t1, $t1, $a1          # Add current state's seconds to total_seconds
    sw $t1, total_seconds
    jr $ra

# Continue Check
ask_continue:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, continue_prompt
    syscall
    li $v0, 5              # Read integer
    syscall                # $v0 contains the user's input (1 or 0)
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Invalid Input Print
print_invalid_input:
    li $v0, 4
    la $a0, invalid_input
    syscall
    jr $ra # No stack needed for simple leaf function

# Statistics Print
print_statistics:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    li $v0, 4
    la $a0, divider
    syscall

    li $v0, 4
    la $a0, total_cycles   # "Total Signal States Processed: "
    syscall
    li $v0, 1
    move $a0, $s2          # Print $s2 (total states)
    syscall
    li $v0, 4
    la $a0, cycles_msg     # " states\n"
    syscall

    li $v0, 4
    la $a0, runtime_msg    # "Total Runtime: "
    syscall
    lw $t1, total_seconds
    move $a0, $t1          # Print total accumulated seconds
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, seconds_msg    # "sec\n"
    syscall

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Exit Message Print
print_goodbye:
    li $v0, 4
    la $a0, goodbye_msg
    syscall
    jr $ra # No stack needed
