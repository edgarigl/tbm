/*
 * Driver for the Xilinx MicroBlaze I/O Module.
 *
 * Copyright (C) 2013 Xilinx Inc.
 * Written by Edgar E. Iglesias
 */
#ifdef __MICROBLAZE__
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "reg-iom.h"
#include "iom.h"
#include "sys.h"

void iom_pit_start(void *base, unsigned int pit, unsigned int count, bool oneshot)
{
	unsigned long pitbase = R_IOM_PIT1_BASE + (pit * 0x10);
	uint32_t ctrl = IOM_PIT_CONTROL_EN;

	if (!oneshot) {
		ctrl |= IOM_PIT_CONTROL_PRELOAD;
	}

	writel(base + pitbase + R_IOM_PIT_PRELOAD, count);
	writel(base + pitbase + R_IOM_PIT_CONTROL, ctrl);
}

void iom_pit_stop(void *base, unsigned int pit)
{
	unsigned long pitbase = R_IOM_PIT1_BASE + (pit * 0x10);
	writel(base + pitbase + R_IOM_PIT_CONTROL, 0);
}

int iom_putchar(void *base, int c)
{
	uint32_t r;
	do {
		r = readl(base + R_IOM_UART_STATUS);
	} while (r & IOM_UART_STATUS_TX_USED);

	writel(base + R_IOM_UART_TX, c);
	return (unsigned char)c;
}

int __iom_getchar(void *base)
{
	return readl(base + R_IOM_UART_RX);
}

int iom_getchar(void *base)
{
	uint32_t r;

	/* Wait for character.  */
	do {
		r = readl(base + R_IOM_UART_STATUS);
	} while (r & IOM_UART_STATUS_RX_VALID);
	return __iom_getchar(base);
}
#endif
