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

#define _MINIC_SOURCE

#include "handler.h"
#include "irq.h"
#include "test_entry.h"
#include "trace.h"
#include "switch.h"
#include "gic.h"
#include "misc.h"

struct irq_ctx {
    const struct timer_info *timer_info;
    struct nested_irq_ctx nested_irq_ctx;

    int el;
    enum e_irq_type type;
    uint32_t irq;
    int timer_id;
    bool is_vcpu;
    phys_addr_t gicc_base;
};

static uint64_t counter[4] = { 0 };
static uint32_t freq[4];

static bool token[4] = { false };
static volatile int irq_lvl = -1;
static int max_lvl = -1;

static const struct test_info *cur_test = NULL;

void udelay(unsigned int us)
{
    uint64_t cur = a64_read_timer_cnt(TIMER_PHYS);
    uint64_t until, cntfrq;

    aarch64_mrs(cntfrq, "cntfrq_el0");
    until = cur + (cntfrq * us / 1000);
    while (a64_read_timer_cnt(TIMER_PHYS) < until) {
        cpu_relax();
    }
}

static inline int timer_get_lvl(int id)
{
    assert(cur_test);
    assert(id < 4);
    assert(cur_test->virt_mode || cur_test->timers[id].enabled);

    return cur_test->timers[id].lvl;
}

static inline void _timer_ack(int timer)
{
    assert(timer < 4);

    a64_write_timer_ctl(timer, 0);
    a64_write_timer_tval(timer, 1);
    ibarrier();
}

static inline void _timer_rearm(int timer)
{
    assert(timer < 4);

#ifdef DEBUG
    uint64_t cur = counter[timer];
#endif

    counter[timer] += freq[timer];
    a64_write_timer_tval(timer, freq[timer]);
    a64_write_timer_ctl(timer, T_ENABLE);
    ibarrier();

    DPRINTF("[%s] rearm: before:%llu, after:%llu\n",
            TIMER_LABEL[timer], cur, counter[timer]);
}

static inline bool is_maintenance_irq(uint32_t irq)
{
    return irq == MAINTENANCE_IRQ;
}

static inline int get_timer_id(uint32_t irq)
{
    assert((irq >= HYP_TIMER_IRQ)
           && (irq <= PHYS_TIMER_IRQ)
           && (irq != (16+12)));

    return TIMER_IRQ_TO_IDX[irq];
}

static inline void reset_token(int id)
{
    token[timer_get_lvl(id)] = false;
}

static inline bool token_is_set(int id)
{
    if (timer_get_lvl(id) == max_lvl) {
        return true;
    } else {
        return token[timer_get_lvl(id)];
    }
}

static inline void set_lower_token(int id)
{
    if (timer_get_lvl(id)) {
        token[timer_get_lvl(id)-1] = true;
    }
}

void wait_us(struct irq_ctx *ctx, void *opaque)
{
    udelay((uintptr_t)opaque);
}

void timer_ack(struct irq_ctx *ctx, void *opaque)
{
    _timer_ack(ctx->timer_id);
}

static uint64_t get_current_tick(void)
{
    uint64_t now;

    aarch64_mrs(now, "cntvct_el0");
    mb();
    ibarrier();

    return now;
}

void timer_rearm(struct irq_ctx *ctx, void *opaque)
{
    uint64_t now = get_current_tick();

    counter[ctx->timer_id] = now;
    _timer_rearm(ctx->timer_id);
}

void timer_set_divisor(struct irq_ctx *ctx, void *opaque)
{
    uintptr_t divisor = (uintptr_t) opaque;
    uint32_t cntfrq;

    aarch64_mrs(cntfrq, "cntfrq_el0");
    freq[ctx->timer_id] = cntfrq / divisor;
}

void timer_assert_el(struct irq_ctx *ctx, void *opaque)
{
    assert(ctx->timer_info->el == ctx->el);
}

void timer_assert_lvl(struct irq_ctx *ctx, void *opaque)
{
    assert(ctx->timer_info->lvl == irq_lvl);
}

void nested_irq_enable(struct irq_ctx *ctx, void *opaque)
{
    enable_nested_irq(ctx->el, &ctx->nested_irq_ctx);
}

void nested_irq_disable(struct irq_ctx *ctx, void *opaque)
{
    disable_nested_irq(ctx->el, &ctx->nested_irq_ctx);
}

void gic_eoi(struct irq_ctx *ctx, void *opaque)
{
    int rpr_before = gic_running_prio(ctx->gicc_base), rpr_after;
    DPRINTF("about to end irq %u. Running prio before: %u\n", ctx->irq, rpr_before);

    gic_end_of_irq(ctx->gicc_base, ctx->irq);

    rpr_after = gic_running_prio(ctx->gicc_base);
    DPRINTF("after: %u\n", rpr_after);

    assert(rpr_before <= rpr_after);
}

void gic_eoi_inexistant_irq(struct irq_ctx *ctx, void *opaque)
{
    const uint32_t WRONG = 42;
#ifdef DEBUG
    int rpr_before = gic_running_prio(ctx->gicc_base), rpr_after;
    uint32_t apr_before = readl(ctx->gicc_base + GICC_APR), apr_after;
#endif

    DPRINTF("ending a wrong irq %u. Running prio before: %u\n", WRONG, rpr_before);
    DPRINTF("APR before: %x\n", apr_before);
    gic_end_of_irq(ctx->gicc_base, WRONG);

#ifdef DEBUG
    rpr_after = gic_running_prio(ctx->gicc_base);
    apr_after = readl(ctx->gicc_base + GICC_APR);
#endif

    DPRINTF("running prio after: %u\n", rpr_after);
    DPRINTF("APR after: %u\n", apr_after);
}

void gic_deactivate_irq(struct irq_ctx *ctx, void *opaque)
{
    const struct gic_cpu_info *info;

    info = ctx->is_vcpu ? &cur_test->gic.vcpu : &cur_test->gic.cpu;

    if (!info->eoi_mode) {
        return;
    }

    DPRINTF("deactivating irq %u\n", ctx->irq);

    writel(ctx->gicc_base + GICC_DIR, ctx->irq);
    mb();
    ibarrier();
}

void gic_deactivate_inexistant_irq(struct irq_ctx *ctx, void *opaque)
{
    const uint32_t WRONG = 42;

    DPRINTF("deactivating a wrong irq %u.\n", WRONG);

    writel(ctx->gicc_base + GICC_DIR, WRONG);
    mb();
    ibarrier();
}

void gic_assert_running_prio(struct irq_ctx *ctx, void *opaque)
{
    uint8_t rp = (uintptr_t) opaque;

    assert(gic_running_prio(ctx->gicc_base) == rp);
}

void token_reset_self(struct irq_ctx *ctx, void *opaque)
{
    token[timer_get_lvl(ctx->timer_id)] = false;
}

void token_set_self(struct irq_ctx *ctx, void *opaque)
{
    token[timer_get_lvl(ctx->timer_id)] = true;
}

void token_wait_self(struct irq_ctx *ctx, void *opaque)
{
    DPRINTF("wfi waiting for token\n");
    while(!token_is_set(ctx->timer_id)) {
        cpu_wfi();
    }
}

void token_assert_self(struct irq_ctx *ctx, void *opaque)
{
    bool val = (bool) opaque;
    assert(token[timer_get_lvl(ctx->timer_id)] == val);
}

void token_set_parent(struct irq_ctx *ctx, void *opaque)
{
    if (timer_get_lvl(ctx->timer_id)) {
        token[timer_get_lvl(ctx->timer_id)-1] = true;
    }
}

void token_assert_parent(struct irq_ctx *ctx, void *opaque)
{
    bool val = (bool) opaque;

    if (timer_get_lvl(ctx->timer_id)) {
        assert(token[timer_get_lvl(ctx->timer_id)-1] == val);
    }
}

void token_set_lvl(struct irq_ctx *ctx, void *opaque)
{
    size_t lvl = (bool) opaque;

    assert(lvl < 4);
    token[lvl] = true;
}

void token_reset_lvl(struct irq_ctx *ctx, void *opaque)
{
    size_t lvl = (bool) opaque;

    assert(lvl < 4);
    token[lvl] = false;
}

void token_assert_true_lvl(struct irq_ctx *ctx, void *opaque)
{
    size_t lvl = (bool) opaque;

    assert(lvl < 4);
    assert(token[lvl]);
}

void token_assert_false_lvl(struct irq_ctx *ctx, void *opaque)
{
    size_t lvl = (bool) opaque;

    assert(lvl < 4);
    assert(!token[lvl]);
}

#ifdef GIC_VCPU_BASE
static void check_elrsr(int lr_num, bool expect_free)
{
    int elrsr;

    if (lr_num >= 32) {
        elrsr = gich_read_elrsr1();
        lr_num -= 32;
    } else {
        elrsr = gich_read_elrsr0();
    }

    assert(((elrsr >> lr_num) & 0x1) == expect_free);
}

void virt_inject_irq(struct irq_ctx *ctx, void *opaque)
{
    const struct virt_inject_irq_params *p;
    p = (struct virt_inject_irq_params *) opaque;

    int next_lr = gich_get_next_lr_entry();
    check_elrsr(next_lr, true);

    DPRINTF("injecting irq %d at lr entry number %d\n",
            p->virt_id, next_lr);

    gich_set_lr_entry(next_lr, p->hw, p->grp1,
                      0x1, /* pending */
                      p->prio, p->phys_id, p->virt_id);

    check_elrsr(next_lr, false);
}

void virt_assert_maint_irq(struct irq_ctx *ctx, void *opaque)
{
    uint32_t misr_exp = (uintptr_t) opaque;
    uint32_t misr_act = gich_read_misr();

    DPRINTF("H_MISR is %x\n", misr_act);
    assert(misr_exp == misr_act);
}

void virt_enable_maint_irq(struct irq_ctx *ctx, void *opaque)
{
    uint32_t enable_mask = (uintptr_t) opaque;
    uint32_t h_hcr = gich_read_hcr();

    h_hcr |= enable_mask;
    gich_write_hcr(h_hcr);
}

void virt_disable_maint_irq(struct irq_ctx *ctx, void *opaque)
{
    uint32_t disable_mask = (uintptr_t) opaque;
    uint32_t h_hcr = gich_read_hcr();

    h_hcr &= ~disable_mask;
    gich_write_hcr(h_hcr);
}

void virt_assert_eoicount(struct irq_ctx *ctx, void *opaque)
{
    uintptr_t expected = (uintptr_t) opaque;
    uint32_t count = gich_read_hcr() >> 27;

    DPRINTF("hcr eoicount is %u\n", count);
    assert(expected == count);
}

void virt_reset_eoicount(struct irq_ctx *ctx, void *opaque)
{
    uint32_t hcr = gich_read_hcr();

    hcr &= 0x07ffffff;
    gich_write_hcr(hcr);
}

void virt_assert_eisr_entry(struct irq_ctx *ctx, void *opaque)
{
    uint32_t assert_mask = (uintptr_t) opaque;
    uint32_t eisr = gich_read_eisr0();

    assert(assert_mask == eisr);
    assert(gich_read_eisr1() == 0);
}

void virt_clear_eoi_in_lr(struct irq_ctx *ctx, void *opaque)
{
    uint32_t lr = (uintptr_t) opaque;

    gich_set_lr_entry(lr, 0, 0, 0, 0, 0, 0);
}
#endif

static inline bool is_vcpu(int cur_el)
{
    if (!cur_test->virt_mode) {
        return false;
    }

    return cur_el == 1;
}

static inline phys_addr_t get_gicc_base(bool is_vcpu)
{
#ifdef GIC_VCPU_BASE
    return is_vcpu ? GIC_VCPU_BASE : GIC_CPU_BASE;
#else
    assert(is_vcpu == false);
    return GIC_CPU_BASE;
#endif
}

static inline void generic_handler(enum e_irq_type type)
{
    struct irq_ctx ctx;
    const struct handler_action *actions;

    irq_lvl++;

    ctx.el = aarch64_current_el();
    ctx.type = type;
    ctx.is_vcpu = is_vcpu(ctx.el);
    ctx.gicc_base = get_gicc_base(ctx.is_vcpu);
    ctx.irq = gic_ack_irq(ctx.gicc_base);

    if (!is_maintenance_irq(ctx.irq)) {
        ctx.timer_id = get_timer_id(ctx.irq);
        ctx.timer_info = &cur_test->timers[ctx.timer_id];
    } else {
        ctx.timer_id = -1;
        ctx.timer_info = NULL;
    }

    DPRINTF("enter [%s]%s el:%d, lvl:%d\n",
            ctx.timer_info ? TIMER_LABEL[ctx.timer_id] : "maint",
            ctx.is_vcpu ? " [vcpu]" : "", ctx.el, irq_lvl);

    if (!ctx.timer_info) {
        /* Maintenance IRQ actions */
        assert(cur_test->virt_mode);
        assert(!ctx.is_vcpu);
        assert(cur_test->maintenance_irq_actions != NULL);
        actions = cur_test->maintenance_irq_actions;
    } else if (ctx.is_vcpu && ctx.timer_info->vcpu_actions) {
        /* timer actions for VCPU  */
        actions = ctx.timer_info->vcpu_actions;
    } else if (ctx.timer_info->actions) {
        /* timer actions */
        actions = ctx.timer_info->actions;
    } else {
        /* main actions */
        actions = cur_test->actions;
    }

    while (actions->fn) {
        actions->fn(&ctx, actions->opaque);
        actions++;
    }

    DPRINTF("leave [%s]%s lvl:%d\n\n",
            ctx.timer_info ? TIMER_LABEL[ctx.timer_id] : "maint",
            ctx.is_vcpu ? " [vcpu]" : "", irq_lvl);

    irq_lvl--;
}

void timer_fiq_h(struct excp_frame *f)
{
    generic_handler(FIQ);
}

void timer_irq_h(struct excp_frame *f)
{
    generic_handler(IRQ);
}

static void configure_gic(const struct gic_info *info)
{
    gic_configure(info);
    ibarrier();
}

static void teardown_git(const struct gic_info *info)
{
    gic_teardown(info);
    ibarrier();
}


static void configure_timers(const struct timer_info *info)
{
    uint32_t cntfrq;
    int i;

    max_lvl = 0;

    aarch64_msr("cntvoff_el2", 0);
    aarch64_mrs(cntfrq, "cntfrq_el0");
    DPRINTF("cntfrq=%d Hz\n", cntfrq);

    for (i = 0; i < 4; i++) {
        freq[i] = cntfrq / info[i].tick_divisor;
        max_lvl = info[i].lvl > max_lvl ? info[i].lvl : max_lvl;
        token[i] = false;
    }
}

static inline bool config_timer_in_el(const struct timer_info *t,
                                      int cur_el, bool virt_mode)
{
    return t->el == cur_el || (cur_el == 3 && t->el == 2 && virt_mode);
}

static void enable_timers(void)
{
    int i;
    const struct timer_info *t;
    int cur_el = aarch64_current_el();

    assert(cur_test);

    for (i = 0; i < 4; i++) {
        t = cur_test->timers + i;

        if (!t->enabled) {
            continue;
        }

        if (!config_timer_in_el(t, cur_el, cur_test->virt_mode)) {
            continue;
        }

        counter[i] = a64_read_timer_cnt(i);
        _timer_rearm(i);
        a64_write_timer_ctl(i, T_ENABLE);
    }
}

static void disable_timers(void)
{
    int i;
    const struct timer_info *t;
    int cur_el = aarch64_current_el();

    assert(cur_test);

    for (i = 0; i < 4; i++) {
        t = cur_test->timers + i;

        if (!t->enabled) {
            continue;
        }

        if (!config_timer_in_el(t, cur_el, cur_test->virt_mode)) {
            continue;
        }

        a64_write_timer_ctl(i, 0);
    }
}

static void el1_entry(void *arg)
{
    int i;

    DPRINTF("enter\n");
    assert(aarch64_current_el() == 1);

    enable_timers();
    local_cpu_ei();

    for (i = 0; i < 8; i++) {
        cpu_wfi();
        DPRINTF("loop\n\n");
#ifndef DEBUG
        printf(".");
#endif
    }

#ifndef DEBUG
    printf("\n");
#endif

    local_cpu_di();
    disable_timers();

    DPRINTF("leave\n");
}

void gic_test(const char * test_name, const struct test_info *info)
{
    assert(aarch64_current_el() == 3);

    local_cpu_fiq_di();
    local_cpu_di();

    printf("\n\nGIC test: %s\n", test_name);
    cur_test = info;

    aarch64_set_irq_h(timer_irq_h);
    aarch64_set_fiq_h(timer_fiq_h);

    configure_gic(&info->gic);
    configure_timers(info->timers);

    enable_timers();
    local_cpu_fiq_ei();

    if (info->virt_mode) {
        switch_to_virt_mode_el1(el1_entry, NULL);
    } else {
        switch_to_el1(el1_entry, NULL);
    }

    assert(aarch64_current_el() == 3);

    local_cpu_fiq_di();
    disable_timers();
    teardown_git(&info->gic);

    printf("GIC test %s passed\n", test_name);
}
