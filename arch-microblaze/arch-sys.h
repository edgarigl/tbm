#ifndef __ARCH_SYS_H__
#define __ARCH_SYS_H__
#include <stdint.h>

#define mb() asm volatile ("mbar 2\n" : : : "memory");

#define local_cpu_ei() asm volatile ("msrset r0, 0x2");
#define local_cpu_di() asm volatile ("msrclr r0, 0x2");

struct excp_frame {
	uint32_t r[32];
};

static inline void cpu_wfi(void)
{
	asm volatile ("mbar 16\n");
}

static inline void cpu_relax(void)
{
	/* In qemu memory barriers make the CPU break from
	   its emulation loop and gives IO a better chance to run.
	   This is not needed nor helpful on real hw.  */
	mb();
}
#endif
