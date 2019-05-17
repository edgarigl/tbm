/*
 * ARM GICv2 test suite
 *
 * Copyright (c) 2018-2019 Luc Michel <luc.michel@greensocs.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#include <aarch64-regs-asm.h>
#include <aarch64-smc-asm.h>

.global switch_to_el1, switch_to_virt_mode_el1

switch_to_el1:
    mrs x3, hcr_el2
    ldr x2, =(HCR_RW)
    orr x3, x3, x2
    msr hcr_el2, x3

    b _switch_to_el1


switch_to_virt_mode_el1:
    mrs x3, hcr_el2
    ldr x2, =(HCR_RW | HCR_IMO | HCR_FMO)
    orr x3, x3, x2
    msr hcr_el2, x3


_switch_to_el1:
    stp x29, x30, [sp, #-16]!

    msr elr_el3, x0

    mrs x0, scr_el3
    mov x2, #(SCR_NS | SCR_FIQ | SCR_RW)
    orr x0, x0, x2
    msr scr_el3, x0

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
