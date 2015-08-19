#ifndef __ARCH_IRQ_H__
#define __ARCH_IRQ_H__

#define CPUSTATE_SIZE		(34 * 4)
#define CPUSTATE_MSR_OFFSET	(32 * 4)
#define CPUSTATE_PC_OFFSET	(33 * 4)

#ifndef __ASSEMBLY__
#include <stdint.h>
struct arch_cpustate {
	uint32_t regs[32];
	uint32_t msr;
	uint32_t pc;
};

void arch_isr_register(int irq, void (*isr)(struct arch_cpustate *regs));
void arch_isr_unregister(int irq, void (*isr)(struct arch_cpustate *regs));
#endif
#endif
