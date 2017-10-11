#define _MINIC_SOURCE

#include "handler.h"
#include "irq.h"
#include "test_entry.h"
#include "trace.h"
#include "switch.h"
#include "gic.h"

struct irq_ctx {
    const struct timer_info *timer_info;
    struct nested_irq_ctx nested_irq_ctx;

    int el;
    enum e_irq_type type;
    uint32_t irq;
    int timer_id;
};

static uint64_t counter[4] = { 0 };
static uint32_t freq[4];
static uint64_t freq_k;

static bool token[4] = { false };
static int irq_lvl = -1;
static int max_lvl = -1;

static uint8_t el1_stack[0x8000];

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
    assert(cur_test->timers[id].enabled);

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

void timer_rearm(struct irq_ctx *ctx, void *opaque)
{
    uint64_t now;

    aarch64_mrs(now, "cntvct_el0");
    mb();
    ibarrier();

    counter[ctx->timer_id] = now;
    _timer_rearm(ctx->timer_id);
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
    enable_nested_irq(ctx->type, ctx->el, &ctx->nested_irq_ctx);
}

void nested_irq_disable(struct irq_ctx *ctx, void *opaque)
{
    disable_nested_irq(ctx->type, ctx->el, &ctx->nested_irq_ctx);
}

void gic_eoi(struct irq_ctx *ctx, void *opaque)
{
    int rpr_before = gic_running_prio(), rpr_after;
    DPRINTF("About to end irq %u. Running prio before: %u\n", ctx->irq, rpr_before);

    gic_end_of_irq(ctx->irq);

    rpr_after = gic_running_prio();
    DPRINTF("After: %u\n", rpr_after);

    assert(rpr_before <= rpr_after);
}

void gic_deactivate_irq(struct irq_ctx *ctx, void *opaque)
{
    writel(GIC_CPU_BASE + GICC_DIR, ctx->irq);
}

void token_reset_self(struct irq_ctx *ctx, void *opaque)
{
    token[timer_get_lvl(ctx->timer_id)] = false;
}

void token_wait_self(struct irq_ctx *ctx, void *opaque)
{
    while(!token_is_set(ctx->timer_id)) {
        cpu_wfi();
    }
}

void token_assert(struct irq_ctx *ctx, void *opaque)
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

static inline void generic_handler(enum e_irq_type type, int expected_el)
{
    struct irq_ctx ctx;
    const struct handler_action *actions;

    irq_lvl++;

    assert(aarch64_current_el() == expected_el);

    ctx.el = expected_el;
    ctx.type = type;
    ctx.irq = gic_ack_irq();
    ctx.timer_id = get_timer_id(ctx.irq);
    ctx.timer_info = &cur_test->timers[ctx.timer_id];

    DPRINTF("enter [%s] lvl:%d\n", TIMER_LABEL[ctx.timer_id], irq_lvl);

    if (ctx.timer_info->actions) {
        actions = ctx.timer_info->actions;
    } else {
        actions = cur_test->actions;
    }

    while (actions->fn) {
        actions->fn(&ctx, actions->opaque);
        actions++;
    }

    DPRINTF("leave [%s] lvl:%d\n", TIMER_LABEL[ctx.timer_id], irq_lvl);
    irq_lvl--;
}

void timer_fiq_h(struct excp_frame *f)
{
    generic_handler(FIQ, 3);
}

void timer_irq_h(struct excp_frame *f)
{
    generic_handler(IRQ, 1);
}

static void configure_gic(const struct gic_info *info)
{
    gic_configure(info);
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

    /* We'd like to show our delays in ns.  */
    freq_k = (1000 * 1000 * 1000ULL) / cntfrq;
    DPRINTF("freq_k=%llx\n", freq_k);

    for (i = 0; i < 4; i++) {
        freq[i] = cntfrq / info[i].tick_divisor;
        max_lvl = info[i].lvl > max_lvl ? info[i].lvl : max_lvl;
    }
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

        if (t->el != cur_el) {
            continue;
        }

        gicd_enable_irq(GIC_DIST_BASE, TIMER_IDX_TO_IRQ[i]);
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

        if (t->el != cur_el) {
            continue;
        }

        a64_write_timer_ctl(i, 0);
        gicd_disable_irq(GIC_DIST_BASE, TIMER_IDX_TO_IRQ[i]);
    }
}

static void el1_entry(void)
{
    int i;

    DPRINTF("enter\n");
    assert(aarch64_current_el() == 1);

    enable_timers();
    local_cpu_ei();

    for (i = 0; i < 8; i++) {
        cpu_wfi();
        DPRINTF("loop\n");
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

static inline void set_el1_stack(void)
{
    uint8_t * stack_bottom = el1_stack + sizeof(el1_stack);

    asm volatile ("msr sp_el1, %0"
                  : : "r" (stack_bottom));
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
    set_el1_stack();

    enable_timers();
    local_cpu_fiq_ei();

    switch_to_el1(el1_entry);
    assert(aarch64_current_el() == 3);

    local_cpu_fiq_di();
    disable_timers();

    printf("GIC test %s passed\n", test_name);
}
