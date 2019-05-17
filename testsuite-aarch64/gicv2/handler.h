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

#ifndef _TESTSUITE_VEXPRESS_HANDLER_H
#define _TESTSUITE_VEXPRESS_HANDLER_H

#include <stdbool.h>
#include <stdint.h>

#include "sys.h"

struct timer_info {
    bool enabled;
    uint32_t tick_divisor;
    int el;
    int lvl;
    unsigned int wait_us;
    bool wait_for_token;
    const struct handler_action* actions;
    const struct handler_action* vcpu_actions;
};

struct irq_ctx;
typedef void (*handler_action_fn)(struct irq_ctx*, void* opaque);

struct handler_action {
    handler_action_fn fn;
    void *opaque;
};

#define HA_PARAM(p) ((void*)(p))
#define HA_END  { NULL, NULL }

/* actions */
void wait_us(struct irq_ctx *ctx, void *opaque);

void timer_ack(struct irq_ctx *ctx, void *opaque);
void timer_rearm(struct irq_ctx *ctx, void *opaque);
void timer_assert_el(struct irq_ctx *ctx, void *opaque);
void timer_assert_lvl(struct irq_ctx *ctx, void *opaque);
void timer_set_divisor(struct irq_ctx *ctx, void *opaque);

void nested_irq_enable(struct irq_ctx *ctx, void *opaque);
void nested_irq_disable(struct irq_ctx *ctx, void *opaque);

void gic_eoi(struct irq_ctx *ctx, void *opaque);
void gic_eoi_inexistant_irq(struct irq_ctx *ctx, void *opaque);
void gic_deactivate_irq(struct irq_ctx *ctx, void *opaque);
void gic_deactivate_inexistant_irq(struct irq_ctx *ctx, void *opaque);
void gic_assert_running_prio(struct irq_ctx *ctx, void *opaque);

void token_reset_self(struct irq_ctx *ctx, void *opaque);
void token_set_self(struct irq_ctx *ctx, void *opaque);
void token_wait_self(struct irq_ctx *ctx, void *opaque);
void token_assert_self(struct irq_ctx *ctx, void *opaque);
void token_set_parent(struct irq_ctx *ctx, void *opaque);
void token_assert_parent(struct irq_ctx *ctx, void *opaque);
void token_reset_lvl(struct irq_ctx *ctx, void *opaque);
void token_set_lvl(struct irq_ctx *ctx, void *opaque);
void token_assert_true_lvl(struct irq_ctx *ctx, void *opaque);
void token_assert_false_lvl(struct irq_ctx *ctx, void *opaque);

struct virt_inject_irq_params {
    bool hw;
    bool grp1;
    int prio;
    int phys_id;
    int virt_id;
};

void virt_inject_irq(struct irq_ctx *ctx, void *opaque);
void virt_assert_maint_irq(struct irq_ctx *ctx, void *opaque);
void virt_enable_maint_irq(struct irq_ctx *ctx, void *opaque);
void virt_disable_maint_irq(struct irq_ctx *ctx, void *opaque);
void virt_assert_eoicount(struct irq_ctx *ctx, void *opaque);
void virt_reset_eoicount(struct irq_ctx *ctx, void *opaque);
void virt_assert_eisr_entry(struct irq_ctx *ctx, void *opaque);
void virt_clear_eoi_in_lr(struct irq_ctx *ctx, void *opaque);

/* handlers */
void timer_fiq_h(struct excp_frame *f);
void timer_irq_h(struct excp_frame *f);

#endif

