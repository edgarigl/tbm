#include <assert.h>

#define aarch64_mrs(V, SR) __asm__ __volatile__ ("mrs\t%0, " SR "\n" : "=r" (V))
#define aarch64_msr(SR, V) __asm__ __volatile__ ("msr\t" SR ", %0\n" : : "r" (V))

static inline uint32_t aarch64_esr(unsigned int el)
{
	uint32_t esr = 0;

	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_mrs(esr, "ESR_EL1");
		break;
	case 2:
		aarch64_mrs(esr, "ESR_EL2");
		break;
	case 3:
		aarch64_mrs(esr, "ESR_EL3");
		break;
	}
	return esr;
}

static inline uint32_t aarch64_sctlr(unsigned int el)
{
	uint32_t v = 0;

	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_mrs(v, "SCTLR_EL1");
		break;
	case 2:
		aarch64_mrs(v, "SCTLR_EL2");
		break;
	case 3:
		aarch64_mrs(v, "SCTLR_EL3");
		break;
	}
	return v;
}

static inline uint32_t aarch64_spsr(unsigned int el)
{
	uint32_t v = 0;

	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_mrs(v, "SPSR_EL1");
		break;
	case 2:
		aarch64_mrs(v, "SPSR_EL2");
		break;
	case 3:
		aarch64_mrs(v, "SPSR_EL3");
		break;
	}
	return v;
}

static inline uint64_t aarch64_elr(unsigned int el)
{
	uint64_t v = 0;

	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_mrs(v, "ELR_EL1");
		break;
	case 2:
		aarch64_mrs(v, "ELR_EL2");
		break;
	case 3:
		aarch64_mrs(v, "ELR_EL3");
		break;
	}
	return v;
}

static inline void aarch64_set_spsr(unsigned int el, uint64_t v)
{
	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_msr("SPSR_EL1", v);
		break;
	case 2:
		aarch64_msr("SPSR_EL2", v);
		break;
	case 3:
		aarch64_msr("SPSR_EL3", v);
		break;
	}
}

static inline void aarch64_set_elr(unsigned int el, uint64_t v)
{
	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_msr("ELR_EL1", v);
		break;
	case 2:
		aarch64_msr("ELR_EL2", v);
		break;
	case 3:
		aarch64_msr("ELR_EL3", v);
		break;
	}
}

static inline void aarch64_set_sctlr(unsigned int el, uint64_t v)
{
	switch (el) {
	case 0:
		assert(0);
		break;
	case 1:
		aarch64_msr("SCTLR_EL1", v);
		break;
	case 2:
		aarch64_msr("SCTLR_EL2", v);
		break;
	case 3:
		aarch64_msr("SCTLR_EL3", v);
		break;
	}
}

#include "aarch64-regs-asm.h"
