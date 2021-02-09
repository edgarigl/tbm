/*
 * Small QSPI driver for the ZynqMP.
 *
 * Copyright (C) 2020 Xilinx Inc
 * Written by Edgar E. Iglesias
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "sys.h"
#include "qspi.h"

#include "drivers/zynqmp/reg-qspi.h"

void gqspi_write(struct qspi_ctx *ctx, uint8_t v)
{
	/* Transfer something.  */
	writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
			| 0 << 19			/* Poll */
			| ctx->stripe << 18		/* Stripe */
			| 0 << 17			/* RX */
			| 1 << 16			/* TX */
			| ctx->data_bus_select << 14	/* Data bus select */
			| ctx->upper_cs << 13		/* UCS.  */
			| ctx->lower_cs << 12		/* LCS.  */
			| ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
			| 0 << 9			/* Exponent  */
			| 0 << 8			/* Data Xfer.  */
			| v);
	writel(ctx->base + R_QSPI_GQSPI_CFG,
		1 << QSPI_GQSPI_CFG_MODE_EN_SHIFT
		|| 1 << QSPI_GQSPI_CFG_START_GEN_FIFO_SHIFT
		);
}

uint32_t gqspi_read(struct qspi_ctx *ctx)
{
	uint32_t r;
	bool empty;

	writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
			| 0 << 19  /* Poll */
			| ctx->stripe << 18  /* Stripe */
			| 1 << 17  /* RX */
			| 0 << 16  /* TX */
			| ctx->data_bus_select << 14  /* Data bus select */
			| ctx->upper_cs << 13  /* UCS.  */
			| ctx->lower_cs << 12  /* LCS.  */
			| ctx->mode << 10  /* Dual SPI.  */
			| 0 << 9   /* Exponent  */
			| 0 << 8   /* Data Xfer.  */
			| 0x00);

	writel(ctx->base + R_QSPI_GQSPI_CFG,
		1 << QSPI_GQSPI_CFG_MODE_EN_SHIFT
		|| 1 << QSPI_GQSPI_CFG_START_GEN_FIFO_SHIFT
		);

	do {
		r = readl(ctx->base + R_QSPI_GQSPI_ISR);
		empty = r & (1 << QSPI_GQSPI_ISR_RX_FIFO_EMPTY_SHIFT);
	} while (empty);

	r = readl(ctx->base + R_QSPI_GQSPI_RXD);
	return r;
}

void gqspi_prepare_cs(struct qspi_ctx *ctx, bool cs)
{
	if (ctx->lqspi_enable) {
		uint32_t r;

		r = readl(ctx->base + R_QSPI_CONFIG_REG);
		r |= !ctx->lower_cs << QSPI_CONFIG_REG_PCS_SHIFT;
		r |= !ctx->upper_cs << (QSPI_CONFIG_REG_PCS_SHIFT + 1);
		writel(ctx->base + R_QSPI_CONFIG_REG, r);

		writel(ctx->base + R_QSPI_LQSPI_CFG, 0
			| 1 << QSPI_LQSPI_CFG_LQ_MODE_SHIFT
			| 0x03);
	}

	writel(ctx->base + R_QSPI_GQSPI_CFG,
		0 << QSPI_GQSPI_CFG_MODE_EN_SHIFT
		| 1 << QSPI_GQSPI_CFG_START_GEN_FIFO_SHIFT
		);

	writel(ctx->base + R_QSPI_GQSPI_EN_REG, 1);
	writel(ctx->base + R_QSPI_GQSPI_SEL, 1);

	writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
			| 0 << 19  /* Poll */
			| 0 << 18  /* Stripe */
			| 0 << 17  /* RX */
			| 0 << 16  /* TX */
			| ctx->data_bus_select << 14  /* Data bus select */
			| (ctx->upper_cs & cs) << 13  /* UCS.  */
			| (ctx->lower_cs * cs) << 12  /* LCS.  */
			| ctx->mode << 10  /* Dual SPI.  */
			| 0 << 9   /* Exponent  */
			| 0 << 8   /* Data Xfer.  */
			| 0x02);
}
