#ifndef __ARCH_SYS_H__
#define __ARCH_SYS_H__

#include <stdint.h>
#include <stdio.h>

#include "aarch64-regs.h"
#include "aarch64-excp.h"
#include "aarch64-el.h"
#include "aarch64-smc.h"

#define mb() __asm__ __volatile__ ("dmb\tsy\n" : : : "memory");
#define ibarrier() __asm__ __volatile__ ("isb\n" : : : "memory");

#define local_cpu_di() __asm__ __volatile__ ("msr    daifset, #2")
#define local_cpu_ei() __asm__ __volatile__ ("msr    daifclr, #2")
#define local_cpu_fiq_di() __asm__ __volatile__ ("msr    daifset, #1")
#define local_cpu_fiq_ei() __asm__ __volatile__ ("msr    daifclr, #1")
#define local_cpu_serror_di() __asm__ __volatile__ ("msr    daifset, #4")
#define local_cpu_serror_ei() __asm__ __volatile__ ("msr    daifclr, #4")



static inline void cpu_wfi(void)
{
	__asm__ __volatile__ ("wfi\n");
}

static inline void cpu_relax(void)
{
	__asm__ __volatile__ ("yield\n");
}

void sys_dump_dfsc(unsigned int dfsc);
static inline void sys_dump_stack(void)
{
        void *sp;
        __asm__ __volatile__ ("mov %0, sp\n" : "=r" (sp));
        printf("sp=%p\n", sp);
        hexdump("stack", sp, 256);
}

/* Flush as much as possible to memory. Expensive */
void plat_cache_flush(void);
#endif
