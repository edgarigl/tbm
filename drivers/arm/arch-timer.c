#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"

#include "drivers/arm/arch-timer.h"

uint64_t a64_read_timer_cnt(int timer)
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

uint32_t a64_read_timer_ctl(int timer)
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

void a64_write_timer_ctl(int timer, uint32_t ctl)
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

void a64_write_timer_tval(int timer, uint32_t tval)
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

void a64_write_timer_cval(int timer, uint32_t val)
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
