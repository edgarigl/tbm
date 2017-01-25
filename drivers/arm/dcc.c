#include "sys.h"

#define SR_TXFULL 0x20000000
#define SR_RXFULL 0x40000000

static inline uint32_t dcc_status(void)
{
	uint32_t r;
	__asm__ __volatile__("mrs %0, mdccsr_el0\n" : "=r" (r));
	return r;
}

static int dcc_tx_ready(void)
{
	bool txfull = dcc_status() & SR_TXFULL;
	return !txfull;
}

int uart_putchar(int c)
{
	while (!dcc_tx_ready())
		barrier();
	__asm__ __volatile__("msr dbgdtrtx_el0, %0\n" : : "r" (c));
	return c;
}

void uart_init(void)
{
}
