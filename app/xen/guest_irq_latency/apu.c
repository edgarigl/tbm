/*
 * TBM application to meassure interrupt-latency.
 * This instance runs as a guest in Xen.
 *
 * Copyright (c) 2016 Xilinx Inc
 * Written by Edgar E. Iglesias.
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "sys.h"

#include "drivers/arm/gic.h"
#include "drivers/arm/arch-timer.h"

#define xstr(s) str(s)
#define str(s) #s

#define VIRT_TIMER_IRQ (16 + 11)

static const char arch_str[] = xstr(__BOARD__) "-" xstr(__UNIT__);

static uint64_t counter;
static uint32_t freq;
static uint64_t freq_k;
static unsigned int el;

struct {
	int64_t avg;			/* Moving average k=0.125  */
	uint64_t max_latency;
	uint64_t min_latency;
	uint64_t max_warm_latency;	/* Max seen after initial warmup runs.  */
	uint64_t rounds;
} st = {
	.avg = 0,
	.max_latency = 0,
	.max_warm_latency = 0,
	.min_latency = -1LL,
	.rounds = 0,
};


void show_stats(void)
{
	printf("latency (ns): max=%lld warm_max=%lld min=%lld avg=%lld\n",
		st.max_latency, st.max_warm_latency, st.min_latency, st.avg);
}

void update_stats(uint64_t latency)
{
	bool update = false;
	int64_t diff;
	int64_t prev_avg = st.avg;

	st.rounds++;

//	printf("latency=%llu ns\n", latency);
	if (latency > st.max_latency) {
		st.max_latency = latency;
		update = true;
	}

	/* Give it 2 rounds to warm up the caches.  */
	if (st.rounds > 2 && latency > st.max_warm_latency) {
		st.max_warm_latency = latency;
		update = true;
	}

	if (latency < st.min_latency) {
		st.min_latency = latency;
		update = true;
	}

	/* Moving average.  */
	if (st.avg == 0) {
		st.avg = latency;
		update = true;
	} else {
		/* Moving average k=0.125 */
		diff = (int64_t)latency - (int64_t)st.avg;
		st.avg += diff / 8;
		if (st.avg != prev_avg)
			update = true;
	}

	if (update) {
		show_stats();
	}
}

static inline void timer_rearm(uint64_t delay)
{
        counter += delay;
        a64_write_timer_cval(TIMER_VIRT, counter);
	ibarrier();
        a64_write_timer_ctl(TIMER_VIRT, T_ENABLE);
	ibarrier();
}

static void timer_irq_h(struct excp_frame *f)
{
	uint32_t r;
	uint64_t now;
	uint64_t diff;

	now = aarch64_irq_get_stamp(el);
	mb();
	ibarrier();

	r = readl(GIC_CPU_BASE + GICC_IAR);
        a64_write_timer_ctl(TIMER_VIRT, T_MASK);

	if (r != VIRT_TIMER_IRQ) {
		printf("r=%d\n", r);
	}
	assert(r == VIRT_TIMER_IRQ);
	/* Disable the timer while we reprogram it.  */

	diff = now - counter;
	counter = now;
	writel(GIC_CPU_BASE + GICC_EOIR, r);
	timer_rearm(freq / 2);

	update_stats(diff * freq_k);
}

void app_run(void)
{
	el = aarch64_current_el();
	aarch64_set_irq_h(timer_irq_h);

	/* Disable interrupts while we configure the GIC.  */
	local_cpu_di();

	/* Setup the GIC.  */
	gicd_set_irq_group(GIC_DIST_BASE, VIRT_TIMER_IRQ, 0);
	gicd_set_irq_target(GIC_DIST_BASE, VIRT_TIMER_IRQ, 0);
	gicd_enable_irq(GIC_DIST_BASE, VIRT_TIMER_IRQ);

	writel(GIC_DIST_BASE + GICD_CTRL, 3);
	writel(GIC_CPU_BASE + GICC_CTRL, 3);
	writel(GIC_CPU_BASE + GICC_PMR, 0xff);
	mb();

	aarch64_mrs(freq, "CNTFRQ_EL0");
	printf("CNTFRQ=%d Hz\n", freq);

	/* We'd like to show our delays in ns.  */
	freq_k = (1000 * 1000 * 1000ULL) / freq;

	/* Enable the timer.  */
	counter = a64_read_timer_cnt(TIMER_VIRT);
        a64_write_timer_tval(TIMER_VIRT, 0);
	timer_rearm(freq);
        a64_write_timer_ctl(TIMER_VIRT, T_ENABLE);
	ibarrier();

	local_cpu_ei();
        do {
		if (0)
			cpu_wfi();
		cpu_relax();
        } while (1);

	printf("Done\n");
	while (1)
		;
}
