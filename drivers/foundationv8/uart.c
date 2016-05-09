#include "sys.h"

#define UART0          0x1c090000
#define R_UART_TX      (UART0 + 0x0)
#define R_UART_LCR     (UART0 + 0x2c)
#define LCR_DATA_LEN_8 (3 << 5)
#define LCR_FIFO_EN    (1 << 4)
#define R_UART_CR      (UART0 + 0x30)
#define CR_UART_EN     1
#define CR_UART_TXE    (1 << 8)
#define CR_UART_RXE    (1 << 9)

int uart_putchar(int c)
{
	* (volatile unsigned int *) R_UART_TX = c;
	return c;
}

void uart_init(void)
{
	writel(R_UART_LCR, LCR_DATA_LEN_8 | LCR_FIFO_EN);
	writel(R_UART_CR, CR_UART_RXE | CR_UART_TXE | CR_UART_EN);
}
