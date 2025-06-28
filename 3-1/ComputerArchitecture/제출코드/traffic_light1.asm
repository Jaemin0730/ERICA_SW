# Traffic Light Control System - MIPS Assembly
# Author: Jaemin Kim (2021050300)
# MARS 4.5 Compatible

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
    time_msg:           .asciiz "Current Time: "
    cycle_msg:          .asciiz "Signal Cycle: "
    remaining_msg:      .asciiz "Remaining Time: "
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
    
    # Statistics
    total_cycles:       .asciiz "Total Signal Cycles: "
    runtime_msg:        .asciiz "Total Runtime: "
    
    # Constants
    GREEN_TIME:         .word 5000    # Green light time (5 sec)
    YELLOW_TIME:        .word 2000    # Yellow light time (2 sec)
    RED_TIME:           .word 5000    # Red light time (5 sec)
    FLASH_TIME:         .word 1000    # Flash time (1 sec)

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
    # Save Stack Pointer
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Initial State Setting
    li $s0, 0       # Current State (0: NS_GREEN, 1: NS_YELLOW, 2: EW_GREEN, 3: EW_YELLOW)
    li $s1, 0       # Current Time Counter
    li $s2, 0       # Total Cycle Counter
    li $s3, 1       # System Mode (1: Normal, 2: Night, 3: Emergency)
    
    # Return
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Print Header Function
print_header:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Print Title
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
    
    # Print Mode Prompt
    li $v0, 4
    la $a0, mode_prompt
    syscall
    
    # Get User Input
    li $v0, 5
    syscall
    
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Normal Mode Execution Function
execute_normal_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Print Normal Mode Message
    li $v0, 4
    la $a0, normal_mode
    syscall
    
    # Start Traffic Signal Cycle
    li $t0, 0       # Local Counter Initialization
    
normal_cycle:
    # State 0: NS_GREEN, EW_RED
    jal print_ns_green_ew_red
    jal print_pedestrian_ew
    lw $a0, GREEN_TIME
    jal delay_ms
    jal update_cycle_counter
    
    # State 1: NS_YELLOW, EW_RED
    jal print_ns_yellow_ew_red
    jal print_pedestrian_wait
    lw $a0, YELLOW_TIME
    jal delay_ms
    
    # State 2: NS_RED, EW_GREEN
    jal print_ns_red_ew_green
    jal print_pedestrian_ns
    lw $a0, GREEN_TIME
    jal delay_ms
    jal update_cycle_counter
    
    # State 3: NS_RED, EW_YELLOW
    jal print_ns_red_ew_yellow
    jal print_pedestrian_wait
    lw $a0, YELLOW_TIME
    jal delay_ms
    
    # Check Cycle Completion (Repeat 3 times)
    addi $t0, $t0, 1
    blt $t0, 3, normal_cycle
    
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Night Mode Execution Function
execute_night_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Print Night Mode Message
    li $v0, 4
    la $a0, night_mode
    syscall
    
    li $t0, 0       # Local Counter
    
night_cycle:
    # NS Flashing
    jal print_night_flash_ns
    lw $a0, FLASH_TIME
    jal delay_ms
    
    # EW Flashing
    jal print_night_flash_ew
    lw $a0, FLASH_TIME
    jal delay_ms
    
    # Repeat Flashing Cycle (10 times)
    addi $t0, $t0, 1
    blt $t0, 10, night_cycle
    
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Emergency Mode Execution Function
execute_emergency_mode:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Print Emergency Mode Message
    li $v0, 4
    la $a0, emergency_mode
    syscall
    
    # All Directions RED (Emergency Situation)
    li $t0, 0
    
emergency_cycle:
    # All Directions RED
    li $v0, 4
    la $a0, ns_red_ew_red
    syscall
    
    lw $a0, RED_TIME
    jal delay_ms
    
    addi $t0, $t0, 1
    blt $t0, 5, emergency_cycle
    
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

# Time Info Print Function
print_time_info:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    
    # Print Current Cycle
    li $v0, 4
    la $a0, cycle_msg
    syscall
    
    li $v0, 1
    move $a0, $s2
    syscall
    
    li $v0, 4
    la $a0, seconds_msg
    syscall
    
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

# Delay Function (in milliseconds)
delay_ms:
    # Use MARS sleep syscall
    li $v0, 32      # sleep syscall
    # $a0 already contains delay time
    syscall
    jr $ra

# Cycle Counter Update
update_cycle_counter:
    addi $s2, $s2, 1
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
    
    la $a0, total_cycles
    syscall
    
    li $v0, 1
    move $a0, $s2
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
