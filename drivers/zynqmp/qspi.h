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

struct qspi_ctx {
	/* data bus select
	 * 00 - No bus
	 * 01 - Lower bus select
	 * 10 - Upper bus select
	 * 11 - Both bus select
	 */
	unsigned int data_bus_select;

	/* Upper and lower CS.  */
	bool upper_cs;
	bool lower_cs;

	/* Mode
	 * 00 - Reserved.
	 * 01 - SPI
	 * 10 - Dual SPI
	 * 11 - Quad SPI
	 */
	unsigned int mode;
	bool stripe;

	bool lqspi_enable;

	/* Base address of controller.  */
	phys_addr_t base;
};

void gqspi_write(struct qspi_ctx *ctx, uint8_t v);
uint32_t gqspi_read(struct qspi_ctx *ctx);
void gqspi_prepare_cs(struct qspi_ctx *ctx, bool cs);

