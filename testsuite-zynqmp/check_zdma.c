/*
 * Smoke testsuite for the Ronaldo zDMA.
 *
 * Copyright (C) 2014 Xilinx Inc
 * Written by Edgar E. Iglesias
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <byteswap.h>

#include "sys.h"
#include "plat.h"
#include "drivers/zynqmp/zdma.h"

#undef D
#define D(x) x

/* a local allocation context to get hold of buffers in DDR.
   BSS and DATA might be on CSU priv memories.  */
struct alloc_ctx *ddr_area;

/* Simple test case for DMA transfer with linked descriptors.  */
static void check_zdma_linked(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 100;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	/* We allocate 8 descriptors, but only use three, 0, 1 and 7.  */
	dsc_src = zdma_descr_alloc(ddr_area, 8, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 8, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 1, 0, false, true, false);
	zdma_descr_init(&dsc_src[1], &dsc_src[7], 0, 0, false, false, false);
	zdma_descr_init(&dsc_src[7], src + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 1, 0, false, true, false);
	zdma_descr_init(&dsc_dst[1], &dsc_dst[7], 0, 0, false, false, false);
	zdma_descr_init(&dsc_dst[7], dst + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2,
		ZDMA_ZDMA_CH_CTRL2_EN_MASK);
	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

/* Check if we can leave from pause to disable.  */
static void check_zdma_linear2_pause3(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 100;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 4, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 4, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 1,
			ZDMA_ATTR_CMD_HALT, false, false, false);
	zdma_descr_init(&dsc_src[1], 0, 0,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_src[3], src + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 1, 0, false, false, false);
	zdma_descr_init(&dsc_dst[3], dst + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_PAUSED);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB,
			(uintptr_t) &dsc_src[3]);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB,
			(uintptr_t) &dsc_dst[3]);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, 0);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_ZDMA_CH_CTRL0_CONT_MASK
		| 1 << ZDMA_ZDMA_CH_CTRL0_CONT_ADDR_SHIFT
		| ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size - 1)) {
		err();
	}

	if (dst[size - 1] != 0) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size - 1)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

/* Check if CONT works with descriptor address in START regs.  */
static void check_zdma_linear2_pause2(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 100;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 4, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 4, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 1,
			ZDMA_ATTR_CMD_HALT, false, false, false);
	zdma_descr_init(&dsc_src[1], 0, 0,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_src[3], src + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 1, 0, false, false, false);
	zdma_descr_init(&dsc_dst[3], dst + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_PAUSED);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB,
			(uintptr_t) &dsc_src[3]);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB,
			(uintptr_t) &dsc_dst[3]);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_ZDMA_CH_CTRL0_CONT_MASK
		| 1 << ZDMA_ZDMA_CH_CTRL0_CONT_ADDR_SHIFT
		| ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

/* Check if CONT works with descriptor address computed from previous descr.  */
static void check_zdma_linear2_pause(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 256;
	uint32_t total_trans;
	int diff, diff2;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 2, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 2, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 1,
			ZDMA_ATTR_CMD_HALT, false, false, false);
	zdma_descr_init(&dsc_src[1], src + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 1, 0, false, false, false);
	zdma_descr_init(&dsc_dst[1], dst + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	mb();
	plat_cache_flush();

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	writel(ch_base + ZDMA_ZDMA_CH_DATA_ATTR,
		0x1 << ZDMA_ZDMA_CH_DATA_ATTR_ARBURST_SHIFT
		| 0xf << ZDMA_ZDMA_CH_DATA_ATTR_ARLEN_SHIFT
		| 0x1 << ZDMA_ZDMA_CH_DATA_ATTR_AWBURST_SHIFT
		| 0xf << ZDMA_ZDMA_CH_DATA_ATTR_AWLEN_SHIFT);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);
	mb();

	zdma_wait_for(base, ch, ZDMA_STATE_PAUSED);
	mb();

	diff = memcmp(src, dst, size - 1);
	if (diff) {
		udelay(1);
#if 0
		hexdump("src", src, size - 1);
		hexdump("dst", dst, size - 1);
#endif
		/* Pause state may not involve data is visible??  */
		diff2 = memcmp(src, dst, size - 1);
		if (diff2) {
			printf("diffs %d %d\n", diff, diff2);
			err();
		}
	}

	if (dst[size - 1] != 0) {
		hexdump("dst", dst, size);
		err();
	}

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_ZDMA_CH_CTRL0_CONT_MASK
		| 0 << ZDMA_ZDMA_CH_CTRL0_CONT_ADDR_SHIFT
		| ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	mb();

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		hexdump("src", src, size);
		hexdump("dst", dst, size);
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_linear_irq(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	uint32_t total_trans;
	unsigned int i;
	static const unsigned int nr_desc = 100;
	unsigned int size = nr_desc;
	unsigned int src_count, dst_count;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, nr_desc, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, nr_desc, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	for (i = 0; i < nr_desc - 1; i++) {
		zdma_descr_init(&dsc_src[i], src + i, 1, 0,
				true, false, false);
		zdma_descr_init(&dsc_dst[i], dst + i, 1, 0,
				true, false, false);
	}
	zdma_descr_init(&dsc_src[i], src + i, 1,
			ZDMA_ATTR_CMD_STOP, true, false, false);

	zdma_descr_init(&dsc_dst[i], dst + i, 1,
			ZDMA_ATTR_CMD_STOP, true, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);
	src_count = readl(ch_base + ZDMA_ZDMA_CH_IRQ_SRC_ACCT);
	dst_count = readl(ch_base + ZDMA_ZDMA_CH_IRQ_DST_ACCT);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);
	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	src_count = readl(ch_base + ZDMA_ZDMA_CH_IRQ_SRC_ACCT);
	dst_count = readl(ch_base + ZDMA_ZDMA_CH_IRQ_DST_ACCT);

	assert(src_count == nr_desc);
	assert(dst_count == nr_desc);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_linear2_asym(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 128;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 3, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 6, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 32, 0, false, false, false);
	zdma_descr_init(&dsc_src[1], src + size - 32, 16, 0, false, false, false);
	zdma_descr_init(&dsc_src[2], src + size - 16, 16,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 20, 0, false, false, false);
	zdma_descr_init(&dsc_dst[1], dst + size - 20, 4, 0, false, false, false);
	zdma_descr_init(&dsc_dst[2], dst + size - 16, 4, 0, false, false, false);
	zdma_descr_init(&dsc_dst[3], dst + size - 12, 4, 0, false, false, false);
	zdma_descr_init(&dsc_dst[4], dst + size - 8, 4, 0, false, false, false);
	zdma_descr_init(&dsc_dst[5], dst + size - 4, 4,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	writel(ch_base + ZDMA_ZDMA_CH_DATA_ATTR,
		0x1 << ZDMA_ZDMA_CH_DATA_ATTR_ARBURST_SHIFT
		| 0x1 << ZDMA_ZDMA_CH_DATA_ATTR_ARLEN_SHIFT
		| 0x1 << ZDMA_ZDMA_CH_DATA_ATTR_AWBURST_SHIFT
		| 0x1 << ZDMA_ZDMA_CH_DATA_ATTR_AWLEN_SHIFT);


	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	printf("Wait for done\n");
	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);
	printf("Done\n");

	if (memcmp(src, dst, size)) {
		hexdump("src", src, size);
		hexdump("dst", dst, size);
		printf("Error, Asymetric desctriptor setups do not work!!!\n");
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_linear2(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 100;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 2, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 2, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(&dsc_src[0], src, size - 1, 0, false, false, false);
	zdma_descr_init(&dsc_src[1], src + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	zdma_descr_init(&dsc_dst[0], dst, size - 1, 0, false, false, false);
	zdma_descr_init(&dsc_dst[1], dst + size - 1, 1,
			ZDMA_ATTR_CMD_STOP, false, false, false);

	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);
	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_linear(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	struct zdma_descr *dsc_src, *dsc_dst;
	char *dsc_src_m, *dsc_dst_m;
	unsigned int size = 100;
	uint32_t total_trans;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	dsc_src = zdma_descr_alloc(ddr_area, 1, &dsc_src_m);
	dsc_dst = zdma_descr_alloc(ddr_area, 1, &dsc_dst_m);

	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	zdma_descr_init(dsc_src, src, size, ZDMA_ATTR_CMD_STOP,
			false, false, false);
	zdma_descr_init(dsc_dst, dst, size, ZDMA_ATTR_CMD_STOP,
			false, false, false);
	total_trans = readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_START_LSB, (uintptr_t) dsc_src);
	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_START_LSB, (uintptr_t) dsc_dst);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_MEM << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != (total_trans + size)) {
//		err();
	}

	_free_ctx(ddr_area, dsc_src_m);
	_free_ctx(ddr_area, dsc_dst_m);
	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_simple_burst_rfixed(phys_addr_t base, unsigned int ch,
					   unsigned int bwidth)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = bwidth * 2;
	unsigned int i;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x33, bwidth);
	memset(src + bwidth, 0x12, size - bwidth);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);
	writel(ch_base + ZDMA_ZDMA_CH_DATA_ATTR,
		AXI_BURST_FIXED << ZDMA_ZDMA_CH_DATA_ATTR_ARBURST_SHIFT
		 | AXI_BURST_INCR << ZDMA_ZDMA_CH_DATA_ATTR_AWBURST_SHIFT
		);


	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, (uintptr_t) src);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, size);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	for (i = 0; i < size; i++) {
		if (dst[i] != 0x33) {
			hexdump("dst", dst, size);
			err();
		}
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_simple_burst_wo_wfixed(phys_addr_t base, unsigned int ch,
						unsigned int bwidth)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = bwidth * 2, i;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);
	writel(ch_base + ZDMA_ZDMA_CH_DATA_ATTR,
		AXI_BURST_INCR << ZDMA_ZDMA_CH_DATA_ATTR_ARBURST_SHIFT
		 | AXI_BURST_FIXED << ZDMA_ZDMA_CH_DATA_ATTR_AWBURST_SHIFT
		);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, (uintptr_t) src);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, size);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	for (i = 0; i < 4; i++) {
		writel(ch_base + ZDMA_ZDMA_CH_WR_ONLY_WORD0 + i * 4, 0x33333333);
	}

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT
		| ZDMA_MODE_WO << ZDMA_ZDMA_CH_CTRL0_MODE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	for (i = 0; i < bwidth; i++) {
		if (dst[i] != 0x33) {
			hexdump("dst", dst, size);
			err();
		}
	}

	for (i = bwidth; i < size; i++) {
		if (dst[i] != 0x0) {
			hexdump("dst", dst, size);
			err();
		}
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	writel(ch_base + ZDMA_ZDMA_CH_DATA_ATTR,
		AXI_BURST_INCR << ZDMA_ZDMA_CH_DATA_ATTR_ARBURST_SHIFT
		| AXI_BURST_INCR << ZDMA_ZDMA_CH_DATA_ATTR_AWBURST_SHIFT
		);

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_wo_bad_src(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = 100, i;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, 0xbad);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, 0x0);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	for (i = 0; i < 4; i++) {
		writel(ch_base + ZDMA_ZDMA_CH_WR_ONLY_WORD0 + i * 4, 0x33333333);
	}

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT
		| ZDMA_MODE_WO << ZDMA_ZDMA_CH_CTRL0_MODE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	for (i = 0; i < size; i++) {
		if (dst[i] != 0x33) {
			hexdump("dst", dst, size);
			err();
		}
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_simple_wo(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = 100, i;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, (uintptr_t) src);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, size);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	for (i = 0; i < 4; i++) {
		writel(ch_base + ZDMA_ZDMA_CH_WR_ONLY_WORD0 + i * 4, 0x33333333);
	}

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT
		| ZDMA_MODE_WO << ZDMA_ZDMA_CH_CTRL0_MODE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	for (i = 0; i < size; i++) {
		if (dst[i] != 0x33) {
			hexdump("dst", dst, size);
			err();
		}
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_simple_ro(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = 100, i;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, (uintptr_t) src);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, size);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT
		| ZDMA_MODE_RO << ZDMA_ZDMA_CH_CTRL0_MODE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	for (i = 0; i < size; i++) {
		if (dst[i] != 0) {
			err();
		}
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_simple(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	unsigned char *src, *dst;
	unsigned int size = 100;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	src = _malloc_ctx(ddr_area, size);
	dst = _malloc_ctx(ddr_area, size);
	memset(src, 0x12, size);
	memset(dst, 0, size);

	writel(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE, 0xFFFFFFFF);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD0, (uintptr_t) src);
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, size);

	zdma_write64(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD0, (uintptr_t) dst);
	writel(ch_base + ZDMA_ZDMA_CH_DST_DSCR_WORD2, size);

	writel(ch_base + ZDMA_ZDMA_CH_CTRL0,
		ZDMA_PT_REG << ZDMA_ZDMA_CH_CTRL0_POINT_TYPE_SHIFT);
	writel(ch_base + ZDMA_ZDMA_CH_CTRL2, ZDMA_ZDMA_CH_CTRL2_EN_MASK);

	zdma_wait_for(base, ch, ZDMA_STATE_DISABLED);

	if (memcmp(src, dst, size)) {
		err();
	}

	if (readl(ch_base + ZDMA_ZDMA_CH_TOTAL_BYTE) != size) {
//		err();
	}

	_free_ctx(ddr_area, src);
	_free_ctx(ddr_area, dst);
}

static void check_zdma_inv_apb(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	uint32_t r, errctrl;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	errctrl = readl(ch_base + ZDMA_ZDMA_ERR_CTRL);
	writel(ch_base + ZDMA_ZDMA_ERR_CTRL, 0);
	writel(ch_base + ZDMA_ZDMA_CH_ISR, ZDMA_ZDMA_CH_ISR_INV_APB_MASK);
	writel(ch_base + 0x20, 1);
	r = readl(ch_base + 0x20);
	assert(r == 0);
	r = readl(ch_base + ZDMA_ZDMA_CH_ISR);
	assert(r & ZDMA_ZDMA_CH_ISR_INV_APB_MASK);
	mb();
	writel(ch_base + ZDMA_ZDMA_ERR_CTRL, errctrl);
}

static void check_zdma_reset(phys_addr_t base, unsigned int ch)
{
	phys_addr_t ch_base = zdma_base(base, ch);
	uint32_t r;

	/* The reset functionality has been removed.  */
	return;

	D(printf("%s: base=%lx ch=%d\n", __func__, base, ch));
	writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, 1);
	r = readl(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2);
	assert(r == 1);

	zdma_reset(base, ch);

	/* Did the reg reset to its default value?  */
	r = readl(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2);
	assert(r == 0);
}

#ifdef __versal__
#include "drivers/versal/versal-lpd-memmap.dtsh"
#endif

void check_zdma(void)
{
	int i, ch;
	static const struct {
		char *name;
		phys_addr_t base;
		unsigned int channels;
		unsigned int bwidth;
	} dma_units[] = {
#ifdef __versal__
		{ "ADMA", (phys_addr_t) MM_ADMA_CH0, 8, 16 },
#else
		{ "ADMA", (phys_addr_t) ADMA_BASEADDR, 8, 8 },
		{ "GDMA", (phys_addr_t) GDMA_BASEADDR, 8, 16 },
#endif
	};
	bool is_qemu = true;
#ifndef __versal__
	struct ronaldo_version v = ronaldo_version();

	if (v.platform != RDO_QEMU) {
		is_qemu = false;
	}
#endif

	if (!plat_mem.ddr) {
		printf("%s: Cannot run zDMA tests without DDR memory\n", __func__);
		return;
	}
	assert(plat_mem.ddr);
	ddr_area = plat_mem.ddr;

	for (i = 0; i < ARRAY_SIZE(dma_units); i++) {
		for (ch = 0; ch < dma_units[i].channels; ch++) {
			printf("%s: %s channel base=%lx %d\n",
				__func__, dma_units[i].name,
				dma_units[i].base, ch);
			check_zdma_reset(dma_units[i].base, ch);
			check_zdma_inv_apb(dma_units[i].base, ch);
			check_zdma_simple(dma_units[i].base, ch);
			check_zdma_simple_ro(dma_units[i].base, ch);
			check_zdma_simple_wo(dma_units[i].base, ch);
			check_zdma_wo_bad_src(dma_units[i].base, ch);
			if (is_qemu) {
				/* Real HW does not support fixed burst on RAM?  */
				check_zdma_simple_burst_rfixed(dma_units[i].base, ch,
								dma_units[i].bwidth);
				check_zdma_simple_burst_wo_wfixed(dma_units[i].base, ch,
								dma_units[i].bwidth);
			}
			if (is_qemu) {
				/* FIXME: Investigate why these don't work on real HW!  */
				check_zdma_linear(dma_units[i].base, ch);
				check_zdma_linear_irq(dma_units[i].base, ch);
				check_zdma_linear2(dma_units[i].base, ch);
				check_zdma_linear2_pause(dma_units[i].base, ch);
				check_zdma_linear2_pause2(dma_units[i].base, ch);
				check_zdma_linear2_pause3(dma_units[i].base, ch);
				check_zdma_linear2_asym(dma_units[i].base, ch);
				check_zdma_linked(dma_units[i].base, ch);
			}
		}
	}
}
