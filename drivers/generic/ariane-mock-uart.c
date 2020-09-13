/* Generic Ariane mock UART driver.
 * Written by Edgar E. Iglesias.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "sys.h"

#define R_THR		0x00

#ifdef ARIANE_MOCK_UART_BASE
int uart_putchar(int c) {
	writel(ARIANE_MOCK_UART_BASE + R_THR, c);
	return c;
}

void uart_init(void) { }
#endif
