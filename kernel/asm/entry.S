    .section .text.entry
    .globl _start
_start:
    la sp, boot_stack_top
    call kernel_init

    .section .bss.stack
    .globl boot_stack_top

boot_stack:
    .space 4096 * 16 # 16KB for stack
boot_stack_top: 