#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "aarch64-smc-asm.h"

bool plat_psci_cpu_on(unsigned int cpu, uintptr_t entry);
bool plat_psci_cpu_off(unsigned int cpu);

uint64_t aarch64_smc_decode(struct excp_frame *f);

static inline uintptr_t aarch64_smc(unsigned int callnr,
				    uintptr_t arg1,
				    uintptr_t arg2,
				    uintptr_t arg3,
				    uintptr_t arg4,
				    uintptr_t arg5,
				    uintptr_t sess_id,
				    uint32_t hyp_id)
{
	register uintptr_t a0 asm("x0") = callnr;
	register uintptr_t a1 asm("x1") = arg1;
	register uintptr_t a2 asm("x2") = arg2;
	register uintptr_t a3 asm("x3") = arg3;
	register uintptr_t a4 asm("x4") = arg4;
	register uintptr_t a5 asm("x5") = arg5;
	register uintptr_t a6 asm("x6") = sess_id;
	register uintptr_t a7 asm("x7") = hyp_id;

	asm volatile ("smc\t0\n"
			: "=r" (a0), "=r"(a1), "=r" (a2), "=r" (a3)
			:	"0" (a0),
				"r" (a1),
				"r" (a2),
				"r" (a3),
				"r" (a4),
				"r" (a5),
				"r" (a6),
				"r" (a7));
	return a0;
}
