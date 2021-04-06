#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "sys.h"
#include "reg-crf.h"
#include "plat.h"

#define APU_BASE	APU
#define R_RVBAR_L_0	(APU_BASE + 0x40)
#define R_RVBAR_H_0	(APU_BASE + 0x44)

#define D(x)

bool plat_psci_cpu_on(unsigned int cpu, uintptr_t entry)
{
	uint32_t r;

	/* Bring the CPU back into RESET state.  */
	r = readl(CRF_APB_RST_FPD_APU);
	r |= (0x401 << cpu);
	writel(CRF_APB_RST_FPD_APU, r);
	mb();
	udelay(10);

	writel(R_RVBAR_L_0 + cpu * 8, entry);
	writel(R_RVBAR_H_0 + cpu * 8, entry >> 32);
	mb();

	D(printf("TBM: PSCI: release CPU%d to %p\n", cpu, (void *)entry));

	/* Release the CPU.  */
	r &= ~(0x401 << cpu);
	D(printf("RST_FPD_APU %p=%x\n", CRF_APB_RST_FPD_APU, r));
	writel(CRF_APB_RST_FPD_APU, r);
	mb();
	D(printf("CPU%d released %x\n", cpu, r));
	udelay(10);
	D(printf("done\n"));
	return true;
}

bool plat_psci_cpu_off(unsigned int cpu)
{
	uint32_t r;
	D(printf("TBM: PSCI: CPU%d off\n", cpu));
	r = readl(CRF_APB_RST_FPD_APU);
	D(printf("read RST_FPD_APU=%x\n", r));
	r |= 0x401 << cpu;
	D(printf("pre-write RST_FPD_APU=%x\n", r));
	writel(CRF_APB_RST_FPD_APU, r);
	mb();
	r = readl(CRF_APB_RST_FPD_APU);
	mb();
	D(printf("post-write RST_FPD_APU=%x\n", r));
	return true;
}
