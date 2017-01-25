#include <stdio.h>
#include <stdint.h>
#include "sys.h"

#define D(x)

static unsigned int excp_count[4][EC_AA64_BKPT + 1];
static unsigned int ex_data_abort_expected = false;

void aarch64_excp_dataabort_expect(unsigned int f)
{
	if (f ==0 && ex_data_abort_expected) {
		printf("lost serrors %d\n", ex_data_abort_expected);
	}
	ex_data_abort_expected = f;
}

unsigned int aarch64_excp_count(unsigned int el, unsigned int ec)
{
	assert(el > 0 && el <= ARRAY_SIZE(excp_count));
	assert(ec < ARRAY_SIZE(excp_count[0]));
	return excp_count[el][ec];
}

void aarch64_excp_decode(struct excp_frame *f)
{
	const unsigned int el = aarch64_current_el();
	uint32_t esr = aarch64_esr(el);
	uint32_t spsr = aarch64_spsr(el);
	uint64_t elr = aarch64_elr(el);
	unsigned int ec = esr >> 26;
	uint64_t r = 0;

	assert(ec < ARRAY_SIZE(excp_count[0]));
	excp_count[el][ec]++;

	switch (ec) {
	case EC_UNCATEGORIZED:
		printf("UDEF at EL%d PC=%llx\n", el, elr);
		r = elr;
		f->x[1] = * (uint32_t *) elr;
		elr += 4;
		break;
	case EC_AA64_SMC:
		r = aarch64_smc_decode(f);

		/* SMC routing to EL2, results in ELR at SMC.  */
		if (el == 2)
			elr += 4;
		break;
	case EC_AA64_HVC:
		r = elr;
		f->x[1] = * (uint32_t *) elr;
		break;
	case EC_DATAABORT:
	case EC_DATAABORT_SAME_EL:
		r = f->x[0];
		if (!ex_data_abort_expected) {
			printf("DATA-ABORT EC=%x ESR=%x at %llx\n", ec, esr, elr);
			sys_dump_dfsc(esr & ((1 << 6) - 1));
			sys_dump_regs(f);
		} else {
			ex_data_abort_expected--;
		}
		/* FIXME: Implement an exception table.  */
		elr += 4;
		break;
	case EC_SERROR:
		if (!ex_data_abort_expected) {
                        sys_dump_dfsc(esr & ((1 << 6) - 1));
                        sys_dump_regs(f);
			panic("Serror");
		} else {
			ex_data_abort_expected--;
		}
		break;
	default:
		printf("\nUnexpected Trap!\n");
		printf("From EL%d ELR=%llx ESR=%x EC=%x\n",
			(spsr >> 2) & 3, elr, esr, ec);
		sys_dump_regs(f);
		panic("");
		break;
	}
	aarch64_set_elr(el, elr);
	f->x[0] = r;
}

static uint64_t irq_stamp[4];
static unsigned int fiq_count[4] = {0};
static unsigned int irq_count[4] = {0};

unsigned int aarch64_get_nr_fiqs(unsigned int el)
{
	return fiq_count[el];
}

unsigned int aarch64_get_nr_irqs(unsigned int el)
{
	return irq_count[el];
}

uint64_t aarch64_irq_get_stamp(unsigned int el)
{
	return irq_stamp[el];
}

static aarch64_excp_h_p irq_h = NULL;
static aarch64_excp_h_p fiq_h = NULL;

void aarch64_set_fiq_h(aarch64_excp_h_p f)
{
	fiq_h = f;
}

aarch64_excp_h_p aarch64_get_fiq_h(void)
{
	return fiq_h;
}

void aarch64_set_irq_h(aarch64_excp_h_p f)
{
	irq_h = f;
}

aarch64_excp_h_p aarch64_get_irq_h(void)
{
	return irq_h;
}

void aarch64_fiq(struct excp_frame *f)
{
	const unsigned int el = aarch64_current_el();
	fiq_count[el]++;

	if (fiq_h) {
		fiq_h(f);
	}
}

void aarch64_irq(struct excp_frame *f)
{
	unsigned int el;
	uint64_t t;

	/* There's an isb in the asm stub protecting the counter read
	 * from happening before aarch64_irq. Even without it, it
	 * would spectacular if the CPU could speculatively take
	 * an interrupt.  */
	aarch64_mrs(t, "cntvct_el0");
	__asm__ __volatile__ ("isb");
	el = aarch64_current_el();

	irq_stamp[el] = t;
	irq_count[el]++;

	if (irq_h) {
		irq_h(f);
	}
}
