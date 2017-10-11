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

static inline void enable_nested_irq(enum e_irq_type type, int cur_el,
                                     struct nested_irq_ctx *ctx)
{
    ctx->spsr = aarch64_spsr(cur_el);
    ctx->elr = aarch64_elr(cur_el);

#if 0
    switch (type) {
    case IRQ:
        local_cpu_ei();
        break;

    case FIQ:
        local_cpu_fiq_ei();
        break;
    }
#else
        local_cpu_ei();
        local_cpu_fiq_ei();
#endif
}

static inline void disable_nested_irq(enum e_irq_type type, int cur_el,
                                      struct nested_irq_ctx *ctx)
{
#if 0
    switch (type) {
    case IRQ:
        local_cpu_di();
        break;

    case FIQ:
        local_cpu_fiq_di();
        break;
    }
#else
        local_cpu_fiq_di();
        local_cpu_di();
#endif

    aarch64_set_spsr(cur_el, ctx->spsr);
    aarch64_set_elr(cur_el, ctx->elr);
}


/* XXX Platform specific */
#include "drivers/arm/arch-timer.h"

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
