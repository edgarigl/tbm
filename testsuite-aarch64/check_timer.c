#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

enum {
	TIMER_PHYS,
	TIMER_PHYS_SEC,
	TIMER_VIRT,
	TIMER_HYP,
};

#define T_ENABLE	1
#define T_MASK		2
#define T_ISTATUS	4

static uint64_t a64_read_timer_cnt(int timer)
{
	uint64_t cnt = 0;

	switch (timer) {
	case TIMER_HYP:
	case TIMER_PHYS:
	case TIMER_PHYS_SEC:
		aarch64_mrs(cnt, "cntpct_el0");
		break;
	case TIMER_VIRT:
		aarch64_mrs(cnt, "cntvct_el0");
		break;
	default:
		assert(0);
		break;
	}
	return cnt;
}

static uint32_t a64_read_timer_ctl(int timer)
{
	uint32_t ctl = 0;
	switch (timer) {
	case TIMER_PHYS:
		aarch64_mrs(ctl, "cntp_ctl_el0");
		break;
	case TIMER_PHYS_SEC:
		aarch64_mrs(ctl, "cntps_ctl_el1");
		break;
	case TIMER_VIRT:
		aarch64_mrs(ctl, "cntv_ctl_el0");
		break;
	case TIMER_HYP:
		aarch64_mrs(ctl, "cnthp_ctl_el2");
		break;
	default:
		assert(0);
		break;
	}
	return ctl;
}

static void a64_write_timer_ctl(int timer, uint32_t ctl)
{
	switch (timer) {
	case TIMER_PHYS:
		aarch64_msr("cntp_ctl_el0", ctl);
		break;
	case TIMER_PHYS_SEC:
		aarch64_msr("cntps_ctl_el1", ctl);
		break;
	case TIMER_VIRT:
		aarch64_msr("cntv_ctl_el0", ctl);
		break;
	case TIMER_HYP:
		aarch64_msr("cnthp_ctl_el2", ctl);
		break;
	default:
		assert(0);
		break;
	}
}

static void a64_write_timer_tval(int timer, uint32_t tval)
{
	switch (timer) {
	case TIMER_PHYS:
		aarch64_msr("cntp_tval_el0", tval);
		break;
	case TIMER_PHYS_SEC:
		aarch64_msr("cntps_tval_el1", tval);
		break;
	case TIMER_VIRT:
		aarch64_msr("cntv_tval_el0", tval);
		break;
	case TIMER_HYP:
		aarch64_msr("cnthp_tval_el2", tval);
		break;
	default:
		assert(0);
		break;
	}
}

static void a64_write_timer_cval(int timer, uint32_t val)
{
	switch (timer) {
	case TIMER_PHYS:
		aarch64_msr("cntp_cval_el0", val);
		break;
	case TIMER_PHYS_SEC:
		aarch64_msr("cntps_cval_el1", val);
		break;
	case TIMER_VIRT:
		aarch64_msr("cntv_cval_el0", val);
		break;
	case TIMER_HYP:
		aarch64_msr("cnthp_cval_el2", val);
		break;
	default:
		assert(0);
		break;
	}
}

static void check_timer_delay(int timer, unsigned int delay, uint64_t offset)
{
	uint32_t ctl;
	uint64_t cval;

	if (timer == TIMER_VIRT)
		aarch64_msr("cntvoff_el2", offset);

	a64_write_timer_tval(timer, delay);
	ctl = a64_read_timer_ctl(timer);
	ctl |= T_MASK;
	ctl &= ~T_ISTATUS;
	ctl |= T_ENABLE;
	a64_write_timer_ctl(timer, ctl);
	do {
		ctl = a64_read_timer_ctl(timer);
	} while (!(ctl & T_ISTATUS));

	ctl = a64_read_timer_ctl(timer);
	ctl |= T_MASK;
	ctl &= ~T_ISTATUS;
	ctl |= T_ENABLE;
	a64_write_timer_ctl(timer, ctl);
	cval = a64_read_timer_cnt(timer) + delay;
	a64_write_timer_cval(timer, cval);
	do {
		ctl = a64_read_timer_ctl(timer);
	} while (!(ctl & T_ISTATUS));
}

uint64_t rand64(void)
{
	uint64_t v;
	v = rand();
	v <<= 32;
	v |= rand();
	return v;
}

static void check_timer(int timer)
{
	unsigned int i;
	unsigned int delay;
	uint64_t offset;

	for (i = 0; i < 512; i++) {
		delay = rand() & 0xf;
		offset = rand64();
		check_timer_delay(timer, delay, offset);
	}
	for (i = 0; i < 128; i++) {
		delay = rand() & 0xfff;
		offset = rand64();
		check_timer_delay(timer, delay, offset);
	}
	for (i = 0; i < 4; i++) {
		delay = rand() & 0xffff;
		offset = rand64();
		check_timer_delay(timer, delay, offset);
	}
#if 0
	for (i = 0; i < 8; i++) {
		delay = rand() & 0x7fffffff;
		offset = rand64();
		check_timer_delay(timer, delay, offset);
	}
#endif
}

static bool check_cnt(int timer)
{
	uint64_t cnt, prev, offset;
	unsigned int i;

	offset = rand64();
	if (timer == TIMER_VIRT)
		aarch64_msr("cntvoff_el2", offset);

	prev = a64_read_timer_cnt(timer);
	mb();
	for (i = 0; i < 10; i++) {
		cnt = a64_read_timer_cnt(timer);
		if (cnt > prev)
			break;
		if (i > 9 || cnt < prev)
			printf("%s: %u cnt=%llu prev=%llu\n", __func__, i, cnt, prev);
		prev = cnt;
		mb();
	}
	if (cnt <= prev) {
		printf("%s: timer=%d failed\n", __func__, timer);
		return false;
	}
	return true;
}

void a64_check_timer(void)
{
	struct {
		int timer;
		const char *name;
		bool works;
	} timers[] = {
		{ TIMER_PHYS, "phys" },
		{ TIMER_VIRT, "virt" },
		{ TIMER_HYP, "hyp" },
#ifndef AARCH64_NO_SEC_TIMER
		{ TIMER_PHYS_SEC, "phys_sec" },
#endif
	};
	uint32_t scr;
	uint32_t t;
	unsigned int i;
	unsigned int el = aarch64_current_el();

	assert(el == 3);

	aarch64_mrs(t, "cnthctl_el2");
	t |= 1;
	aarch64_msr("cnthctl_el2", t);

	aarch64_mrs(scr, "scr_el3");
	scr |= SCR_ST | SCR_EA | SCR_RW;
	scr &= ~SCR_NS;

	for (i = 0; i < ARRAY_SIZE(timers); i++) {

		timers[i].works = check_cnt(timers[i].timer);
		printf("%s: %s works=%d\n", __func__, timers[i].name, timers[i].works);
		if (timers[i].works)
			check_timer(timers[i].timer);

	}

	for (i = 0; i < 256; i++) {
		unsigned int timer, test, el1;

		timer = rand() % ARRAY_SIZE(timers);
		test = rand() & 1;
		el1 = rand() & 1;
		if (!timers[timer].works)
			continue;
		printf("%s.", timers[timer].name);

		el = aarch64_current_el();

		if (timers[timer].timer == TIMER_PHYS_SEC && el1) {
			aarch64_msr("scr_el3", scr);
			aarch64_drop_el(EL1, SPXH);
		}

		if (test)
			check_cnt(timers[timer].timer);
		else
			check_timer(timers[timer].timer);

		if (timers[timer].timer == TIMER_PHYS_SEC && el1) {
			aarch64_raise_el(EL3);
		}
	}
	printf("\n");
}
