/*
 * Interrupt handling routines for MicroBlaze
 *
 * Copyright (C) 2013 Xilinx Inc.
 * Written by Edgar E. Iglesias
 */

#include <stdio.h>
#include <assert.h>

#include "arch-irq.h"
#include "sys.h"

static void (*mb_isr)(struct arch_cpustate *regs);

__attribute__ ((weak))
void arch_interrupt_handler(struct arch_cpustate *regs)
{
	if (mb_isr)
		mb_isr(regs);
	else {
		printf("spurious irq 0\n");
		err();
	}
}

void arch_isr_register(int irq, void (*isr)(struct arch_cpustate *regs))
{
	assert(irq == 0);
	assert(mb_isr == NULL);
	mb_isr = isr;
}

void arch_isr_unregister(int irq, void (*isr)(struct arch_cpustate *regs))
{
	assert(irq == 0);
	assert(mb_isr == isr);
	mb_isr = NULL;
}
