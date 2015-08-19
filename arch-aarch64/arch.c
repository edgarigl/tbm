#include <stdio.h>
#include <stdint.h>
#include "sys.h"

void __arch_early_init(void)
{
	extern char vectors;
	const unsigned int current_el = aarch64_current_el();
	uint64_t cr;

	aarch64_hel = current_el;
	switch (current_el) {
	case 3:
		asm volatile ("msr\tvbar_el3, %0\n" : : "r" (&vectors));
		aarch64_mrs(cr, "scr_el3");
		/* We want to catch Serrors in EL3.  */
		cr = SCR_EA | SCR_RW | SCR_HCE | SCR_NS;
		aarch64_msr("scr_el3", cr);
		aarch64_msr("sctlr_el3", 0);
		aarch64_msr("tcr_el3", 0);
		ibarrier();
		mb();
		__asm__ __volatile__ ("tlbi alle3\n");
	case 2:
		asm volatile ("msr\tvbar_el2, %0\n" : : "r" (&vectors));
		aarch64_mrs(cr, "hcr_el2");
		cr = 0;
		aarch64_msr("hcr_el2", cr);
		aarch64_msr("sctlr_el2", 0);
		aarch64_msr("tcr_el2", 0);
		ibarrier();
		mb();
		__asm__ __volatile__ ("tlbi alle2\n");
	case 1:
		asm volatile ("msr\tvbar_el1, %0\n" : : "r" (&vectors));
		aarch64_msr("sctlr_el1", 0);
		aarch64_msr("tcr_el1", 0);
		ibarrier();
		mb();
		__asm__ __volatile__ ("tlbi alle1\n");
		break;
	}
	ibarrier();
}

void arch_init(void)
{
}
