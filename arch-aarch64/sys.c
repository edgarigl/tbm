#include <stdio.h>
#include <stdint.h>
#include "sys.h"

unsigned long __stack_chk_guard = 0;

void __stack_chk_fail(void)
{
	puts(__func__);
	err();
}


static const char *dfsc_str[] = {
	[0] = "Address size",
	[1] = "Translation fault",
	[2] = "Access fault",
	[3] = "Permission fault",
	[4] = "Sync ext abort on access",
	[6] = "Sync parity on access",
	[5] = "Sync ext abort on PTW",
	[7] = "Sync parity err on PTW",
};

void sys_dump_dfsc(unsigned int dfsc)
{
	unsigned int code, level;

	code = dfsc >> 2;
	level = dfsc & 3;
	printf("DFSC=%x %s level %d\n", dfsc, dfsc_str[code], level);
}

#define DUMP_CPREG64(n) do { \
	uint64_t r;\
	__asm__ __volatile__ ("mrs %0," n : "=r" (r)); \
	printf(n  "= %llx\n", r); \
} while (0);

void sys_dump_regs(struct excp_frame *f)
{
	const unsigned int el = aarch64_current_el();
	unsigned int i;

	printf("CurrentEL=%d\n", el);
	if (f) {
		for (i = 0; i < ARRAY_SIZE(f->x); i++) {
			printf("x[%d] = %llx%c", i, f->x[i], i & 1 ? '\n' : '\t');
		}
	}
	printf("\n");
	if (el >= 1) DUMP_CPREG64("elr_el1");
	if (el >= 2) DUMP_CPREG64("elr_el2");
	if (el >= 3) DUMP_CPREG64("elr_el3");
	if (el >= 1) DUMP_CPREG64("spsr_el1");
	if (el >= 2) DUMP_CPREG64("spsr_el2");
	if (el >= 3) DUMP_CPREG64("spsr_el3");

	if (el >= 1) DUMP_CPREG64("esr_el1");
	if (el >= 2) DUMP_CPREG64("esr_el2");
	if (el >= 3) DUMP_CPREG64("esr_el3");

	if (el >= 1) DUMP_CPREG64("far_el1");
	if (el >= 2) DUMP_CPREG64("far_el2");
	if (el >= 3) DUMP_CPREG64("far_el3");

	if (el >= 1) DUMP_CPREG64("sctlr_el1");
	if (el >= 2) DUMP_CPREG64("sctlr_el2");
	if (el >= 3) DUMP_CPREG64("sctlr_el3");

	printf("L2ECTLR: ");
	if (el >= 1) DUMP_CPREG64("S3_1_C11_C0_3");
	if (el >= 2) DUMP_CPREG64("vtcr_el2");
	if (el >= 2) DUMP_CPREG64("vttbr_el2");

	if (el >= 1) DUMP_CPREG64("ttbr0_el1");
	if (el >= 1) DUMP_CPREG64("tcr_el1");
	if (el >= 1) DUMP_CPREG64("sctlr_el1");

	if (el >= 2) DUMP_CPREG64("ttbr0_el2");
	if (el >= 2) DUMP_CPREG64("tcr_el2");
	if (el >= 2) DUMP_CPREG64("sctlr_el2");

	if (el >= 3) DUMP_CPREG64("ttbr0_el3");
	if (el >= 3) DUMP_CPREG64("tcr_el3");
	if (el >= 3) DUMP_CPREG64("sctlr_el3");

	if (el >= 2) DUMP_CPREG64("hcr_el2");
	if (el >= 2) {
		uint64_t hcr;
		aarch64_mrs(hcr, "hcr_el2");
		printf("HCR=%llx VM=%lld SWIO=%lld PTW=%lld FMO=%lld "
			"IMO=%lld AMO=%lld VF=%lld VI=%lld\n"
			"VSE=%lld FB=%lld DC=%lld TWI=%lld TWE=%lld\n"
			"TID0=%lld TID1=%lld TID2=%lld TID3=%lld\n"
			"TSC=%lld HCD=%lld RW=%lld CD=%lld ID=%lld\n",
			hcr,
			hcr & HCR_VM,
			hcr & HCR_SWIO,
			hcr & HCR_PTW,
			hcr & HCR_FMO,
			hcr & HCR_IMO,
			hcr & HCR_AMO,
			hcr & HCR_VF,
			hcr & HCR_VI,
			hcr & HCR_VSE,
			hcr & HCR_FB,
			hcr & HCR_DC,
			hcr & HCR_TWI,
			hcr & HCR_TWE,
			hcr & HCR_TID0,
			hcr & HCR_TID1,
			hcr & HCR_TID2,
			hcr & HCR_TID3,
			hcr & HCR_TSC,
			hcr & HCR_HCD,
			hcr & HCR_RW,
			hcr & HCR_CD,
			hcr & HCR_ID);
	}
	if (el >= 3) {
		uint32_t scr;
		aarch64_mrs(scr, "scr_el3");
		printf("SCR=%x TWE=%d TWI=%d RW=%d HCE=%d "
			"SMD=%d IRQ=%d FIQ=%d NS=%d\n", scr,
			scr & SCR_TWE,
			scr & SCR_TWI,
			scr & SCR_RW,
			scr & SCR_HCE,
			scr & SCR_SMD,
			scr & SCR_IRQ,
			scr & SCR_FIQ,
			scr & SCR_NS);
	}
	if (el >= 1) DUMP_CPREG64("mair_el1");
	if (el >= 2) DUMP_CPREG64("mair_el2");
	if (el >= 3) DUMP_CPREG64("mair_el3");
	printf("\n");
}

void aarch64_drop_el(const enum aarch64_el el, bool stack_h)
{
	const unsigned int spsr_w = el << 2 | stack_h;
	const unsigned int current_el = aarch64_current_el();
	unsigned char *newpc;
	uintptr_t sp;

	assert(current_el >= el);

	if (current_el == el)
		return;

	/* Don't know about the MMU/Cache state of the target.  */
	plat_cache_flush();
	newpc = &&done;

	/* GCC ???  */
	newpc += 8;

	switch (current_el) {
	case 3:
		asm volatile ("msr\telr_el3, %0\n"
			      "\tmsr\tspsr_el3, %1\n"
				: : "r" (newpc), "r" (spsr_w));
		break;
	case 2:
		asm volatile ("msr\telr_el2, %0\n"
			      "\tmsr\tspsr_el2, %1\n"
				: : "r" (newpc), "r" (spsr_w));
		break;
	case 1:
		asm volatile ("msr\telr_el1, %0\n"
			      "\tmsr\tspsr_el1, %1\n"
				: : "r" (newpc), "r" (spsr_w));
		break;
	}

	switch (el) {
	case EL2:
		asm volatile ("msr\tsp_el2, %0\n"
				: : "r" (0xeddebeef));
		break;
	case EL1:
		asm volatile ("msr\tsp_el1, %0\n"
				: : "r" (0xeddebeef));
		break;
	case EL0:
		asm volatile ("msr\tsp_el0, %0\n"
				: : "r" (0xeddebeef));
		break;
	default:
		assert(0);
		break;
	}

	/* Setup the stack to what it used to be.  */
	asm volatile ("mov\tx17, sp\n"
		      "\teret\n");
done:
	asm volatile ("mov\t%0, sp\n" : "=r" (sp));
	asm volatile ("mov\tsp, x17\n");

	assert(sp == 0xeddebeef);
	return;
}

void aarch64_raise_el(unsigned int target_el)
{
        uintptr_t old_sp;

        asm volatile ("mov\t%0, sp\n" : "=r" (old_sp));
        aarch64_smc(SMC_TBM_RAISE_EL, target_el, 0, 0, 0, 0, 0, 0);
        asm volatile ("mov\tsp, %0\n" : : "r" (old_sp));
}

/* Dummy.  */
__attribute__ ((weak)) void cpu_test(void)
{
}

__attribute__ ((weak)) void app_run(void)
{
}
