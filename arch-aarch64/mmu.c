#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "sys.h"
#include "mmu.h"

#define D(x)

static struct ptdesc *aarch64_mmu_alloc_table(struct mmu_ctx *mmu)
{
	unsigned int nr_entries = 1 << (mmu->grainsize - 3);
	struct ptdesc *ret = mmu->pt.pos;

	if (mmu->pt.pos + nr_entries >= mmu->pt.end) {
		printf("PT OOM\n");
		return NULL;
	}

	mmu->pt.pos += nr_entries;
	memset(ret, 0, nr_entries * sizeof *ret);
	D(printf("alloc table: %p nr_entries=%u\n", ret, nr_entries));
	return ret;
}

bool aarch64_mmu_map(struct mmu_ctx *mmu,
			uintptr_t va, uintptr_t pa,
			uintptr_t len, unsigned int prot,
			unsigned int attridx,
			bool secure)
{
	uintptr_t masks[] = {
		[PAGEKIND_TINY_4K]  = ((1 << 12) - 1),
		[PAGEKIND_MID_16K] = ((1 << 14) - 1),
		[PAGEKIND_HUGE_64K] = ((1 << 16) - 1),
	};
	uintptr_t sizes[3][4] = {
		[PAGEKIND_TINY_4K]  = {
			0, MAPSIZE_1G, MAPSIZE_2M, MAPSIZE_4K,
		},
	};
	uintptr_t mask = masks[mmu->pagekind];
	struct ptdesc *next = mmu->pt.root;
	struct ptdesc *pte;
	unsigned int level = 0;

	assert((va & mask) == 0);
	assert((pa & mask) == 0);
	assert((len & mask) == 0);

	D(printf("\nmap S%d %lx -> %lx\n", mmu->stage, va, pa));
	for (level = mmu->startlevel; level < 4; level++) {
		pte = aarch64_ptdesc(mmu, next, level, va);

		D(printf("\tL%d size=%lx len=%lx\n", level, sizes[mmu->pagekind][level], len));
		if (sizes[mmu->pagekind][level] == len) {
			/* Level 3 descriptors have a different encoding.  */
			bool is_table = level == 3;
			if (mmu->stage == 1) {
				unsigned int ap = prot & PROT_WRITE ? 0 : 2;

				pte[0] = (struct ptdesc) {
					.block_s1 = {
						.is_valid = 1,
						.is_table = is_table,
						.af = prot ? 1 : 0,
						.ap = ap,
						.attrindex = attridx,
						.addr = aarch64_block_addr(mmu, level, pa),
					},
			        };
				D(printf("\tblock[L%d][%p] -> %lx ap=%x\n",
					level, pte, pte->block_s1.addr << 12, ap));

			} else {
				unsigned int ap = prot & PROT_WRITE ? 2 : 0;
				ap |= prot & PROT_READ ? 1 : 0;

				assert(mmu->stage == 2);
				pte[0] = (struct ptdesc) {
					.block_s2 = {
						.is_valid = 1,
						.is_table = is_table,
						.af = 1,
						.s2ap = ap,
						.memattr = 0,
						.addr = aarch64_block_addr(mmu, level, pa),
					},
			        };
				D(printf("\tblock[L%d][%p] -> %lx s2ap=%x\n",
					level, pte, pte->block_s2.addr << 12, ap));

			}
			break;
		} else {
			assert(level < 3);
			if (pte->sw.swdef & PTE_ALLOCATED) {
				next = (struct ptdesc *) (uintptr_t) (pte->table.addr << 2);
				continue;
			} else {
				next = aarch64_mmu_alloc_table(mmu);
				if (!next)
					return false;
			}

			pte[0] = (struct ptdesc) {
				.table = {
					.is_valid = 1,
					.is_table = 1,
					.addr = ((uintptr_t) next) >> 2,
				},
		        };
			pte->sw.swdef |= PTE_ALLOCATED;
			next = (struct ptdesc *) (uintptr_t) (pte->table.addr << 2);
			D(printf("\ttable[L%d][%p] -> %lx (%lx)\n",
				level, pte, pte->table.addr << 2, pte->table.addr));
		}
	}
	mb();
	return true;
}

void aarch64_mmu_compute_tcr(struct mmu_ctx *mmu, unsigned int el)
{
	uint64_t tcr;

	tcr = TCR_IRGN0_NONCACHE | TCR_ORGN0_NONCACHE \
		| TCR_SH0_INNER_SHARE | TCR_TG0_4K_MASK;
	if (el > 1 || mmu->stage == 2)
		tcr |= TCR_PHYSSIZE_40_1TB_MASK;
	if (mmu->stage == 2) {
		tcr |= 25;
		tcr |= TCR_SL0_4K_L1;
		mmu->tcr = tcr;
		return;
	}

	tcr |= 16;
	switch (el) {
	case 1:
		tcr |= TCR_PHYSSIZE_40_1TB_MASK << 16;
		tcr |= 16 << 16;
		break;
	default:
		break;
	};
	mmu->tcr = tcr;
}

void aarch64_mmu_setup_tcr(struct mmu_ctx *mmu, unsigned int el)
{
	uint64_t tcr = mmu->tcr;

	if (mmu->stage == 2) {
		aarch64_msr("vtcr_el2", tcr);
		ibarrier();
		return;
	}

	switch (el) {
	case 1: aarch64_msr("tcr_el1", tcr); break;
	case 2: aarch64_msr("tcr_el2", tcr); break;
	case 3: aarch64_msr("tcr_el3", tcr); break;
	default: abort();
	};
	ibarrier();
}

void aarch64_mmu_compute_mair(struct mmu_ctx *mmu, unsigned int el)
{
	static const unsigned int attrs[] = {
		[MAIR_IDX_DEVICE] = MAIR_OUTER_DEVICE | MAIR_DEVICE_NG_NR_NE,
		[MAIR_IDX_DEVICE_E] = MAIR_OUTER_DEVICE | MAIR_DEVICE_NG_NR_E,
		[MAIR_IDX_MEM] = MAIR_OUTER_MEM_WB | MAIR_INNER_MEM_WB,
		[MAIR_IDX_MEM_NONCACHE] = MAIR_OUTER_MEM_NONCACHE | MAIR_INNER_MEM_NONCACHE,
	};
	uint64_t mair = 0;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(attrs); i++) {
		mair |= attrs[i] << (i * 8);
	}
	mmu->mair = mair;
}


void aarch64_mmu_setup_mair(struct mmu_ctx *mmu, unsigned int el)
{
	uint64_t mair = mmu->mair;

	switch (el) {
	case 1: aarch64_msr("mair_el1", mair); break;
	case 2: aarch64_msr("mair_el2", mair); break;
	case 3: aarch64_msr("mair_el3", mair); break;
	default: abort();
	};
	ibarrier();
}

static uint64_t aatch64_mmu_table_size(struct mmu_ctx *mmu,
					unsigned int level)
{
	unsigned int stride = mmu->grainsize - 3;

	int bits = mmu->virtbits - mmu->grainsize  - (3 - level) * stride;
	if (bits <= 0)
		return 0;
	return (1 << bits) * 3;
}

void aarch64_mmu_pt_zap(struct mmu_ctx *mmu, unsigned int el)
{
	/* Free all page tables, realloc the root.  */
//	memset(mmu->pt.root, 0, (char *)mmu->pt.end - (char *)mmu->pt.root);
	mmu->pt.pos = mmu->pt.root;
	mmu->pt.root = aarch64_mmu_alloc_table(mmu);
	aarch64_mmu_tlb_flush(mmu, el);
}

uintptr_t aarch64_mmu_pt_setup(struct mmu_ctx *mmu,
			enum pagekind pagekind,
			unsigned int stage,
			unsigned int virtbits,
			unsigned int physbits,
			void *start, void *end)
{
	uintptr_t pagemap[] = {
		[PAGEKIND_TINY_4K]  = PAGESIZE_4K,
		[PAGEKIND_MID_16K]  = PAGESIZE_16K,
		[PAGEKIND_HUGE_64K] = PAGESIZE_64K,
	};
	uintptr_t pagebits_map[] = {
		[PAGEKIND_TINY_4K]  = 12,
		[PAGEKIND_MID_16K]  = 14,
		[PAGEKIND_HUGE_64K] = 16,
	};
	unsigned int i;
	uintptr_t pos = (uintptr_t) start;

	mmu->stage = stage;
	mmu->pagekind = pagekind;
	mmu->pagesize = pagemap[pagekind];
	mmu->physbits = physbits;
	mmu->virtbits = virtbits;
	mmu->grainsize = pagebits_map[mmu->pagekind];

	mmu->startlevel = 0;
	mmu->pt.pos = start;
	mmu->pt.end = end;
	for (i = 0; i < 4; i++) {
		uint64_t size = aatch64_mmu_table_size(mmu, i);
		if (size == 0) {
			mmu->startlevel = i + 1;
			continue;
		}
		pos += size;
	}
	mmu->pt.root = aarch64_mmu_alloc_table(mmu);


	D(printf("PT: S%d startlevel=%d vs=%u ps=%u end=%lx\n",
		stage, mmu->startlevel, virtbits, physbits, pos));
	return 0;
}

void aarch64_mmu_setup(struct mmu_ctx *mmu, unsigned int el, bool enable)
{
	uint32_t sctlr, tmp;

	D(printf("%s: el=%d stage=%d enable=%d\n", __func__, el, mmu->stage, enable));
	if (!enable) {
		if (el < 2 && mmu->stage == 2) {
			aarch64_msr("vttbr_el2", 0);
			aarch64_mrs(tmp, "hcr_el2");
			tmp &= ~HCR_VM;
			mb();
			aarch64_msr("hcr_el2", tmp);
			ibarrier();
		}
		switch (el) {
		case 1:
			aarch64_mrs(sctlr, "sctlr_el1");
			sctlr &= ~SCTLR_M;
			mb();
			aarch64_msr("sctlr_el1", sctlr);
			ibarrier();
			break;
		default:
			assert(0);
		};
		aarch64_mmu_tlb_flush(mmu, el);
		return;
	}


	aarch64_mmu_compute_mair(mmu, el);
	aarch64_mmu_compute_tcr(mmu, el);
	aarch64_mmu_setup_mair(mmu, el);
	aarch64_mmu_setup_tcr(mmu, el);

	if (enable) {
		uintptr_t ttbr = (uintptr_t) mmu->pt.root;
		if (el < 2) {
			if (mmu->stage == 2) {
				aarch64_msr("vttbr_el2",ttbr);
				ibarrier();
				aarch64_mrs(tmp, "hcr_el2");
				tmp |= HCR_VM;
				mb();
				aarch64_msr("hcr_el2", tmp);
				ibarrier();
				return;
			}
		}
		switch (el) {
		case 1:
			aarch64_msr("ttbr0_el1", ttbr);
			/* We don't suppor the split yet.  */
			aarch64_msr("ttbr1_el1", ttbr);
			ibarrier();
			aarch64_mrs(sctlr, "sctlr_el1");
			sctlr |= SCTLR_M;
			mb();
			aarch64_msr("sctlr_el1", sctlr);
			break;
		case 3:
			aarch64_msr("ttbr0_el3",ttbr);
			/* ttbr0 must be written prior to enabling the MMU.  */
			ibarrier();

			aarch64_mrs(sctlr, "sctlr_el3");
			sctlr |= SCTLR_M;
			/* Make sure all outstanding mem transactions are done.  */
			mb();
			aarch64_msr("sctlr_el3", sctlr);
			ibarrier();
			break;
		default:
			assert(0);
		}
	}
}
