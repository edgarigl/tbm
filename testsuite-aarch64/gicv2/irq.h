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

#ifndef _TESTSUITE_VEXPRESS_IRQ_H
#define _TESTSUITE_VEXPRESS_IRQ_H

#include <stdint.h>

enum e_irq_type {
    IRQ = 0, FIQ,
};

struct nested_irq_ctx {
    uint64_t spsr;
    uint64_t elr;
};

static inline void enable_nested_irq(int cur_el, struct nested_irq_ctx *ctx)
{
    ctx->spsr = aarch64_spsr(cur_el);
    ctx->elr = aarch64_elr(cur_el);

    local_cpu_ei();
    local_cpu_fiq_ei();
}

static inline void disable_nested_irq(int cur_el, struct nested_irq_ctx *ctx)
{
    local_cpu_fiq_di();
    local_cpu_di();

    aarch64_set_spsr(cur_el, ctx->spsr);
    aarch64_set_elr(cur_el, ctx->elr);
}


/* XXX Platform specific */
#include "drivers/arm/arch-timer.h"

#define MAINTENANCE_IRQ (16 + 9)

#define HYP_TIMER_IRQ (16 + 10)
#define VIRT_TIMER_IRQ (16 + 11)
#define SEC_TIMER_IRQ (16 + 13)
#define PHYS_TIMER_IRQ (16 + 14)

static const int TIMER_IRQ_TO_IDX[] = {
    [VIRT_TIMER_IRQ] = TIMER_VIRT,
    [PHYS_TIMER_IRQ] = TIMER_PHYS,
    [HYP_TIMER_IRQ] = TIMER_HYP,
    [SEC_TIMER_IRQ] = TIMER_PHYS_SEC,
};

static const int TIMER_IDX_TO_IRQ[] = {
    [TIMER_VIRT] = VIRT_TIMER_IRQ,
    [TIMER_PHYS] = PHYS_TIMER_IRQ,
    [TIMER_HYP] = HYP_TIMER_IRQ,
    [TIMER_PHYS_SEC] = SEC_TIMER_IRQ,
};

static const char * const TIMER_LABEL[] = {
    [TIMER_VIRT] = "virt",
    [TIMER_PHYS] = "phys",
    [TIMER_HYP] = "hyp",
    [TIMER_PHYS_SEC] = "sec",
};

#endif
