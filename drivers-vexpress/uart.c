#include "sys.h"

#define UART0 0x1c090000
#define R_UART_TX  (UART0 + 0x0)
int uart_putchar(int c)
{
	* (volatile unsigned int *) R_UART_TX = c;
	return c;
}

void uart_init(void)
{
}
