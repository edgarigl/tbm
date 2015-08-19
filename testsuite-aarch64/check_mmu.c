#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libverif/verif.h"
#include "sys.h"
#include "mmu.h"

extern char _pt_s2_start;
extern char _pt_s2_end;

struct test_map {
	uintptr_t va;
	uintptr_t ipa;
	uintptr_t pa;
	uint64_t size;
	uint32_t prot;
	uint32_t prot2;
};

struct tester_map {
	struct mmu_ctx mmu[2];
	crt_constr crt_map[6 + 1];
};

#define HAVE_SMMU 0
#define PHYSMASK ((1ULL << 40) - 1)

#if HAVE_SMMU
#include "smmu.h"
#include "ronaldo-map.dtsh"
#endif

struct tester_map tmap = {
	.crt_map = {
	{ CRT_UINTPTR(struct test_map, va, 1), .uintptr = { .mask = PHYSMASK }, },
	{ CRT_UINTPTR(struct test_map, ipa, 1), .uintptr = { .mask = PHYSMASK }, },
	{ CRT_UINTPTR(struct test_map, pa, 1), .uintptr = { .mask = PHYSMASK }, },
	{ CRT_UINT64(struct test_map, size, 1), .uint64 = {
		.include = { .nr = 3,
			.values = (uint64_t []) { MAPSIZE_1G, MAPSIZE_2M, MAPSIZE_4K},
		}},
	},
	{ CRT_UINT32(struct test_map, prot, 1), .uint32 = { .mask = PROT_RW }, },
	{ CRT_UINT32(struct test_map, prot2, 1), .uint32 = { .mask = PROT_RW }, },
	{ },
	},
};

enum stages {
	S1,
	S12
};

#define D(x)
#define A64_AT(m, va) __asm__ __volatile__("at " m ", %0\n" : : "r" (va))

static void print_par(uint64_t par)
{
	unsigned int fst;

	if (!(par & PAR_F)) {
		printf("par=%llx\n", par);
		return;
	}

	fst = (par >> 1) & ((1 << 6) - 1);
	printf("Fault: par.ptw=%lld par.stage%d fst=%x\n",
		par & PAR_PTW, !!(par & PAR_S) + 1, fst);
	sys_dump_dfsc(fst);
}

static void dump_par(void)
{
	uint64_t par;

	aarch64_mrs(par, "PAR_EL1");
	print_par(par);
}

static bool map_smmu(enum stages stages,
			bool write, uintptr_t va, uintptr_t *pa)
{
	return smmu_map(true, stages == S12, write, va, pa);
}

static bool map_cpu(unsigned int el, enum stages stages,
			bool write, uintptr_t va, uintptr_t *pa)
{
	uint64_t par;
	bool err;

	switch (el) {
	case 0:
		switch (stages) {
		case S1:
			if (write)
				A64_AT("S1E0W", va);
			else
				A64_AT("S1E0R", va);
			break;
		default:
			abort();
		}
		break;
	case 1:
		switch (stages) {
		case S1:
			if (write)
				A64_AT("S1E1W", va);
			else
				A64_AT("S1E1R", va);
			break;
		break;
		case S12:
			if (write)
				A64_AT("S12E1W", va);
			else
				A64_AT("S12E1R", va);
			break;
		break;
		default:
			abort();
		}
		break;
	case 2:
		switch (stages) {
		case S1:
			if (write)
				A64_AT("S1E2W", va);
			else
				A64_AT("S1E2R", va);
			break;
		break;
		default:
			abort();
		}
		break;
	case 3:
		switch (stages) {
		case S1:
			if (write)
				A64_AT("S1E3W", va);
			else
				A64_AT("S1E3R", va);
			break;
		break;
		default:
			abort();
		}
		break;
	default:
		abort();
	}
	aarch64_mrs(par, "PAR_EL1");
	err = par & 1;
	*pa = par & ((1ULL << 48) - 1);
	*pa &= ~((1ULL << 12) - 1);
	return err;
}

static void show_map(struct test_map *map)
{
	printf("va=%lx ipa=%lx pa=%lx prot=%x.%x size=%llx\n",
		map->va, map->ipa, map->pa,
		map->prot, map->prot2, map->size);
}


static void rand_test_map(struct crt_runner *r,
			unsigned int *seedp, struct test_map *tm, uintptr_t physbits,
			uintptr_t min_pa)
{
	struct tester_map *tmap = crt_runner_get_user(r);
	uintptr_t physsize = 1ULL << physbits;
	uintptr_t mask;

	crt_objs(r, tmap->crt_map, tm);

	mask = ~(tm->size - 1);
	mask = (physsize - 1) & mask;

	tm->va &= mask;
	tm->ipa &= mask;
	tm->pa &= mask;
	if (tm->ipa < min_pa)
		tm->ipa |= 1ULL << 30;
	if (tm->pa < min_pa)
		tm->pa |= 1ULL << 30;

	/* write only not supported in EL1.  */
	if (tm->prot & PROT_WRITE)
		tm->prot |= PROT_READ;
}

static void check_map(struct crt_runner *r, struct mmu_ctx *mmu, struct test_map *map)
{
	unsigned int el = 1;
	uintptr_t pa;
	uintptr_t pt_root = (uintptr_t) mmu[0].pt.root;
	bool err;
	uintptr_t smmu_pa;
	bool smmu_err;
	aarch64_mmu_pt_zap(&mmu[0], el);
	aarch64_mmu_pt_zap(&mmu[1], el);
	aarch64_mmu_map(&mmu[0], map->va, map->ipa, map->size, map->prot,
			MAIR_IDX_MEM, MAP_NONSECURE);
	aarch64_mmu_map(&mmu[1], map->ipa, map->pa, map->size, map->prot2,
			MAIR_IDX_MEM, MAP_NONSECURE);

	/* Need this one for the page table access.  */
	aarch64_mmu_map(&mmu[1], pt_root, pt_root,
			MAPSIZE_2M, PROT_READ, MAIR_IDX_MEM, MAP_NONSECURE);
	aarch64_mmu_tlb_flush(&mmu[0], el);
	aarch64_mmu_tlb_flush(&mmu[1], el);

	err = map_cpu(el, S1, 0, map->va, &pa);
	if (HAVE_SMMU) {
		smmu_err = map_smmu(S1, 0, map->va, &smmu_pa);
		assert(err == smmu_err);
		if (!err) {
			if (pa != smmu_pa) {
				printf("ipa=%lx pa=%lx smmu_pa=%lx\n", map->ipa, pa, smmu_pa);
			}
			assert(pa == smmu_pa);
		}
	}

	if (map->prot & PROT_READ) {
		if (err) dump_par();
		CRT_FAIL_GOTO_ON(r, err, fail);
		CRT_FAIL_GOTO_ON(r, pa != map->ipa, fail);
		assert(err == 0);
		assert(pa == map->ipa);
	} else {
		if (!(map->prot & PROT_WRITE) && !err) {
			dump_par();
			CRT_FAIL_GOTO_ON(r, !err, fail);
			assert(err);
		}
	}

	err = map_cpu(el, S12, 0, map->va, &pa);
	if (HAVE_SMMU) {
		smmu_err = map_smmu(S12, 0, map->va, &smmu_pa);
		assert(err == smmu_err);
		if (!err) {
			if (pa != smmu_pa) {
				printf("ipa=%lx pa=%lx smmu_pa=%lx\n", map->ipa, pa, smmu_pa);
			}
			assert(pa == smmu_pa);
		}
	}

	if (map->prot & map->prot2 & PROT_READ) {
		if (err) dump_par();
		CRT_FAIL_GOTO_ON(r, err, fail);
		CRT_FAIL_GOTO_ON(r, pa != map->pa, fail);
		assert(err == 0);
		assert(pa == map->pa);
	} else {
		CRT_FAIL_GOTO_ON(r, !err, fail);
		assert(err);
	}

	err = map_cpu(el, S1, true, map->va, &pa);
	if (HAVE_SMMU) {
		smmu_err = map_smmu(S1, true, map->va, &smmu_pa);
		assert(err == smmu_err);
		if (!err) {
			if (pa != smmu_pa) {
				printf("ipa=%lx pa=%lx smmu_pa=%lx\n", map->ipa, pa, smmu_pa);
			}
			assert(pa == smmu_pa);
		}
	}

	if (map->prot & PROT_WRITE) {
		if (err) dump_par();
		CRT_FAIL_GOTO_ON(r, err, fail);
		CRT_FAIL_GOTO_ON(r, pa != map->ipa, fail);
		assert(err == 0);
		assert(pa == map->ipa);
	} else {
		CRT_FAIL_GOTO_ON(r, !err, fail);
		assert(err);
	}

	err = map_cpu(el, S12, true, map->va, &pa);
	if (HAVE_SMMU) {
		smmu_err = map_smmu(S12, true, map->va, &smmu_pa);
		assert(err == smmu_err);
		if (!err) {
			if (pa != smmu_pa) {
				printf("ipa=%lx pa=%lx smmu_pa=%lx\n", map->ipa, pa, smmu_pa);
			}
			assert(pa == smmu_pa);
		}
	}

	if (map->prot & map->prot2 & PROT_WRITE) {
		if (err) dump_par();
		CRT_FAIL_GOTO_ON(r, err, fail);
		CRT_FAIL_GOTO_ON(r, pa != map->pa, fail);
		assert(err == 0);
		assert(pa == map->pa);
	} else {
		CRT_FAIL_GOTO_ON(r, !err, fail);
		assert(err);
	}
	return;
fail:
	dump_par();
	printf("pa=%lx\n", pa);
	show_map(map);
}

static void prep_smmu_1(struct crt_runner *r, struct tester_map *tmap)
{
	smmu_init_ctx(&tmap->mmu[0], &tmap->mmu[1], 0, 1, 1, true);
	D(printf("SMMU: S12MMU enabled\n"));
}

#if 0
static void prep_smmu(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = user;

	aarch64_mmu_pt_setup(&tmap->mmu[0], PAGEKIND_TINY_4K, 1,
			40, 40,
			&_pt_s2_start,
			&_pt_s2_start + 0x40000);
	aarch64_mmu_pt_setup(&tmap->mmu[1], PAGEKIND_TINY_4K, 2,
			39, 40,
			&_pt_s2_start + 0x40000,
			&_pt_s2_start + 0x80000);
	prep_smmu_1(r, tmap);
}
#endif

static void prep_mmu(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = user;
	uint64_t hcr;
	uint32_t scr;

	aarch64_mmu_pt_setup(&tmap->mmu[0], PAGEKIND_TINY_4K, 1,
			40, 40,
			&_pt_s2_start,
			&_pt_s2_start + 0x40000);
	aarch64_mmu_pt_setup(&tmap->mmu[1], PAGEKIND_TINY_4K, 2,
			39, 40,
			&_pt_s2_start + 0x40000,
			&_pt_s2_start + 0x80000);
	aarch64_mmu_setup(&tmap->mmu[0], 1, true);
	aarch64_mmu_setup(&tmap->mmu[1], 1, true);
	D(printf("S12MMU enabled\n"));

	aarch64_mrs(scr, "scr_el3");
	scr |= SCR_RW | SCR_NS;
	aarch64_msr("scr_el3", scr);

	aarch64_mrs(hcr, "hcr_el2");
	hcr |= HCR_RW | HCR_VM;
	aarch64_msr("hcr_el2", hcr);

	if (HAVE_SMMU) {
		/* Setup the SMMU.  */
		prep_smmu_1(r, tmap);
	}
}

void teardown_map(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = user;
	aarch64_mmu_setup(&tmap->mmu[0], 1, false);
	aarch64_mmu_setup(&tmap->mmu[1], 1, false);
	D(printf("S2MMU disabled\n"));
}

static void crt_map_cb(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = user;
	struct test_map map;

	prep_mmu(r, user);

//	rand_test_map(r, &r->rand.seed, &map, 39, ((uintptr_t) tmap->mmu[1].pt.root) + MAPSIZE_2M);
	rand_test_map(r, &r->rand.seed, &map, 39,
		((uintptr_t) tmap->mmu[1].pt.root) + MAPSIZE_2M);
	check_map(r, &tmap->mmu[0], &map);

	teardown_map(r, user);
}

static void crt_map_direct1_cb(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = user;
	struct test_map map = {
		.va = 0x41ULL << 30,
		.ipa = 0x41ULL << 30,
		.pa = 0x42ULL << 30,
		.size = MAPSIZE_1G,
		.prot = PROT_RW,
		.prot2 = PROT_RW,
	};
	show_map(&map);
	check_map(r, &tmap->mmu[0], &map);
}

static unsigned int completeness(struct crt_runner *r, void *user)
{
	struct tester_map *tmap = crt_runner_get_user(r);
	unsigned int c;
	c = crt_completeness_objs(r, tmap->crt_map);
	return c;
}
static struct crt_runner runner = {
	.name = "ARM MMU",
	.prep_cb = prep_mmu,
	.cb = crt_map_cb,
	.teardown_cb = teardown_map,
	.completeness_cb = completeness,
	.user = &tmap,
};
void check_mmu(void)
{
	printf("%s\n", __func__);
	crt_runner_init(&runner);
	crt_run_one_func(&runner, crt_map_direct1_cb);

	crt_runner_set_seet(&runner, 0x3043a06f);
	crt_run_one(&runner);
}
