#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define SMC_ATOMIC	0x80000000
#define SMC_A64		0x40000000

#define SMC_OWNER_OEM	0x02000000

#define SMC_TBM_BASE		(SMC_OWNER_OEM)
#define SMC_TBM_NOT		(SMC_TBM_BASE | 0)
#define SMC_TBM_RAISE_EL	(SMC_TBM_BASE | 1)
#define SMC_TBM_MAX		(SMC_TBM_RAISE_EL)

#define SMC_PSCI_BASE		0
#define SMC_PSCI_CPU_OFF	(0x84000002)
#define SMC_PSCI_CPU_ON		(0xc4000003)
#define SMC_PSCI_MAX		SMC_PSCI_CPU_ON

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
