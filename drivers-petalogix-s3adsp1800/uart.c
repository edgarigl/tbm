#define R_UART_TX 0x84000004
int uart_putchar(int c)
{
	* (volatile unsigned int *) R_UART_TX = c;
	return c;
}

void uart_init(void)
{
}

