#ifndef __ARCH_SYS_H__
#define __ARCH_SYS_H__
#include <stdint.h>
#include <regs.h>

#define mb() __asm__ __volatile__ ("dmb\n" : : : "memory");

struct excp_frame {
        uint32_t r[16];
};

static inline void cpu_wfi(void)
{
	__asm__ __volatile__ ("wfi\n");
}

static inline void cpu_relax(void)
{
}
#endif
