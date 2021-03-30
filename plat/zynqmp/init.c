/*
 * Bare-metal Xilinx ZynqMPSoc setup.
 * Copyright (C) 2015 Edgar E. Iglesias.
 */
#define _MINIC_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sys.h"
#include "plat.h"
#include "mmu.h"
#include "cache.h"
#include "reg-crf.h"
#include "reg-crl.h"
#include "reg-ddrc.h"
#include "reg-fpd_slcr.h"
#include "ronaldo-map.dtsh"
#include "ronaldo-version.h"
#ifdef HAVE_FDT
#include "devtree.h"
#endif
#include "reg-gic.h"

#define D(x)

extern char _dtb;
struct alloc_areas plat_mem;
static struct alloc_ctx ddr_ctx;
static struct alloc_ctx ocm_ctx;

static const char *platform_names[] = {
	[RDO_SILICON] = "Silicon",
	[RDO_REMUS] = "REMUS",
	[RDO_VELOCE] = "Veloce",
	[RDO_QEMU] = "QEMU"
};

extern char _heap_start;
__attribute__((weak)) char _heap_ddr_start;
__attribute__((weak)) char _heap_ddr_end;
__attribute__((weak)) char _heap_ocm_start;
__attribute__((weak)) char _heap_ocm_end;

static struct alloc_ctx * plat_memheap(const char *name,
					struct alloc_ctx *new,
					char *start, char *end)
{
	bool def;
	uintptr_t size_K;

	/* To small.  */
	if (end <= start + 32)
		return NULL;

	def = &_heap_start == start;
	size_K = end - start;
	size_K >>= 10;
	printf("%s: %s (%s) %p - %p %ldKB\n", __func__, name, def ? "def" : "new", start, end, size_K);
	/* This is the default HEAP.  */
	if (def) {
		return _alloc_get_default_ctx();
	}

	_alloc_ctx_init(new, start, end - start);
	return new;
}

#ifdef __aarch64__
static struct {
	struct aarch64_cache_descr caches;
	unsigned int freq;
} rdo;

__attribute__ ((weak)) char _pt_start;
__attribute__ ((weak)) char _pt_end;

static struct mmu_ctx mmu;

static void plat_setup_mmu(unsigned int el)
{
	uint64_t addr;

	if (&_pt_start + 4 >= &_pt_end) {
		printf("WARN: No space for MMU tables.\n");
		return;
	}

	aarch64_mmu_pt_setup(&mmu, PAGEKIND_TINY_4K, 1,
				40, 40,
				&_pt_start,
				&_pt_end);

	/* Create lower 32bit identity map.  */
	aarch64_mmu_map(&mmu, 0, 0, MAPSIZE_1G, PROT_RWE, MAIR_IDX_MEM, MAP_SECURE);
	aarch64_mmu_map(&mmu, 1ULL << 30, 1ULL << 30, MAPSIZE_1G, PROT_RWE,
			MAIR_IDX_MEM, MAP_SECURE);
	aarch64_mmu_map(&mmu, 2ULL << 30, 2ULL << 30, MAPSIZE_1G, PROT_RW,
			MAIR_IDX_DEVICE, MAP_SECURE);

	/* Map cosim RAM.  */
	aarch64_mmu_map(&mmu, 0xa0460000, 0xa0460000, MAPSIZE_2M, PROT_RWE,
			MAIR_IDX_DEVICE, MAP_SECURE);

	/* DEVICE area.  */
	for (addr = 0xC0000000; addr < 0xFFC00000; addr += MAPSIZE_2M) {
//		printf("Mapping DEVICE %llx - %llx\n", addr, addr + MAPSIZE_2M);
		aarch64_mmu_map(&mmu, addr, addr, MAPSIZE_2M, PROT_RWE,
				MAIR_IDX_DEVICE, MAP_SECURE);
	}
	/* OCM.  */
	for (addr = 0xFFC00000; addr < 0x100000000ULL; addr += MAPSIZE_2M) {
		printf("Mapping OCM %llx - %llx\n", addr, addr + MAPSIZE_2M);
		aarch64_mmu_map(&mmu, addr, addr, MAPSIZE_2M, PROT_RWE,
				MAIR_IDX_MEM, MAP_SECURE);
	}
	aarch64_mmu_map(&mmu, 4ULL << 30, 4ULL << 30, MAPSIZE_1G, 0,
			MAIR_IDX_DEVICE, MAP_SECURE);

	/* Invalidate caches before enabling them.  */
	a64_icache_invalidate(&rdo.caches);
	a64_dcache_invalidate(&rdo.caches, false);
	/*aarch64_enable_caches(el, true, true);*/
	/*printf("EL%d I and D caches enabled\n", el);*/

	printf("RDO: enable MMU\n");
	aarch64_mmu_setup(&mmu, el, true);
	printf("RDO: Still here? EL%d S1 MMU is on\n", el);
}
#endif

#ifdef __aarch64__
void plat_setup_freq(void)
{
	if (aarch64_current_el() != 3)
		return;

	if (rdo.freq) {
		aarch64_msr("cntfrq_el0", rdo.freq);
	}
}

void plat_cache_invalidate(void)
{
	a64_icache_invalidate(&rdo.caches);
	a64_dcache_invalidate(&rdo.caches, false);
	ibarrier();
	mb();
	/* Clean state.  */
}

void plat_cache_flush(void)
{
	a64_icache_invalidate(&rdo.caches);
	a64_dcache_invalidate(&rdo.caches, true);
	ibarrier();
	mb();

	/* Clean state.  */
}

void plat_init_a64(void)
{
	const unsigned int current_el = aarch64_current_el();

	printf("%s\n", __func__);
	plat_setup_mmu(current_el);
}

#define R_L2CTRL_EL1 "S3_1_C11_C0_2"
#define R_CPUACTLR_EL1 "S3_1_C15_C2_0"
#define R_CPUECTLR_EL1 "S3_1_C15_C2_1"
#define CPUECTLR_SMP (0x40)

void cortexa53_init(void)
{
	const unsigned int current_el = aarch64_current_el();
	uint64_t r;

	if (current_el != 3)
		return;

	aarch64_mrs(r, "sctlr_el1");
	r |= CPUECTLR_SMP;
	aarch64_msr("sctlr_el1", r);
	aarch64_mrs(r, "sctlr_el1");
	ibarrier();
	D(printf("Updated SCTLR_EL1=%llx\n", r));

	aarch64_mrs(r, R_CPUECTLR_EL1);
	r |= CPUECTLR_SMP;
	aarch64_msr(R_CPUECTLR_EL1, r);
	aarch64_mrs(r, R_CPUECTLR_EL1);
	ibarrier();
	D(printf("Updated CPUECTLR_EL1=%llx\n", r));
}
#endif

static void rdo_wait_for_ddrc(void)
{
	uint32_t r;
	unsigned int i;
	bool ok = false;

	for (i = 0; i < 10000; i++) {
		r = readl(DDRC_STAT);
		if ((r & DDRC_STAT_OPERATING_MODE_MASK) == 1) {
			ok = true;
			break;
		}
	}
	ok = ok;
	D(printf("DDRC running=%d r=%x\n", ok, r));
}

static void release_timestamp_cnt(struct ronaldo_version *v)
{
	uint32_t r;
	unsigned int freq = 0;

	r = readl(CRL_APB_RST_LPD_IOU2);
	r &= ~(CRL_APB_RST_LPD_IOU2_TIMESTAMP_RESET_MASK);
	writel(CRL_APB_RST_LPD_IOU2, r);
	mb();
	r = readl(CRL_APB_RST_LPD_IOU2);
	D(printf("CRL_APB_RST_LPD_IOU2=%x\n", r));

	switch (v->platform) {
	case RDO_REMUS:
		freq = 4000000;
		break;
	case RDO_QEMU:
		freq = 50000000;
		break;
	default:
	case RDO_SILICON:
		freq = 100000000;
		break;
	}

	printf("Setup freq %dhz\n", freq);
	/* Set the freq. These regs are wrong, so use literal addresses.  */
	writel(0xFF260020, freq);
	mb();
	writel(0xFF260000, 0x1);
	mb();

#ifdef __aarch64__
	if (aarch64_current_el() != 3)
		return;

	rdo.freq = freq;
	aarch64_msr("cntfrq_el0", rdo.freq);
#endif
}

static void release_ttc(void)
{
	uint32_t r;

	r = readl(CRL_APB_RST_LPD_IOU2);
	r &= ~(CRL_APB_RST_LPD_IOU2_TTC0_RESET_MASK
		| CRL_APB_RST_LPD_IOU2_TTC1_RESET_MASK
		| CRL_APB_RST_LPD_IOU2_TTC2_RESET_MASK
		| CRL_APB_RST_LPD_IOU2_TTC3_RESET_MASK
		| CRL_APB_RST_LPD_IOU2_TIMESTAMP_RESET_MASK);
	writel(CRL_APB_RST_LPD_IOU2, r);
	mb();
	r = readl(CRL_APB_RST_LPD_IOU2);
	D(printf("CRL_APB_RST_LPD_IOU2=%x\n", r));
}

void gic_sec_setup(void)
{
        writel(GIC_DIST_BASE, 3);
        writel(GIC_DIST_BASE + GICD_IGROUPR, ~0);
        writel(GIC_DIST_BASE + GICD_IGROUPR + 4, ~0);
        writel(GIC_DIST_BASE + GICD_IGROUPR + 8, ~0);
        writel(GIC_DIST_BASE + GICD_IGROUPR + 12, ~0);
        writel(GIC_DIST_BASE + GICD_IGROUPR + 16, ~0);
        writel(GIC_DIST_BASE + GICD_IGROUPR + 20, ~0);
        writel(GIC_CPU_BASE, 3);
        writel(GIC_CPU_BASE + 4, 1 << 7);
}

void plat_init_secondaries(void)
{
#ifdef __aarch64__
	plat_cache_invalidate();
	cortexa53_init();
	plat_setup_freq();

	writel(GIC_DIST_BASE + GICD_IGROUPR, ~0);
	mb();
	writel(GIC_CPU_BASE, 3);
	writel(GIC_CPU_BASE + 4, 1 << 7);
	mb();
#endif
}

void plat_init(void)
{
	struct ronaldo_version v;

	v = ronaldo_version();
	printf("Ronaldo Platform %x %s v%x RTLv%d.%d PSv=%x\n",
		v.platform, platform_names[v.platform & 3], v.platform_version,
		v.rtl_version >> 4, v.rtl_version & 0xf,
		v.ps_version);

	rdo_wait_for_ddrc();
#ifdef HAVE_FDT
	printf("\nFDT: scan\n");
	devtree_setup(&_dtb);
	printf("FDT: done\n\n");
#endif

	D(printf("RST_FPD_TOP=%x\n", readl(CRF_APB_RST_FPD_TOP)));
	D(printf("RST_FPD_APU=%x\n", readl(CRF_APB_RST_FPD_APU)));
	D(printf("RST_FPD_DDR_SS=%x\n", readl(CRF_APB_RST_DDR_SS)));

#ifdef __aarch64__
	a64_cache_discover(&rdo.caches);
	cortexa53_init();
	plat_init_a64();
	if (aarch64_current_el() == 3)
		gic_sec_setup();

	/* Catch Serrors.  */
	local_cpu_serror_ei();
#endif
	release_ttc();
	release_timestamp_cnt(&v);

	plat_mem.ocm = plat_memheap("OCM", &ocm_ctx, &_heap_ocm_start, &_heap_ocm_end);
	plat_mem.ddr = plat_memheap("DDR", &ddr_ctx, &_heap_ddr_start, &_heap_ddr_end);

	putchar('\n');
}
