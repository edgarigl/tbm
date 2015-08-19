#define R_UART_TX (0x83e00000 + 0x1000 + 0x0)
int uart_putchar(int c)
{
	* (volatile unsigned int *) R_UART_TX = c;
	return c;
}

void uart_init(void)
{
}

