/* Generic Xilinx UART-Lite driver.
 * Written by Edgar E. Iglesias.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "sys.h"

#define R_RX_FIFO		0x00
#define R_TX_FIFO		0x04
#define R_STATUS		0x08
#define RX_FIFO_VALID_MASK	(1 << 0)
#define RX_FIFO_FULL_MASK	(1 << 1)
#define TX_FIFO_EMPTY_MASK	(1 << 2)
#define TX_FIFO_FULL_MASK	(1 << 3)

#define R_CTRL			0x08

static inline bool xlx_uart_tx_full(char *base)
{
	uint32_t r;

	r = readl(base + R_STATUS);
	return r & TX_FIFO_FULL_MASK;
}

int xlx_uart_putchar(char *base, int c)
{
	/* Wait for space.  */
	while (xlx_uart_tx_full(base))
		;

	writel(base + R_TX_FIFO, c);
	return c;
}

#ifdef UARTLITE_BASE
int uart_putchar(int c) {
    return xlx_uart_putchar((char *) UARTLITE_BASE, c);
}

void uart_init(void) { }
#endif
