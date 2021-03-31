#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "sys.h"

#define D(x)

static uint64_t g_context_id;
static intptr_t g_entry;

void _secondary_entry_head(void);

__attribute__((weak))
void plat_init_secondaries(void)
{
}

__attribute__((weak))
void app_psci_cpu_pre_handover(void)
{
	/* Last wish.  */
}

__attribute__((weak))
bool plat_psci_cpu_on(unsigned int cpu, uintptr_t entry)
{
	return false;
}

__attribute__((weak))
bool plat_psci_cpu_off(unsigned int cpu)
{
	return false;
}

__attribute__((noreturn))
void secondary_entry(void)
{
	const unsigned int target_el = 2;

	plat_init_secondaries();
	app_psci_cpu_pre_handover();

        aarch64_msr("SCR_EL3", SCR_HCE | SCR_RW | SCR_NS | 0x30);
	__asm__ __volatile__ (
			"\tmsr\telr_el3, %0\n"
			"\tmsr\tspsr_el3, %1\n"
			"\tmov\tx0, %2\n"
			"\tmov\tx1, xzr\n"
			"\tmov\tx2, xzr\n"
			"\tmov\tx3, xzr\n"
			"eret\n"
				: : "r" (g_entry),
				    "r" ((7 << 6) | (target_el << 2) | 1),
				    "r" (g_context_id));
	while (1)
		;
}

uintptr_t aarch64_psci_on(uint64_t id, uint64_t cpu,
			uint64_t entry, uint64_t context_id)
{
	bool ok;
	D(printf("%s: id=%llx cpu=%lld entry=%llx context=%llx\n",
			__func__, id, cpu, entry, context_id));
	g_entry = entry;
	g_context_id = context_id;
	mb();
	plat_cache_flush();
	ok = plat_psci_cpu_on(cpu, (uintptr_t) _secondary_entry_head);
	mb();
	ibarrier();
	return ok ? PSCI_SUCCESS : -1;
}

uintptr_t aarch64_psci_off(uint64_t id, uint64_t cpu)
{
	D(printf("%s: id=%llx cpu=%lld\n", __func__, id, cpu));
	plat_psci_cpu_off(cpu);
	return 0;
}

uintptr_t aarch64_psci_self_suspend(uint64_t id)
{
	int cpu;

	D(printf("%s: id=%llx cpu=%lld\n", __func__, id));

	aarch64_mrs(cpu, "mpidr_el1");
	cpu &= 0xff;
	plat_psci_cpu_off(cpu);
	return 0;
}

uintptr_t aarch64_smc_not(uint64_t id, uint64_t v64)
{
	return ~v64;
}

uintptr_t aarch64_smc_raise_el(unsigned int id,
				unsigned int target_el, void *ret)
{
	unsigned int cur_el = aarch64_current_el();
	unsigned int prev_el = 10;
	uint32_t spsr;

	switch (cur_el) {
	case 3:
		asm volatile ("mrs\t%0, spsr_el3\n" : "=r" (spsr));
		prev_el = (spsr >> 2) & 3;
		spsr &= ~(3 << 2);
		spsr |= target_el << 2;
		/* Force into 64-bit.  */
		spsr &= ~(1 << 4);
		asm volatile ("msr\tspsr_el3, %0\n" : : "r" (spsr));
		break;
	case 2:
		asm volatile ("mrs\t%0, spsr_el2\n" : "=r" (spsr));
		prev_el = (spsr >> 2) & 3;
		spsr &= ~(3 << 2);
		spsr |= target_el << 2;
		/* Force into 64-bit.  */
		spsr &= ~(1 << 4);
		asm volatile ("msr\tspsr_el2, %0\n" : : "r" (spsr));
		break;
	default:
		assert(0);
	}

	/* We don't know if the target EL has MMU and caches on.  */
	plat_cache_flush();
	assert(target_el >= prev_el);
	return 0;
}


typedef uint64_t (*smc_callback)(uint64_t id,
                             uint64_t arg1,
                             uint64_t arg2,
                             uint64_t arg3,
                             uint64_t arg4,
                             uint64_t arg5,
                             uint64_t sess_id,
                             uint32_t hyp_id);

smc_callback aarch64_smc_psci_handlers[] = {
	[SMC_PSCI_CPU_ON & 0xffff] = (smc_callback) aarch64_psci_on,
	[SMC_PSCI_CPU_OFF & 0xffff] = (smc_callback) aarch64_psci_self_suspend,
};

smc_callback aarch64_smc_oem_handlers[] = {
	(smc_callback) aarch64_smc_not,
	(smc_callback) aarch64_smc_raise_el
};

uint64_t aarch64_smc_decode(struct excp_frame *f)
{
	unsigned int cur_el = aarch64_current_el();
	uint64_t r;
	smc_callback fcb = NULL;
	uint64_t id = f->x[0];

	cur_el = cur_el;
	D(printf("EL%d SMC=%llx [0x%llx, 0x%llx 0x%llx 0x%llx, 0x%llx]\n",
		cur_el, id, f->x[1],
		f->x[2], f->x[3],
		f->x[4], f->x[5]));

	if (id > SMC_PSCI_BASE && id <= SMC_PSCI_MAX) {
		uint32_t sid = id & 0xffff;
		if (sid < ARRAY_SIZE(aarch64_smc_psci_handlers))
			fcb = aarch64_smc_psci_handlers[sid];
	}

	if (id >= SMC_TBM_BASE && id <= SMC_TBM_MAX) {
		uint32_t sid = id & 0xffff;
		if (sid < ARRAY_SIZE(aarch64_smc_oem_handlers))
			fcb = aarch64_smc_oem_handlers[sid];
	}

        if (!fcb) {
		printf("Unhandled SMC call\n");
		return -1;
	}
	r = fcb(f->x[0], f->x[1],
		f->x[2], f->x[3],
		f->x[4], f->x[5],
		f->x[6], f->x[7]);
	return r;
}
