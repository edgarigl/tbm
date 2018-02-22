#include <aarch64-regs-asm.h>
#include <aarch64-smc-asm.h>

.global switch_to_el1

switch_to_el1:
    stp x29, x30, [sp, #-16]!

    msr elr_el3, x0

    mrs x0, scr_el3
    mov x2, #(SCR_NS | SCR_FIQ | SCR_RW)
    orr x0, x0, x2
    msr scr_el3, x0

    mrs x0, hcr_el2
    mov x2, #(HCR_RW)
    orr x0, x0, x2
    msr hcr_el2, x0

    mov x0, #(1 << 2 | 1)
    msr spsr_el3, x0

    msr sctlr_el1, xzr

    ldr x0, =el1_stack_bottom
    msr sp_el1, x0

    ldr x30, =switch_back_to_el3
    mov x0, x1
    eret


switch_back_to_el3:
    ldr x0, =SMC_TBM_RAISE_EL
    mov x1, #3
    mov x2, xzr
    mov x3, xzr
    mov x4, xzr
    mov x5, xzr
    mov x6, xzr
    mov x7, xzr
    smc 0

    ldp x29, x30, [sp], #16
    ret


    .data
el1_stack:  .skip 0x8000
el1_stack_bottom:
