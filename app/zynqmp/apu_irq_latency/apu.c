/*
 * Dummy CSU code for testing the TLM demo.
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

static uint64_t counter = 0;
static uint32_t freq;
static uint64_t freq_k;
static unsigned int el;

struct {
	uint64_t max_latency;
	uint64_t min_latency;
	uint64_t max_warm_latency;
	uint64_t rounds;
} st = {
	.max_latency = 0,
	.max_warm_latency = 0,
	.min_latency = -1LL,
	.rounds = 0,
};

static inline void timer_rearm(uint64_t delay)
{
        counter += delay;
        a64_write_timer_cval(TIMER_VIRT, counter);
	ibarrier();
//        a64_write_timer_ctl(TIMER_VIRT, T_ENABLE);
	ibarrier();
}

static void timer_irq_h(struct excp_frame *f)
{
	uint32_t r;
	uint64_t now;
	uint64_t diff;
	uint64_t tmp_counter;

	now = aarch64_irq_get_stamp(el);
	mb();
	ibarrier();

	r = readl(GIC_CPU_BASE + GICC_IAR);
	writel(GIC_CPU_BASE + GICC_EOIR, r);

	assert(r == VIRT_TIMER_IRQ);

	tmp_counter = counter;
	diff = now - counter;
	counter = now;
	timer_rearm(freq);

	printf("%s: %lld ns\n", __func__, diff * freq_k);
}

void app_run(void)
{
	uint32_t scr;

	printf("%s\n", __func__);

	el = aarch64_current_el();
	aarch64_set_irq_h(timer_irq_h);

	local_cpu_fiq_di();
	local_cpu_di();
	gicd_set_irq_group(GIC_DIST_BASE, VIRT_TIMER_IRQ, 0);
	gicd_set_irq_target(GIC_DIST_BASE, VIRT_TIMER_IRQ, 0);
	gicd_enable_irq(GIC_DIST_BASE, VIRT_TIMER_IRQ);

	writel(GIC_DIST_BASE + GICD_CTRL, 3);
	writel(GIC_CPU_BASE + GICC_CTRL, 3);
	writel(GIC_CPU_BASE + GICC_PMR, 0xff);

	printf("GICD_CTL=%x\n", readl(GIC_DIST_BASE + GICD_CTRL));
	printf("GICD_ISENABLE=%x\n", readl(GIC_DIST_BASE + GICD_ISENABLER));
	printf("GICD_TARGET0=%x\n", readl(GIC_DIST_BASE + GICD_ITARGETSR));
	printf("GICD_TARGET1=%x\n", readl(GIC_DIST_BASE + GICD_ITARGETSR + 4));
	printf("GICD_TARGET3=%x\n", readl(GIC_DIST_BASE + GICD_ITARGETSR + 8));
	printf("GICC_CTL=%x\n", readl(GIC_CPU_BASE + GICC_CTRL));

	if (el == 3) {
		aarch64_mrs(scr, "scr_el3");
		scr |= SCR_IRQ;
		aarch64_msr("scr_el3", scr);
	}
	ibarrier();

	aarch64_msr("cntvoff_el2", 0);
	aarch64_mrs(freq, "CNTFRQ_EL0");
	printf("CNTFRQ=%d Hz\n", freq);

	/* We'd like to show our delays in ns.  */
	freq_k = (1000 * 1000 * 1000ULL) / freq;
	printf("freq_k=%llx\n", freq_k);

	/* Enable the timer.  */
	counter = a64_read_timer_cnt(TIMER_VIRT);
	timer_rearm(freq);
        a64_write_timer_ctl(TIMER_VIRT, T_ENABLE);

	local_cpu_ei();

        do {
		cpu_wfi();
		udelay(8);
        } while (1);


	printf("Done\n");
	while (1)
		;
}
