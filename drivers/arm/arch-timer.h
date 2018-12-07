#include <stdint.h>

enum {
	TIMER_PHYS,
	TIMER_PHYS_SEC,
	TIMER_VIRT,
	TIMER_HYP,
};

#define T_ENABLE	1
#define T_MASK		2
#define T_ISTATUS	4

uint64_t a64_read_timer_cnt(int timer);
uint32_t a64_read_timer_ctl(int timer);
void a64_write_timer_ctl(int timer, uint32_t ctl);
void a64_write_timer_tval(int timer, uint32_t tval);
void a64_write_timer_cval(int timer, uint64_t val);
