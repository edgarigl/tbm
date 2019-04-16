#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "regops.h"
#include "devtree.h"
#include "mmu.h"
#include "smmu.h"

#define D(x)

/* We should really be registering this kind of drivers some where
 * but got no time now, so we'll just hack it into a global.  */
static struct smmu {
	phys_addr_t base;
	unsigned int cb[2];
	struct mmu_ctx *ctx[2];
} smmu;

static unsigned int dec_bussize(unsigned int v)
{
	unsigned int map[8] = {
		[0] = 32,
		[1] = 36,
		[2] = 40,
		[3] = 42,
		[4] = 44,
		[5] = 48,
	};

	v &= 7;
	return map[v];
}

static unsigned int cb_offset(unsigned int cb)
{
	return cb * (R_SMMU500_SMMU_CB1_SCTLR - R_SMMU500_SMMU_CB0_SCTLR);
}

bool smmu_map(bool s1, bool s2, bool write, uintptr_t va, uintptr_t *pa)
{
	struct smmu *m = &smmu;
	uintptr_t regs[] = {
		[0] = R_SMMU500_SMMU_GATS1PR,
		[1] = R_SMMU500_SMMU_GATS12PR
	};
	uint64_t addr;
	uint64_t v = va;
	uint64_t par;
	bool err;

	assert(s1);
	addr = m->base + regs[s2];
	addr += write * 8;

	v |= m->cb[0];

	reg64_write(addr, v);
	mb();
	reg32_waitformask(m->base + R_SMMU500_SMMU_GATSR, 1, 0);
	par = reg64_read(m->base + R_SMMU500_SMMU_GPAR);
	err = par & 1;
	if (!err) {
		*pa = par & ~((1ULL << 12) - 1);
		*pa &= ((1ULL << 48) - 1);
	}
#if 0
	printf("smmu: S%s%s err=%d %lx -> %lx\n",
		s1 ? "1" : "",
		s2 ? "2" : "",
		err, va, *pa);
#endif
	return err;
}

void smmu_setup_tcr(struct smmu *m)
{
	unsigned int i;

	for (i = 0; i < 2; i++) {
		uint64_t tcr;

		tcr = m->ctx[i]->tcr;
		tcr |= SMMU500_SMMU_CB0_TCR_LPAE_EAE_MASK;
		writel(m->base + cb_offset(m->cb[i]) + R_SMMU500_SMMU_CB0_TCR_LPAE,
				tcr);
		writel(m->base + cb_offset(m->cb[i]) + R_SMMU500_SMMU_CB0_TCR2,
				tcr >> 32);
	}
	mb();
}

void smmu_setup_mair(struct smmu *m)
{
	/* MAIR is only used for stage1.  */
	writel(m->base + cb_offset(m->cb[0]) + R_SMMU500_SMMU_CB0_PRRR_MAIR0,
		m->ctx[0]->mair);
	mb();
}

void smmu_init_ctx(struct mmu_ctx *s1_mmu, struct mmu_ctx *s2_mmu,
			unsigned int s1_cb, unsigned int s2_cb,
			unsigned int el, bool enable)
{
	struct smmu *m = &smmu;
	uint32_t r;

	m->cb[0] = s1_cb;
	m->cb[1] = s2_cb;
	m->ctx[0] = s1_mmu;
	m->ctx[1] = s2_mmu;

	aarch64_mmu_compute_tcr(s1_mmu, el);
	aarch64_mmu_compute_tcr(s2_mmu, el);
	aarch64_mmu_compute_mair(s1_mmu, el);
	smmu_setup_mair(m);
	smmu_setup_tcr(m);

	writel(m->base + R_SMMU500_SMMU_CBA2R0, SMMU500_SMMU_CBA2R0_VA64_MASK);

	r = readl(m->base + R_SMMU500_SMMU_SCR0);
	r &= ~SMMU500_SMMU_SCR0_CLIENTPD_MASK;
	writel(m->base + R_SMMU500_SMMU_SCR0, r);

	r = readl(m->base + R_SMMU500_SMMU_S2CR0);
	r &= ~SMMU500_SMMU_S2CR0_TYPE_MASK;
	writel(m->base + R_SMMU500_SMMU_S2CR0, r);

	writel(m->base + R_SMMU500_SMMU_SCR1, 0x00000101);
	writel(m->base + R_SMMU500_SMMU_SMR0, 0xFFFE0001);

	if (enable && m->ctx[0]) {
		phys_addr_t cb_base = m->base + cb_offset(s1_cb);

		reg32_write64le(cb_base + R_SMMU500_SMMU_CB0_TTBR0_LOW,
				(uintptr_t) m->ctx[0]->pt.root);
		/* No support for split yet.  */
		reg32_write64le(cb_base + R_SMMU500_SMMU_CB0_TTBR1_LOW,
				(uintptr_t) m->ctx[1]->pt.root);

		writel(m->base + R_SMMU500_SMMU_CBAR0 + s1_cb * 4,
			s2_cb << 8 |
			3 << SMMU500_SMMU_CBAR0_TYPE_SHIFT);

		mb();

		r = readl(cb_base + R_SMMU500_SMMU_CB0_SCTLR);
		D(printf("SMMU: SCTLR %x - >%x\n", r,
			r | SMMU500_SMMU_CB0_SCTLR_M_MASK));
		r |= SMMU500_SMMU_CB0_SCTLR_M_MASK;
		writel(cb_base + R_SMMU500_SMMU_CB0_SCTLR, r);
		mb();
	}

	if (enable && m->ctx[1]) {
		phys_addr_t cb_base = m->base + cb_offset(s2_cb);

		reg32_write64le(cb_base + R_SMMU500_SMMU_CB0_TTBR0_LOW,
				(uintptr_t) m->ctx[1]->pt.root);
		/* No support for split yet.  */
		reg32_write64le(cb_base + R_SMMU500_SMMU_CB0_TTBR1_LOW,
				(uintptr_t) m->ctx[1]->pt.root);
		r = readl(cb_base + R_SMMU500_SMMU_CB0_SCTLR);
		D(printf("SMMU: SCTLR %x - >%x\n", r,
			r | SMMU500_SMMU_CB0_SCTLR_M_MASK));
		r |= SMMU500_SMMU_CB0_SCTLR_M_MASK;
		writel(cb_base + R_SMMU500_SMMU_CB0_SCTLR, r);
		mb();
	}
	writel(m->base + R_SMMU500_SMMU_STLBIALL, 1);
	writel(m->base + R_SMMU500_SMMU_TLBIALLNSNH, 1);
	mb();
	writel(m->base + R_SMMU500_SMMU_STLBGSYNC, 1);
	mb();
	reg32_waitformask(m->base + R_SMMU500_SMMU_STLBGSTATUS, 1, 0);
}

static void smmu_probe_init(struct smmu *m)
{
	uint32_t r;

	r = readl(m->base + R_SMMU500_SMMU_SIDR7);

	printf("SMMU-500: v%d.%d at %lx %lx\n",
		reg32_field(r, SMMU500, SMMU_SIDR7, MAJOR),
		reg32_field(r, SMMU500, SMMU_SIDR7, MINOR), m->base, m->base + R_SMMU500_SMMU_SIDR0);

	r = readl(m->base + R_SMMU500_SMMU_SIDR0);
	printf("SMMU: cttw=%d btm=%d sec=%d stages=%s.%s str-match=%d ato-ns=%d nrsidb=%d extids=%d nrsmrg=%d\n",
		reg32_field(r, SMMU500, SMMU_SIDR0, CTTW),
		reg32_field(r, SMMU500, SMMU_SIDR0, BTM),
		reg32_field(r, SMMU500, SMMU_SIDR0, SES),
		reg32_field(r, SMMU500, SMMU_SIDR0, S1TS) ? "S1" : "",
		reg32_field(r, SMMU500, SMMU_SIDR0, S2TS) ? "S2" : "",
		reg32_field(r, SMMU500, SMMU_SIDR0, SMS),
		reg32_field(r, SMMU500, SMMU_SIDR0, ATOSNS),
		reg32_field(r, SMMU500, SMMU_SIDR0, NUMSIDB),
		r & (1 << 8),
		reg32_field(r, SMMU500, SMMU_SIDR0, NUMSMRG)
		);

	r = readl(m->base + R_SMMU500_SMMU_SIDR1);
	printf("SMMU: pagesize=%d nrpgidx=%d nrs2cb=%d smcd=%d nrcb=%d\n",
		reg32_field(r, SMMU500, SMMU_SIDR1, PAGESIZE),
		reg32_field(r, SMMU500, SMMU_SIDR1, NUMPAGENDXB),
		reg32_field(r, SMMU500, SMMU_SIDR1, NUMS2CB),
		reg32_field(r, SMMU500, SMMU_SIDR1, SMCD),
		reg32_field(r, SMMU500, SMMU_SIDR1, NUMCB));

	r = readl(m->base + R_SMMU500_SMMU_SIDR2);
	printf("SMMU: r=%x 64k=%d 16k=%d 4k=%d ubs=%d oas=%d ias=%d\n", r,
		reg32_field(r, SMMU500, SMMU_SIDR2, PTFSV8_64KB),
		reg32_field(r, SMMU500, SMMU_SIDR2, PTFSV8_16KB),
		reg32_field(r, SMMU500, SMMU_SIDR2, TFSV8_4KB),
		dec_bussize(reg32_field(r, SMMU500, SMMU_SIDR2, UBS)),
		dec_bussize(reg32_field(r, SMMU500, SMMU_SIDR2, OAS)),
		dec_bussize(reg32_field(r, SMMU500, SMMU_SIDR2, IAS))
		);

	printf("SMMU: idr[]=%x, %x, %x\n",
			readl(m->base + R_SMMU500_SMMU_SIDR0),
			readl(m->base + R_SMMU500_SMMU_SIDR1),
			readl(m->base + R_SMMU500_SMMU_SIDR2));
}

static bool smmu_probe(void *fdt, int node, const char *compat)
{
	phys_addr_t p;
	static bool bound = false;

	if (bound)
		return false;

	p = (phys_addr_t) dt_map(fdt, node, 0);
	printf("Bound %s at %lx\n", compat, p);
	smmu.base = p;
	bound = true;

	smmu_probe_init(&smmu);
	return true;
}

static struct fdt_probe fdt_smmu = {
	.probe = smmu_probe,
	.match = (struct fdt_match []) {
		{ .compat = "arm,mmu-500" },
		{},
	},
};

__fdt_probe(fdt_smmu);
