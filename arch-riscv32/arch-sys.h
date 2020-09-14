#ifndef __ARCH_SYS_H__
#define __ARCH_SYS_H__
#include <stdint.h>

#define mb() asm volatile ("fence\n" : : : "memory");

#define local_cpu_ei()
#define local_cpu_di()

struct excp_frame {
	uint32_t r[32];
};

static inline void cpu_wfi(void)
{
	asm volatile ("wfi\n");
}

static inline void cpu_relax(void)
{
	/* In qemu memory barriers make the CPU break from
	   its emulation loop and gives IO a better chance to run.
	   This is not needed nor helpful on real hw.  */
	mb();
}
#endif
