#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void check_hvc_from_el2(void)
{
	unsigned int cur_el = aarch64_current_el();
	unsigned long udef_start, udef_end;
	uint32_t insn;
	uint32_t scr;

	printf("%s:\n", __func__);
	assert(aarch64_current_el() == 3);
	aarch64_mrs(scr, "scr_el3");
	scr |= SCR_HCE | SCR_RW | SCR_NS;
	aarch64_msr("scr_el3", scr);
	aarch64_drop_el(EL2, SPXH);

	__asm__ __volatile__ ("hvc 0\nisb\n");
	__asm__ __volatile__ ("mov %0, x1\n" : "=r" (insn));
	/* Make sure the UDEF raises on the SMC itself.  */
	printf("insn = %x\n", insn);
	aarch64_raise_el(EL3);
	assert(insn == 0xd5033fdf);

	/* Disable HVC and check the UDEF behaviour.  */
	scr &= ~SCR_HCE;
	aarch64_msr("scr_el3", scr);
	aarch64_drop_el(EL2, SPXH);

	udef_start = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	__asm__ __volatile__ ("hvc 0\nisb\n");
	__asm__ __volatile__ ("mov %0, x1\n" : "=r" (insn));
	printf("insn = %x\n", insn);
	aarch64_raise_el(EL3);
	assert(insn == 0xd4000002);

	udef_end = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	assert(udef_start == (udef_end));
}

static void check_hvc_from_el3(void)
{
	unsigned int cur_el = aarch64_current_el();
	unsigned long udef_start, udef_end;
	uint32_t insn;
	uint32_t scr;

	assert(aarch64_current_el() == 3);
	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_HCE;
	aarch64_msr("scr_el3", scr);

	printf("%s: cur_el=%d\n", __func__, cur_el);
	udef_start = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	__asm__ ("hvc 0\n");
	__asm__ ("mov %0, x1\n" : "=r" (insn));
	/* Make sure the UDEF raises on the SMC itself.  */
	printf("insn = %x\n", insn);
	assert(insn == 0xd4000002);

	udef_end = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	assert(udef_start == (udef_end - 1));
}

static void check_simple_udef(void)
{
	unsigned int cur_el = aarch64_current_el();
	unsigned long udef_start, udef_end;
	uint64_t v = 0x1234;
	uint32_t insn;
	uint32_t scr;
	uint64_t r;

	printf("%s:\n", __func__);
	/* Disable SMC.  */
	aarch64_mrs(scr, "scr_el3");
	scr |= SCR_SMD;
	aarch64_msr("scr_el3", scr);

	udef_start = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	__asm__ __volatile__ ("mov %0, x1\n" : "=r" (insn));
	/* Make sure the UDEF raises on the SMC itself.  */
	printf("insn = %x\n", insn);
	assert(insn == 0xd4000003);
	assert(r != ~v);

	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_SMD;
	aarch64_msr("scr_el3", scr);

	udef_end = aarch64_excp_count(cur_el, EC_UNCATEGORIZED);
	assert(udef_start == (udef_end - 1));
}

static void check_simple_not_from_el1(void)
{
	unsigned int smc_start, smc_end;
	uint32_t scr;
	uint64_t hcr;
	uint64_t v = 0x123456789abcdefULL;
	uint64_t r;

	printf("%s:\n", __func__);
	aarch64_msr("tcr_el1", 0);
	aarch64_msr("sctlr_el1", 0);

	assert(aarch64_current_el() == 3);
	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_SMD;
	scr |= SCR_NS | SCR_RW;
	aarch64_msr("scr_el3", scr);
	aarch64_mrs(hcr, "hcr_el2");
	hcr &= ~HCR_TSC;
	hcr |= HCR_RW;
	aarch64_msr("hcr_el2", hcr);
	aarch64_drop_el(EL1, SPXH);

	smc_start = aarch64_excp_count(3, EC_AA64_SMC);
	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	assert(r == ~v);
	smc_end = aarch64_excp_count(3, EC_AA64_SMC);
	aarch64_raise_el(EL3);
	assert(smc_end == smc_start + 1);

	printf("%s: Route to EL2\n", __func__);
	assert(aarch64_current_el() == 3);
	aarch64_mrs(hcr, "hcr_el2");
	hcr |= HCR_TSC | HCR_RW;
	aarch64_msr("hcr_el2", hcr);
	aarch64_mrs(hcr, "hcr_el2");
	aarch64_mrs(scr, "scr_el3");
	printf("scr=%x\n", scr);
	printf("hcr=%llx\n", hcr);
	aarch64_drop_el(EL1, SPXH);

	smc_start = aarch64_excp_count(2, EC_AA64_SMC);
	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	assert(r == ~v);
	smc_end = aarch64_excp_count(2, EC_AA64_SMC);
	printf("start=%d end=%d\n", smc_start, smc_end);

	/* We are at EL1, need to raise to EL2, then disable TSC and
	   goto EL3  */
	aarch64_raise_el(EL2);
	aarch64_mrs(hcr, "hcr_el2");
	hcr &= ~HCR_TSC;
	aarch64_msr("hcr_el2", hcr);
	aarch64_raise_el(EL3);
	assert(smc_end == smc_start + 1);
}

static void check_simple_not_from_el2(void)
{
	unsigned int smc_start, smc_end;
	uint32_t scr;
	uint64_t hcr;
	uint64_t v = 0x123456789abcdefULL;
	uint64_t r;

	printf("%s:\n", __func__);
	assert(aarch64_current_el() == 3);
	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_SMD;
	scr |= SCR_NS | SCR_RW;
	aarch64_msr("scr_el3", scr);
	aarch64_drop_el(EL2, SPXH);

	smc_start = aarch64_excp_count(3, EC_AA64_SMC);
	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	assert(r == ~v);
	smc_end = aarch64_excp_count(3, EC_AA64_SMC);
	aarch64_raise_el(EL3);
	assert(smc_end == smc_start + 1);

	printf("%s: Route to EL2\n", __func__);
	aarch64_mrs(hcr, "hcr_el2");
	hcr |= HCR_TSC;
	aarch64_msr("hcr_el2", hcr);
	aarch64_drop_el(EL2, SPXH);

	smc_start = aarch64_excp_count(3, EC_AA64_SMC);
	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	assert(r == ~v);
	smc_end = aarch64_excp_count(3, EC_AA64_SMC);

	/* Disable SMC routing to EL2, before trying to raise EL.  */
	aarch64_mrs(hcr, "hcr_el2");
	hcr &= ~HCR_TSC;
	aarch64_msr("hcr_el2", hcr);
	mb();
	aarch64_raise_el(EL3);
	assert(smc_end == smc_start + 1);

}

static void check_simple_not(void)
{
	uint32_t scr;
	uint64_t v = 0x123456789abcdefULL;
	uint64_t r;

	printf("%s:\n", __func__);
	assert(aarch64_current_el() == 3);
	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_SMD;
	aarch64_msr("scr_el3", scr);

	r = aarch64_smc(SMC_TBM_NOT, v, 0, 0, 0, 0, 0, 0);
	assert(r == ~v);
}

static void check_hcr_mask(void)
{
	uint64_t hcr, hcr_new, hcr_prev;
	uint64_t hcr_res0 = (0x3ffffffUL << 39) | (0xfUL << 34);
	uint32_t scr;

	aarch64_mrs(scr, "scr_el3");
	aarch64_msr("scr_el3", scr | SCR_NS | SCR_RW);
	aarch64_mrs(hcr_prev, "hcr_el2");

	hcr_new = ~0;
	hcr_new &= ~hcr_res0;

	/* Not sure why but setting .VM fails on real HW. Disable for now.  */
	hcr_new &= ~1;
	printf("HCR=%llx prev=%llx\n", hcr_new, hcr_prev);
	aarch64_msr("hcr_el2", hcr_new);
	aarch64_mrs(hcr, "hcr_el2");
	printf("hcr=%llx\n", hcr);
	assert(hcr == 0x3dffffffeUL);
	barrier();

	aarch64_msr("hcr_el2", 0);
	aarch64_mrs(hcr, "hcr_el2");
	printf("hcr=%llx\n", hcr);
	assert(hcr == 0x0 || hcr == 2);

	aarch64_msr("hcr_el2", hcr_prev);
	aarch64_mrs(hcr, "hcr_el2");
	printf("restore HCR to %llx actual=%llx\n", hcr_prev, hcr);
	aarch64_msr("scr_el3", scr);
}

static void check_scr_mask(void)
{
	uint32_t scr, scr_prev;
	aarch64_mrs(scr_prev, "scr_el3");

	aarch64_msr("scr_el3", ~0);
	aarch64_mrs(scr, "scr_el3");
	printf("scr=%x\n", scr);
	assert(scr == 0x3fbf || scr == 0x3fff);

	aarch64_msr("scr_el3", 0);
	aarch64_mrs(scr, "scr_el3");
	printf("scr=%x\n", scr);
	assert(scr == 0x30 || scr == 0);

	aarch64_msr("scr_el3", scr_prev);
}

void a64_check_smc(void)
{
	unsigned int cur_el = aarch64_current_el();
	uint32_t scr;

	printf("%s: cur_el=%d\n", __func__, cur_el);

	if (cur_el != 3) {
		uint64_t hcr;

		/* Useful when running TBM after a failed run stuck
		 * in lower ELs.  */
		printf("%s: Try to raise EL to 3\n", __func__);
		if (cur_el < 2)
			aarch64_raise_el(EL2);
		aarch64_mrs(hcr, "hcr_el2");
		hcr &= ~HCR_TSC;
		aarch64_msr("hcr_el2", hcr);
		aarch64_raise_el(EL3);
		cur_el = aarch64_current_el();
		if (cur_el != 3) {
			printf("Warning, cannot run SMC tests at EL%d\n", cur_el);
			return;
		}
		assert(cur_el == 3);
	}
	aarch64_mrs(scr, "scr_el3");
	scr &= ~SCR_SMD;
	scr = SCR_HCE | SCR_NS;
	aarch64_msr("scr_el3", scr);

	check_scr_mask();
	check_hcr_mask();
	check_simple_not();
	check_simple_udef();
	check_simple_not_from_el2();
	check_simple_not_from_el1();
	check_hvc_from_el3();
	check_hvc_from_el2();
}
