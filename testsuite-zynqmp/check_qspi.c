/*
 * Smoke testsuite for QSPI.
 *
 * Copyright (C) 2018 Xilinx Inc
 * Written by Francisco Iglesias
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

#include <stdio.h>
#include <stdint.h>
#include "sys.h"
#include "drivers/zynqmp/qspi.h"
#include "reg-qspi.h"

#define ENABLE_LOGS 0

#if ENABLE_LOGS

#define LOG0(fmt, ...) \
do { printf(" - " fmt , ## __VA_ARGS__); } while (0)

#define LOG1(fmt, ...) \
do { printf("   * " fmt , ## __VA_ARGS__); } while (0)

#define LOG2(fmt, ...) \
do { printf(fmt , ## __VA_ARGS__); } while (0)

#else
#define LOG0(fmt, ...)
#define LOG1(fmt, ...)
#define LOG2(fmt, ...)
#endif

#define err_msg(msg...)                         \
{                                               \
    printf(" [FAILED]\n         ------ " msg);  \
    err();                                      \
}

#define print_err(msg...)                       \
{                                               \
    printf(" [FAILED]\n         ------ " msg);  \
}

#define QSPI_BASE 0xFF0F0000
#define QSPI_LINEAR_BASE ((uint32_t*)0xC0000000)

#define IRQ(x) (1 << QSPI_GQSPI_IER_ ## x ## _SHIFT)
#define LQSPI_IRQ(x) (1 << QSPI_INTRPT_EN_REG_ ## x ## _SHIFT)

#define qspi_read_reg(reg) (readl(QSPI_BASE + (reg)))
#define qspi_write_reg(reg, val) (writel(QSPI_BASE + (reg), val))

#define print_reg(x)                                \
{                                                   \
    LOG1("[" #x "]: 0x%x\n", qspi_read_reg(x));     \
}

#define ARRAY_SZ(t) (sizeof(t)/sizeof(*t))

#define SPI_CMD(x, d) { .name = #x, .cmd = x, \
                        .dummy_bytes = d, .addr_length = 3 }
#define SPI_CMD4(x, d) { .name = #x, .cmd = x, \
                         .dummy_bytes = d, .addr_length = 4 }

#define dump_buf(buf)               \
{                                   \
    int i = 0;                      \
    printf("\n" #buf ": {");        \
    for (;i<sizeof(buf);i++)        \
        printf(" 0x%x,", buf[i]);   \
    printf("}\n");                  \
}

#define D(x)

/******************************************************************************/
enum {
    SPI_MODE_1 = 1, /* SPI, use one line */
    SPI_MODE_2 = 2, /* Dual SPI, use two lines */
    SPI_MODE_4 = 3  /* QUAD SPI, use four lines */
};

enum {
    NO_BUS = 1,
    LOWER_BUS = 1,
    UPPER_BUS = 2,
    BOTH_BUS  = 3
};

enum {
    CMD_PP    = 0x2,
    CMD_READ  = 0x3,

    CMD_DOR   = 0x3b,
    CMD_DOR4  = 0x3c,
    CMD_DIOR  = 0xbb,
    CMD_DIOR4 = 0xbc,

    CMD_QOR   = 0x6b,
    CMD_QOR4  = 0x6c,
    CMD_QIOR  = 0xeb,
    CMD_QIOR4 = 0xec,

    CMD_QPP   = 0x32,
    CMD_QPP4  = 0x34,

    CMD_CHIP_ERASE = 0x60,
    CMD_ERASE_4K   = 0x20,
    CMD_WR_EN      = 0x06,
    CMD_WR_DIS     = 0x04,
};

struct spi_cmd
{
    char *name;
    uint32_t cmd;
    uint32_t dummy_bytes;
    uint32_t addr_length;
};

static uint8_t ff[] =  { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static uint8_t def_buf[] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

/******************************************************************************/

void __chip_erase(struct qspi_ctx *ctx)
{
    gqspi_prepare_cs(ctx, true);
    gqspi_write(ctx, CMD_WR_EN);
    gqspi_prepare_cs(ctx, false);

    gqspi_prepare_cs(ctx, true);
    gqspi_write(ctx, CMD_CHIP_ERASE);
    gqspi_prepare_cs(ctx, false);
}

void chip_erase()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = 2,
        .base = QSPI_BASE,
    };
    __chip_erase(&ctx);
}

void gqspi_write_dummy_cycles(struct qspi_ctx *ctx, uint8_t dummy_cycles)
{
    /* Transfer something.  */
    writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
            | 0 << 19			/* Poll */
            | 0 << 18		/* Stripe */
            | 0 << 17			/* RX */
            | 0 << 16			/* TX */
            | ctx->data_bus_select << 14	/* Data bus select */
            | ctx->upper_cs << 13		/* UCS.  */
            | ctx->lower_cs << 12		/* LCS.  */
            | ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
            | 0 << 9			/* Exponent  */
            | 1 << 8			/* Data Xfer.  */
            | dummy_cycles);
}

void gqspi_write_data(struct qspi_ctx *ctx, uint8_t data)
{
    /* Transfer something.  */
    writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
            | 0 << 19			/* Poll */
            | 0 << 18		/* Stripe */
            | 0 << 17			/* RX */
            | 1 << 16			/* TX */
            | ctx->data_bus_select << 14	/* Data bus select */
            | ctx->upper_cs << 13		/* UCS.  */
            | ctx->lower_cs << 12		/* LCS.  */
            | ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
            | 0 << 9			/* Exponent  */
            | 0 << 8			/* Data Xfer.  */
            | data);
}

void gqspi_write_txfifo(struct qspi_ctx *ctx, bool exp, uint8_t size)
{
    /* Transfer something.  */
    writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
            | 0 << 19			/* Poll */
            | 0 << 18		/* Stripe */
            | 0 << 17			/* RX */
            | 1 << 16			/* TX */
            | ctx->data_bus_select << 14	/* Data bus select */
            | ctx->upper_cs << 13		/* UCS.  */
            | ctx->lower_cs << 12		/* LCS.  */
            | ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
            | exp << 9			/* Exponent  */
            | 1 << 8			/* Data Xfer.  */
            | size);
}

void gqspi_read_into_rx_fifo(struct qspi_ctx *ctx, bool exp, uint8_t imm)
{
    /* Transfer something.  */
    writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
            | 0 << 19			/* Poll */
            | ctx->stripe << 18		/* Stripe */
            | 1 << 17			/* RX */
            | 0 << 16			/* TX */
            | ctx->data_bus_select << 14	/* Data bus select */
            | ctx->upper_cs << 13		/* UCS.  */
            | ctx->lower_cs << 12		/* LCS.  */
            | ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
            | exp << 9			/* Exponent  */
            | 1 << 8			/* Data Xfer.  */
            | imm);
}

void __cmd_read(struct qspi_ctx *ctx, uint8_t *buf, size_t sz)
{
    uint8_t r;
    size_t i;

    gqspi_prepare_cs(ctx, true);

    /* Command */
    gqspi_write(ctx, CMD_READ);

    /* Address */
    gqspi_write(ctx, 0x00);
    gqspi_write(ctx, 0x00);
    gqspi_write(ctx, 0x00);

    /* Read and check byte */
    for (i = 0;  i < sz; i++) {
        r = gqspi_read(ctx);

        if (r != buf[i]) {
            err_msg("r != buf[%lu] == %x\n", i, buf[i]);
        }
    }

    gqspi_prepare_cs(ctx, false);
}

void check_cmd_read(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };

    __cmd_read(&ctx, buf, sz);
}

void check_cmd_read_upper_cs(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .upper_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };

    __cmd_read(&ctx, buf, sz);
}

void __cmd_read_data_xfer_exp(bool exp, uint32_t imm)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Read into rxfifo */
    gqspi_read_into_rx_fifo(&ctx, exp, imm);

    gqspi_prepare_cs(&ctx, false);
}

void check_cmd_read_data_xfer_exp(bool exp, uint8_t *buf, uint32_t imm)
{
    unsigned i;
    uint32_t sz = (exp == true) ? (1 << imm) : imm;

    __cmd_read_data_xfer_exp(exp, imm);

    /* Fill buffer */
    for (i = 0; i < sz; i+=4) {
        uint32_t r = qspi_read_reg(R_QSPI_GQSPI_RXD);

        buf[i+0] = r & 0xFF;
        buf[i+1] = (r >> 8) & 0xFF;
        buf[i+2] = (r >> 16) & 0xFF;
        buf[i+3] = (r >> 24) & 0xFF;
    }
}

void check_cmd_qor(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QOR);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    ctx.mode = SPI_MODE_4;

    /* Read and check byte */
    for (i = 0;  i < sz; i++) {
        r = gqspi_read(&ctx);

        if (r != buf[i]) {
            err_msg("r != buf[%lu] == %x\n", i, buf[i]);
        }
    }

    gqspi_prepare_cs(&ctx, false);
}

void check_cmd_qor4(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QOR4);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    ctx.mode = SPI_MODE_4;

    /* Read and check byte */
    for (i = 0;  i < sz; i++) {
        r = gqspi_read(&ctx);

        if (r != buf[i]) {
            err_msg("r != buf[%lu] == %x\n", i, buf[i]);
        }
    }

    gqspi_prepare_cs(&ctx, false);
}

void check_cmd_qior(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QIOR);

    ctx.mode = SPI_MODE_4;

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    /* Read and check byte */
    for (i = 0;  i < sz; i++) {
        r = gqspi_read(&ctx);

        if (r != buf[i]) {
            err_msg("r != buf[%lu] == %x\n", i, buf[i]);
        }
    }

    gqspi_prepare_cs(&ctx, false);
}

void check_cmd_qior4(uint8_t *buf, size_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QIOR4);

    ctx.mode = SPI_MODE_4;

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    /* Read and check byte */
    for (i = 0;  i < sz; i++) {
        r = gqspi_read(&ctx);

        if (r != buf[i]) {
            err_msg("r != buf[%lu] == %x\n", i, buf[i]);
        }
    }

    gqspi_prepare_cs(&ctx, false);
}

void check_gqspi_cmd_read()
{
    LOG1("%s", __func__);

    check_cmd_read(ff, sizeof(ff));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_qor()
{
    LOG1("%s", __func__);

    check_cmd_qor(ff, sizeof(ff));

    LOG2(" [OK]\n");
}


void check_gqspi_cmd_qor4()
{
    LOG1("%s", __func__);

    check_cmd_qor4(ff, sizeof(ff));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_qior()
{
    LOG1("%s", __func__);

    check_cmd_qior(ff, sizeof(ff));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_qior4()
{
    LOG1("%s", __func__);

    check_cmd_qior4(ff, sizeof(ff));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_dor()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    LOG1("%s", __func__);

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_DOR);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    ctx.mode = SPI_MODE_2;

    /* Read and check byte */
    for (i = 0;  i < 12; i++) {
        r = gqspi_read(&ctx);

        if (r != 0xFF) {
            err_msg("r != 0xFF\n");
        }
    }

    gqspi_prepare_cs(&ctx, false);

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_dor4()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    LOG1("%s", __func__);

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_DOR4);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    ctx.mode = SPI_MODE_2;

    /* Read and check byte */
    for (i = 0;  i < 12; i++) {
        r = gqspi_read(&ctx);

        if (r != 0xFF) {
            err_msg("r != 0xFF\n");
        }
    }

    gqspi_prepare_cs(&ctx, false);

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_dior()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    LOG1("%s", __func__);

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_DOR4);

    ctx.mode = SPI_MODE_2;

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    /* Read and check byte */
    for (i = 0;  i < 12; i++) {
        r = gqspi_read(&ctx);

        if (r != 0xFF) {
            err_msg("r != 0xFF\n");
        }
    }

    gqspi_prepare_cs(&ctx, false);

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_dior4()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    LOG1("%s", __func__);

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_DOR4);

    ctx.mode = SPI_MODE_2;

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Write dummy cycles */
    gqspi_write_dummy_cycles(&ctx, 8);

    /* Read and check byte */
    for (i = 0;  i < 12; i++) {
        r = gqspi_read(&ctx);

        if (r != 0xFF) {
            err_msg("r != 0xFF\n");
        }
    }

    gqspi_prepare_cs(&ctx, false);

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_pp()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;

    LOG1("%s (verify with cmd_read)", __func__);

    chip_erase();

    check_cmd_read(ff, sizeof(ff));

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_PP);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    for (i = 0; i < sizeof(def_buf); i++) {
        gqspi_write_data(&ctx, def_buf[i]);
    }

    gqspi_prepare_cs(&ctx, false);

    check_cmd_read(def_buf, sizeof(def_buf));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_qpp()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;

    LOG1("%s (verify with cmd_qor)", __func__);

    chip_erase();

    check_cmd_qor(ff, sizeof(ff));

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QPP);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    ctx.mode = SPI_MODE_4;

    for (i = 0; i < sizeof(def_buf); i++) {
        gqspi_write_data(&ctx, def_buf[i]);
    }

    gqspi_prepare_cs(&ctx, false);

    check_cmd_qor(def_buf, sizeof(def_buf));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_qpp4()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;

    LOG1("%s (verify with cmd_qor4)", __func__);

    chip_erase();

    check_cmd_qor4(ff, sizeof(ff));

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_QPP4);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    ctx.mode = SPI_MODE_4;

    for (i = 0; i < sizeof(def_buf); i++) {
        gqspi_write_data(&ctx, def_buf[i]);
    }

    gqspi_prepare_cs(&ctx, false);

    check_cmd_qor4(def_buf, sizeof(def_buf));

    LOG2(" [OK]\n");
}

void __qspi_write_data(struct qspi_ctx *ctx, uint8_t *buf, uint8_t len)
{
    uint8_t i;

    if (ctx == NULL) {
        err_msg("ctx == NULL");
    }

    gqspi_prepare_cs(ctx, true);
    /* Command */
    gqspi_write(ctx, CMD_PP);
    /* Address */
    gqspi_write(ctx, 0x00);
    gqspi_write(ctx, 0x00);
    gqspi_write(ctx, 0x00);
    for (i = 0; i < len; i++) {
        gqspi_write_data(ctx, buf[i]);
    }
    gqspi_prepare_cs(ctx, false);
}

void qspi_write_data(uint32_t len)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .upper_cs = 0,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t buf[len];
    int i;

    /* Count up */
    for (i = 0; i < len; i++) {
        buf[i] = i;
    }

    __qspi_write_data(&ctx, buf, len);
}

void check_gqspi_cmd_pp_with_txfifo()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;
    uint32_t r;
    bool exp = false;

    LOG1("%s", __func__);

    chip_erase();

    check_cmd_read(ff, sizeof(ff));

    /* Write txfifo */
    for (i = 0; i < 3; i++) {
        r  = def_buf[i*4+0];
        r |= def_buf[i*4+1] << 8;
        r |= def_buf[i*4+2] << 16;
        r |= def_buf[i*4+3] << 24;
        qspi_write_reg(R_QSPI_GQSPI_TXD, r);
    }

    /* Write out txfifo to flash */
    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_PP);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    gqspi_write_txfifo(&ctx, exp, sizeof(def_buf));

    gqspi_prepare_cs(&ctx, false);

    check_cmd_read(def_buf, sizeof(def_buf));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_pp_with_txfifo_exponent()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;
    uint32_t r;
    uint8_t buf[256];
    bool exp = true;

    LOG1("%s", __func__);

    chip_erase();

    check_cmd_read(ff, sizeof(ff));

    /* Write txfifo */
    for (i = 0; i < sizeof(buf); i++) {
        buf[i] = i;
    }
    for (i = 0; i < (sizeof(buf)/4); i++) {
        r  = buf[i*4+0];
        r |= buf[i*4+1] << 8;
        r |= buf[i*4+2] << 16;
        r |= buf[i*4+3] << 24;
        qspi_write_reg(R_QSPI_GQSPI_TXD, r);
    }

    /* Write out txfifo to flash */
    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_PP);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    gqspi_write_txfifo(&ctx, exp, 8);

    gqspi_prepare_cs(&ctx, false);

    check_cmd_read(buf, sizeof(buf));

    LOG2(" [OK]\n");
}

void enable_irq(uint32_t irq)
{
    uint32_t r = qspi_read_reg(R_QSPI_GQSPI_IMASK);

    r &= ~(irq);
    qspi_write_reg(R_QSPI_GQSPI_IMASK, r);
}

void disable_irq(uint32_t irq)
{
    uint32_t r = qspi_read_reg(R_QSPI_GQSPI_IMASK);

    r |= irq;
    qspi_write_reg(R_QSPI_GQSPI_IMASK, r);
}

bool is_set(uint32_t irq)
{
    uint32_t r = qspi_read_reg(R_QSPI_GQSPI_ISR);

    return r & irq;
}

bool check_gen_fifo_irqs()
{
    if (is_set(IRQ(GEN_FIFO_FULL)) == true) {
        print_err("IRQ_GEN_FIFO_FULL set when it should be unset!\n");
        return false;
    }
    if (is_set(IRQ(GEN_FIFO_NOT_FULL)) == false) {
        print_err("GEN_FIFO_NOT_FULL unset when it should be set!\n");
        return false;
    }
    if (is_set(IRQ(GEN_FIFO_EMPTY)) == false) {
        print_err("GEN_FIFO_EMPTY unset when it should be set!\n");
        return false;
    }
    return true;
}

bool check_all_tx_irqs()
{
    if (is_set(IRQ(TX_FIFO_EMPTY)) == false) {
        print_err("TX_FIFO_EMPTY unset when it should be set!\n");
        return false;
    }
    if (is_set(IRQ(TX_FIFO_FULL)) == true) {
        print_err("TX_FIFO_FULL set when it should be unset!\n");
        return false;
    }
    if (is_set(IRQ(TX_FIFO_NOT_FULL)) == false) {
        print_err("TX_FIFO_FULL unset when it should be set!\n");
        return false;
    }
    return check_gen_fifo_irqs();
}

void check_gqspi_irqs_with_cmd_pp()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;

    LOG1("%s", __func__);

    /* Clear tx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            (1 << QSPI_GQSPI_FIFO_CTRL_RST_TX_FIFO_SHIFT));

    chip_erase();

    check_cmd_read(ff, sizeof(ff));

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    if (check_all_tx_irqs() == false) {
        err();
    }

    gqspi_prepare_cs(&ctx, true);

    if (check_all_tx_irqs() == false) {
        err();
    }

    /* Command */
    gqspi_write(&ctx, CMD_PP);

    if (check_all_tx_irqs() == false) {
        err();
    }

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    if (check_all_tx_irqs() == false) {
        err();
    }

    for (i = 0; i < sizeof(def_buf); i++) {
        gqspi_write_data(&ctx, def_buf[i]);

        if (check_all_tx_irqs() == false) {
            err();
        }
    }

    gqspi_prepare_cs(&ctx, false);

    if (check_all_tx_irqs() == false) {
        err();
    }

    check_cmd_read(def_buf, sizeof(def_buf));

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    LOG2(" [OK]\n");
}

void check_gqspi_irqs_with_txfifo()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    size_t i;
    uint32_t r;
    bool exp = false;

    LOG1("%s", __func__);

    /* Clear tx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            (1 << QSPI_GQSPI_FIFO_CTRL_RST_TX_FIFO_SHIFT));

    chip_erase();

    check_cmd_read(ff, sizeof(ff));

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    /* Write txfifo */
    for (i = 0; i < 3; i++) {
        r  = def_buf[i*4+0];
        r |= def_buf[i*4+1] << 8;
        r |= def_buf[i*4+2] << 16;
        r |= def_buf[i*4+3] << 24;
        qspi_write_reg(R_QSPI_GQSPI_TXD, r);
    }

    if (is_set(IRQ(TX_FIFO_EMPTY)) == true) {
        err_msg("TX_FIFO_EMPTY set when it should be unset!");
    }
    if (is_set(IRQ(TX_FIFO_FULL)) == true) {
        err_msg("TX_FIFO_FULL set when it should be unset!");
    }
    if (is_set(IRQ(TX_FIFO_NOT_FULL)) == true) {
        err_msg("TX_FIFO_NOT_FULL set when it should be unset!");
    }

    /* Test TX threshold value */
    qspi_write_reg(R_QSPI_GQSPI_TX_THRESH, 14);
    if (is_set(IRQ(TX_FIFO_NOT_FULL)) == false) {
        err_msg("TX_FIFO_NOT_FULL unset when it should be set!");
    }
    qspi_write_reg(R_QSPI_GQSPI_TX_THRESH, 1);
    if (is_set(IRQ(TX_FIFO_NOT_FULL)) == true) {
        err_msg("TX_FIFO_NOT_FULL set when it should be unset!");
    }

    /* Write out txfifo to flash */
    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_PP);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    gqspi_write_txfifo(&ctx, exp, sizeof(def_buf));

    gqspi_prepare_cs(&ctx, false);

    if (check_all_tx_irqs() == false) {
        err();
    }

    check_cmd_read(def_buf, sizeof(def_buf));

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    LOG2(" [OK]\n");
}

bool check_all_rx_irqs()
{
    if (is_set(IRQ(RX_FIFO_EMPTY)) == false) {
        print_err("RX_FIFO_EMPTY unset when it should be set!\n");
        return false;
    }
    if (is_set(IRQ(RX_FIFO_FULL)) == true) {
        print_err("RX_FIFO_FULL set when it should be unset!\n");
        return false;
    }
    if (is_set(IRQ(RX_FIFO_NOT_EMPTY)) == true) {
        print_err("RX_FIFO_NOT_EMPTY set when it should be unset!\n");
        return false;
    }
    return check_gen_fifo_irqs();
}

void check_gqspi_irqs_after_rx_tx_fifo_reset()
{
    uint32_t r;
    bool exp = false;
    uint8_t buf[16];
    unsigned i;

    LOG1("%s", __func__);

    chip_erase();

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    /* Write txfifo */
    for (i = 0; i < 3; i++) {
        r  = buf[i*4+0];
        r |= buf[i*4+1] << 8;
        r |= buf[i*4+2] << 16;
        r |= buf[i*4+3] << 24;
        qspi_write_reg(R_QSPI_GQSPI_TXD, r);
    }

    if (is_set(IRQ(TX_FIFO_EMPTY)) == true) {
        err_msg("TX_FIFO_EMPTY set when it should be unset!");
    }
    if (is_set(IRQ(TX_FIFO_FULL)) == true) {
        err_msg("TX_FIFO_FULL set when it should be unset!");
    }
    if (is_set(IRQ(TX_FIFO_NOT_FULL)) == true) {
        err_msg("TX_FIFO_NOT_FULL set when it should be unset!");
    }

    /* Read into rxfifo */
    __cmd_read_data_xfer_exp(exp, sizeof(buf));

    if (is_set(IRQ(RX_FIFO_EMPTY)) == true) {
        err_msg("RX_FIFO_EMPTY set when it should be unset!");
    }
    if (is_set(IRQ(RX_FIFO_FULL)) == true) {
        err_msg("RX_FIFO_FULL set when it should be unset!");
    }
    if (is_set(IRQ(RX_FIFO_NOT_EMPTY)) == false) {
        err_msg("RX_FIFO_NOT_EMPTY unset when it should be set!");
    }

    /* Clear rx tx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            (1 << QSPI_GQSPI_FIFO_CTRL_RST_RX_FIFO_SHIFT) |
            (1 << QSPI_GQSPI_FIFO_CTRL_RST_TX_FIFO_SHIFT));

    if (check_all_tx_irqs() == false) {
        err();
    }

    if (check_all_rx_irqs() == false) {
        err();
    }

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(TX_FIFO_EMPTY) |
            IRQ(TX_FIFO_FULL) |
            IRQ(TX_FIFO_NOT_FULL));

    LOG2(" [OK]\n");
}

void check_gqspi_irqs_with_cmd_read()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    uint8_t r;
    size_t i;

    LOG1("%s", __func__);

    chip_erase();

    /* Clear rx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            1 << QSPI_GQSPI_FIFO_CTRL_RST_RX_FIFO_SHIFT);

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));

    if (check_all_rx_irqs() == false) {
        err();
    }

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Read and check byte */
    for (i = 0;  i < 8; i++) {
        r = gqspi_read(&ctx);

        if (r != 0xFF) {
            err_msg("r != 0xFF\n");
        }

        if (check_all_rx_irqs() == false) {
            err();
        }
    }

    gqspi_prepare_cs(&ctx, false);

    if (check_all_rx_irqs() == false) {
        err();
    }

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));

    LOG2(" [OK]\n");
}

void check_gqspi_irqs_with_rxfifo()
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };

    LOG1("%s", __func__);

    chip_erase();

    /* Clear rx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            1 << QSPI_GQSPI_FIFO_CTRL_RST_RX_FIFO_SHIFT);

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));

    if (check_all_rx_irqs() == false) {
        err();
    }

    gqspi_prepare_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Read and check byte */
    gqspi_read_into_rx_fifo(&ctx, false, 12);

    if (is_set(IRQ(RX_FIFO_EMPTY)) == true) {
        err_msg("RX_FIFO_EMPTY set when it should be unset!");
    }
    if (is_set(IRQ(RX_FIFO_FULL)) == true) {
        err_msg("RX_FIFO_FULL set when it should be unset!");
    }
    if (is_set(IRQ(RX_FIFO_NOT_EMPTY)) == false) {
        err_msg("RX_FIFO_NOT_EMPTY unset when it should be set!");
    }

    qspi_write_reg(R_QSPI_GQSPI_RX_THRESH, 14);
    if (is_set(IRQ(RX_FIFO_NOT_EMPTY)) == true) {
        err_msg("RX_FIFO_NOT_EMPTY set when it should be unset!");
    }
    qspi_write_reg(R_QSPI_GQSPI_RX_THRESH, 1);
    if (is_set(IRQ(RX_FIFO_NOT_EMPTY)) == false) {
        err_msg("RX_FIFO_NOT_EMPTY unset when it should be set!");
    }

    gqspi_prepare_cs(&ctx, false);

    /* Clear rx fifo */
    qspi_write_reg(R_QSPI_GQSPI_FIFO_CTRL,
            1 << QSPI_GQSPI_FIFO_CTRL_RST_RX_FIFO_SHIFT);

    if (check_all_rx_irqs() == false) {
        err();
    }

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_read_into_rxfifo()
{
    uint8_t buf[16];
    bool exp = false;
    unsigned i;

    LOG1("%s", __func__);


    chip_erase();

    check_cmd_read_data_xfer_exp(exp, buf, sizeof(buf));

    /* Check bytes */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != 0xFF) {
            err_msg("buf[i] != 0xFF\n");
        }
    }

    qspi_write_data(16);

    check_cmd_read_data_xfer_exp(exp, buf, sizeof(buf));

    /* Check bytes */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != i) {
            err_msg("buf[i] != %d\n", i);
        }
    }

    LOG2(" [OK]\n");
}

void check_gqspi_cmd_read_into_rxfifo_exponent()
{
    uint8_t buf[16];
    bool exp = true;
    uint8_t imm = 4;    /* 2^4 == 16 bytes */
    unsigned i;

    LOG1("%s", __func__);

    chip_erase();

    check_cmd_read_data_xfer_exp(exp, buf, imm);

    /* Check bytes */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != 0xFF) {
            err_msg("buf[i] != 0xFF\n");
        }
    }

    qspi_write_data(16);

    check_cmd_read_data_xfer_exp(exp, buf, imm);

    /* Check bytes */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != i) {
            err_msg("buf[i] != %d\n", i);
        }
    }

    LOG2(" [OK]\n");
}

void check_bus_cs(struct qspi_ctx *ctx)
{
    __chip_erase(ctx);
    __cmd_read(ctx, ff, sizeof(ff));

    __qspi_write_data(ctx, def_buf, sizeof(def_buf));
    __cmd_read(ctx, def_buf, sizeof(def_buf));
}

void check_gqspi_lower_bus_lower_cs()
{
    struct qspi_ctx ctx = {
        .data_bus_select = LOWER_BUS,
        .lower_cs = 1,
        .mode = 2,
        .base = QSPI_BASE,
    };

    LOG1("%s", __func__);

    check_bus_cs(&ctx);

    LOG2(" [OK]\n");
}

void check_gqspi_lower_bus_upper_cs()
{
    struct qspi_ctx ctx = {
        .data_bus_select = LOWER_BUS,
        .upper_cs = 1,
        .mode = 2,
        .base = QSPI_BASE,
    };

    LOG1("%s", __func__);

    check_bus_cs(&ctx);

    LOG2(" [OK]\n");
}

void check_gqspi_upper_bus_lower_cs()
{
    struct qspi_ctx ctx = {
        .data_bus_select = UPPER_BUS,
        .lower_cs = 1,
        .mode = 2,
        .base = QSPI_BASE,
    };

    LOG1("%s", __func__);

    check_bus_cs(&ctx);

    LOG2(" [OK]\n");
}

void check_gqspi_upper_bus_upper_cs()
{
    struct qspi_ctx ctx = {
        .data_bus_select = UPPER_BUS,
        .upper_cs = 1,
        .mode = 2,
        .base = QSPI_BASE,
    };

    LOG1("%s", __func__);

    check_bus_cs(&ctx);

    LOG2(" [OK]\n");
}

void gqspi_stripe_write_data(struct qspi_ctx *ctx,
                             bool rx, bool tx,
                             bool data_xfer, uint8_t data)
{
	/* Transfer something.  */
	writel(ctx->base + R_QSPI_GQSPI_GEN_FIFO, 0
			| 0 << 19			/* Poll */
			| ctx->stripe << 18		/* Stripe */
			| rx << 17			/* RX */
			| tx << 16			/* TX */
			| ctx->data_bus_select << 14	/* Data bus select */
			| ctx->upper_cs << 13		/* UCS.  */
			| ctx->lower_cs << 12		/* LCS.  */
			| ctx->mode << 10		/* 0 Rsvd. 1 - SPI, 2 - Dual SPI, 3 - Quad SPI.  */
			| 0 << 9			/* Exponent  */
			| data_xfer << 8	/* Data Xfer.  */
			| data);
}

void qspi_read_with_stripe(uint8_t *buf, uint8_t len)
{
	struct qspi_ctx ctx = {
		.data_bus_select = 3,
		.lower_cs = 1,
		.upper_cs = 1,
		.mode = SPI_MODE_1,
		.base = QSPI_BASE,
        .stripe = false
	};
    bool rx = false;
    bool tx = false;
    bool data_xfer = false;
    uint8_t data = 4;
    int i;

    /* CS */
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* CMD_READ */
    tx = true;
    data = CMD_READ;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Address 0x00, 0x00, 0x00 */
    data = 0x00;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Read with stripe */
    ctx.stripe = true;
    tx = false;
    rx = true;
    data_xfer = true;
    data = len;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Deassert CS */
    rx = false;
    tx = false;
    data_xfer = false;
    data = 4;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Fill buffer */
    for (i = 0; i < len; i+=4) {
        uint32_t r = qspi_read_reg(R_QSPI_GQSPI_RXD);

        D(printf("r = 0%x\n", r));

        buf[i+0] = r & 0xFF;
        buf[i+1] = (r >> 8) & 0xFF;
        buf[i+2] = (r >> 16) & 0xFF;
        buf[i+3] = (r >> 24) & 0xFF;
    }
}

void qspi_write_with_stripe(uint8_t *buf, uint8_t len)
{
	struct qspi_ctx ctx = {
		.data_bus_select = 3,
		.lower_cs = 1,
		.upper_cs = 1,
		.mode = SPI_MODE_1,
		.base = QSPI_BASE,
        .stripe = false
	};
    bool rx = false;
    bool tx = false;
    bool data_xfer = false;
    uint8_t data = 4;
    int i;

    /* Fill txfifo */
    for (i = 0; i < 3; i++) {
        uint32_t r;

        r  = buf[i*4+0];
        r |= buf[i*4+1] << 8;
        r |= buf[i*4+2] << 16;
        r |= buf[i*4+3] << 24;
        qspi_write_reg(R_QSPI_GQSPI_TXD, r);
    }

    /* CS */
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* CMD_PP */
    tx = true;
    data = CMD_PP;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Address 0x00, 0x00, 0x00 */
    data = 0x00;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Write with stripe */
    ctx.stripe = true;
    tx = true;
    rx = false;
    data_xfer = true;
    data = len;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);

    /* Deassert CS */
    rx = false;
    tx = false;
    data_xfer = false;
    data = 4;
    gqspi_stripe_write_data(&ctx, rx, tx, data_xfer, data);
}

void check_gqspi_stripe_read()
{
	struct qspi_ctx lower_ctx = {
		.data_bus_select = LOWER_BUS,
		.lower_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
	struct qspi_ctx upper_ctx = {
		.data_bus_select = UPPER_BUS,
		.upper_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
    uint8_t zeros[] = { 0, 0, 0, 0 };
    uint8_t ones[] = { 1, 1, 1, 1 };
    uint8_t buf[sizeof(zeros) + sizeof(ones)];
    int i;

    LOG1("%s", __func__);

    __chip_erase(&lower_ctx);
    __chip_erase(&upper_ctx);

    __qspi_write_data(&lower_ctx, zeros, sizeof(zeros));
    __qspi_write_data(&upper_ctx, ones, sizeof(ones));

    qspi_read_with_stripe(buf, sizeof(buf));

    D(dump_buf(buf));

    for (i = 0; i < sizeof(buf); i++)
    {
        if (buf[i] != (i % 2)) {
            err_msg("buf[%u] != %u", buf[i], (i % 2));
        }
    }

    LOG2(" [OK]\n");
}

void check_gqspi_stripe_write()
{
	struct qspi_ctx lower_ctx = {
		.data_bus_select = LOWER_BUS,
		.lower_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
	struct qspi_ctx upper_ctx = {
		.data_bus_select = UPPER_BUS,
		.upper_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
    uint8_t zeros[4] = { 0, 0, 0, 0 };
    uint8_t ones[4] = { 1, 1, 1, 1 };
    uint8_t buf[] = { 0, 1, 0, 1, 0, 1, 0, 1 };
    int i;

    LOG1("%s", __func__);

    __chip_erase(&lower_ctx);
    __chip_erase(&upper_ctx);

    qspi_write_with_stripe(buf, sizeof(buf));

    __cmd_read(&lower_ctx, zeros, sizeof(zeros));
    __cmd_read(&upper_ctx, ones, sizeof(ones));

    D(dump_buf(zeros));
    D(dump_buf(ones));

    for (i = 0; i < sizeof(zeros); i++)
    {
        if (zeros[i] != 0) {
            err_msg("zeros[%d] != 0", i);
        }
    }

    for (i = 0; i < sizeof(ones); i++)
    {
        if (ones[i] != 1) {
            err_msg("ones[%d] != 1", i);
        }
    }

    LOG2(" [OK]\n");
}

void check_gqspi(void)
{
    LOG0("%s\n", __func__);

    chip_erase();

    check_gqspi_cmd_read();
    check_gqspi_cmd_dor();
    check_gqspi_cmd_dor4();
    check_gqspi_cmd_dior();
    check_gqspi_cmd_dior4();
    check_gqspi_cmd_qor();
    check_gqspi_cmd_qor4();
    check_gqspi_cmd_qior();
    check_gqspi_cmd_qior4();
    check_gqspi_cmd_read_into_rxfifo();
    check_gqspi_cmd_read_into_rxfifo_exponent();

    check_gqspi_cmd_pp();
    check_gqspi_cmd_qpp();
    check_gqspi_cmd_qpp4();
    check_gqspi_cmd_pp_with_txfifo();
    check_gqspi_cmd_pp_with_txfifo_exponent();

    check_gqspi_lower_bus_lower_cs();
//    check_gqspi_lower_bus_upper_cs();
//    check_gqspi_upper_bus_lower_cs();
    check_gqspi_upper_bus_upper_cs();

    check_gqspi_stripe_read();
    check_gqspi_stripe_write();

    LOG2("\n");
}

void check_gqspi_irqs(void)
{
    LOG0("%s\n", __func__);

    check_gqspi_irqs_with_cmd_read();
    check_gqspi_irqs_with_rxfifo();
    check_gqspi_irqs_with_cmd_pp();
    check_gqspi_irqs_with_txfifo();
    check_gqspi_irqs_after_rx_tx_fifo_reset();

    LOG2("\n");
}

/******************************************************************************/

void setup_lqspi_cs(uint32_t lower_cs, uint32_t upper_cs)
{
    uint32_t r = qspi_read_reg(R_QSPI_CONFIG_REG);

    /* Clear and set lower chip select enable */
    r &= ~(1 << QSPI_CONFIG_REG_PCS_SHIFT);
    r |= !lower_cs << QSPI_CONFIG_REG_PCS_SHIFT;

    /* Clear and set upper chip select */
    r &= ~(1 << (QSPI_CONFIG_REG_PCS_SHIFT + 1));
    r |= !upper_cs << (QSPI_CONFIG_REG_PCS_SHIFT + 1);

    qspi_write_reg(R_QSPI_CONFIG_REG, r);
}

void setup_lqspi(struct spi_cmd *cmd)
{
    uint32_t r = qspi_read_reg(R_QSPI_CONFIG_REG);

    /* Clear manual start enable */
    r &= ~(1 << QSPI_CONFIG_REG_MAN_START_EN_SHIFT);

    qspi_write_reg(R_QSPI_CONFIG_REG, r);

    /* LQSPI config */
    r = (1 << QSPI_LQSPI_CFG_LQ_MODE_SHIFT) | cmd->cmd;
    r |= cmd->dummy_bytes << QSPI_LQSPI_CFG_DUMMY_BYTE_SHIFT;
    if (cmd->addr_length == 4)
    {
        r |= 1 << QSPI_LQSPI_CFG_ADDR_32BIT_SHIFT;
    }
    qspi_write_reg(R_QSPI_LQSPI_CFG, r);
}

void check_lqspi_data(uint8_t *buf, size_t sz)
{
    int i;

    for (i = 0; i < sz; i+=4) {
        uint32_t v = QSPI_LINEAR_BASE[i/4];

        D(printf("[%d] v: %x [%x]\n", i, v, buf[i]));

        if ((v & 0xFF) != buf[i+0] ||
                (v >> 8 & 0xFF) != buf[i+1] ||
                (v >> 16 & 0xFF) != buf[i+2] ||
                (v >> 24 & 0xFF) != buf[i+3]) {
            err_msg("QSPI_LINEAR_BASE[i] != buf[i]\n");
        }
    }
}

uint32_t dummy_cache_reload(struct spi_cmd *cmd)
{
    uint32_t v;

    setup_lqspi(cmd);

    /* Assert lower cs */
    setup_lqspi_cs(1, 0);

    /* Enable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 1);

    /* Check data */
    v = QSPI_LINEAR_BASE[1024/4];

    setup_lqspi_cs(0, 0);

    /* Disable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 0);

    return v;
}

void check_lqspi(struct spi_cmd *cmd, uint8_t *buf, size_t sz)
{
    setup_lqspi(cmd);

    /* Assert lower cs */
    setup_lqspi_cs(1, 0);

    /* Enable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 1);

    /* Check data */
    check_lqspi_data(buf, sz);

    setup_lqspi_cs(0, 0);

    /* Disable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 0);
}

void lqspi_enable_irq(uint32_t irq)
{
    qspi_write_reg(R_QSPI_INTRPT_EN_REG, irq);
}

void lqspi_disable_irq(uint32_t irq)
{
    qspi_write_reg(R_QSPI_INTRPT_DIS_REG, irq);
}

bool lqspi_is_set(uint32_t irq)
{
    uint32_t r = qspi_read_reg(R_QSPI_INTR_STATUS_REG);

    return r & irq;
}

void check_linear_address_mode_irqs()
{
    struct spi_cmd cmd = SPI_CMD(CMD_READ, 0);

    LOG1("%s", __func__);

    lqspi_enable_irq(LQSPI_IRQ(RX_FIFO_NOT_EMPTY) |
            LQSPI_IRQ(RX_FIFO_FULL) |
            LQSPI_IRQ(RX_OVERFLOW));

    chip_erase();

    check_lqspi(&cmd, ff, sizeof(ff));

    if (lqspi_is_set(LQSPI_IRQ(RX_OVERFLOW)) == true) {
        err_msg("RX_OVERFLOW set when it should be unset!");
    }
    if (lqspi_is_set(LQSPI_IRQ(RX_FIFO_FULL)) == true) {
        err_msg("RX_FIFO_FULL set when it should be unset!");
    }
    if (lqspi_is_set(LQSPI_IRQ(RX_FIFO_NOT_EMPTY)) == true) {
        err_msg("RX_FIFO_NOT_EMPTY set when it should be unset!");
    }

    LOG2("\n");
}

void setup_lqspi_stripe_cmd_read()
{
    uint32_t r = qspi_read_reg(R_QSPI_CONFIG_REG);

    /* Clear manual start enable */
    r &= ~(1 << QSPI_CONFIG_REG_MAN_START_EN_SHIFT);

    qspi_write_reg(R_QSPI_CONFIG_REG, r);

    /* LQSPI config */
    r = (1 << QSPI_LQSPI_CFG_LQ_MODE_SHIFT) | CMD_READ;
    r |= (1 << QSPI_LQSPI_CFG_TWO_MEM_SHIFT);
    r |= (1 << QSPI_LQSPI_CFG_SEP_BUS_SHIFT);
    qspi_write_reg(R_QSPI_LQSPI_CFG, r);
}

void check_linear_address_mode_stripe()
{
	struct qspi_ctx lower_ctx = {
		.data_bus_select = LOWER_BUS,
		.lower_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
	struct qspi_ctx upper_ctx = {
		.data_bus_select = UPPER_BUS,
		.upper_cs = 1,
		.mode = 2,
		.base = QSPI_BASE,
	};
    uint8_t zeros[] = { 0, 0, 0, 0 };
    uint8_t ff[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t buf[] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, };

    LOG1("%s", __func__);

    __chip_erase(&lower_ctx);
    __chip_erase(&upper_ctx);

    __qspi_write_data(&lower_ctx, zeros, sizeof(zeros));
    __qspi_write_data(&upper_ctx, ff, sizeof(ff));

    /* Setup lqspi cfg */
    setup_lqspi_stripe_cmd_read();

    /* Assert lower cs */
    setup_lqspi_cs(1, 0);

    /* Enable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 1);

    /* Check data */
    check_lqspi_data(buf, sizeof(buf));

    setup_lqspi_cs(0, 0);

    /* Disable qspi */
    qspi_write_reg(R_QSPI_GQSPI_EN_REG, 0);

    LOG2(" [OK]\n");
}

void check_linear_address_mode()
{
    /*
     * 1 dummy byte for DOR/DOR4/QOR/QOR4 == 8 dummy cycles,
     * 2 dummy bytes for DIOR/DIOR4 == 8 dummy cycles
     * 4 dummy bytes for QIOR/QIOR4 == 8 dummy cycles
     */
    struct spi_cmd cmd[] = { SPI_CMD(CMD_READ, 0),
        SPI_CMD(CMD_DOR, 1),
        SPI_CMD4(CMD_DOR4, 1),
        SPI_CMD(CMD_DIOR, 2),
        SPI_CMD4(CMD_DIOR4, 2),
        SPI_CMD(CMD_QOR, 1),
        SPI_CMD4(CMD_QOR4, 1),
        SPI_CMD(CMD_QIOR, 4),
        SPI_CMD4(CMD_QIOR4, 4) };
    int i;

    LOG0("%s\n", __func__);

    for (i = 0; i < ARRAY_SZ(cmd); i++)
    {
        LOG1("%s with %s", __func__, cmd[i].name);

        chip_erase();

        check_lqspi(&cmd[i], ff, sizeof(ff));

        qspi_write_data(12);

        dummy_cache_reload(&cmd[i]);

        check_lqspi(&cmd[i], def_buf, sizeof(def_buf));

        dummy_cache_reload(&cmd[i]);

        LOG2(" [OK]\n");
    }

    check_linear_address_mode_stripe();

    check_linear_address_mode_irqs();

    LOG2("\n");
}

void qspi_dma_setup(uint64_t dst, uint32_t num_bytes)
{
    /* DMA DST */
    qspi_write_reg(R_QSPI_QSPIDMA_DST_ADDR, dst & 0xFFFFFFFF);
    qspi_write_reg(R_QSPI_QSPIDMA_DST_ADDR_MSB, ((dst >> 32) & 0xFFFFFFFF));
    qspi_write_reg(R_QSPI_QSPIDMA_DST_SIZE, num_bytes);
}

bool qspi_dma_rd_xfer_completed()
{
    uint32_t r = qspi_read_reg(R_QSPI_QSPIDMA_DST_I_STS);
    return (r & (1 << QSPI_QSPIDMA_DST_I_STS_DONE_SHIFT));
}

void gqspi_dma_cs(struct qspi_ctx *ctx, bool cs)
{
    writel(ctx->base + R_QSPI_GQSPI_CFG,
            2 << QSPI_GQSPI_CFG_MODE_EN_SHIFT
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
            | (ctx->lower_cs & cs) << 12  /* LCS.  */
            | ctx->mode << 10  /* Dual SPI.  */
            | 0 << 9   /* Exponent  */
            | 0 << 8   /* Data Xfer.  */
            | 0x04);
}

void qspi_dma_read(uint8_t *buf, uint32_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    bool exp = false;

    /* Setup dma */
    qspi_dma_setup((uint64_t) buf, sz);

    gqspi_dma_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Read into rxfifo */
    gqspi_read_into_rx_fifo(&ctx, exp, sz);

    gqspi_dma_cs(&ctx, false);

    /* Wait for completion */
    while (qspi_dma_rd_xfer_completed() == false);
}

void qspi_dma_read_irq(uint8_t *buf, uint32_t sz)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    bool exp = false;

    /* Setup dma */
    qspi_dma_setup((uint64_t) buf, sz);

    enable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));

    if (check_all_rx_irqs() == false) {
        err();
    }

    gqspi_dma_cs(&ctx, true);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Read into rxfifo */
    gqspi_read_into_rx_fifo(&ctx, exp, sz);

    gqspi_dma_cs(&ctx, false);

    if (check_all_rx_irqs() == false) {
        err();
    }

    /* Wait for completion */
    while (qspi_dma_rd_xfer_completed() == false);

    disable_irq(IRQ(GEN_FIFO_FULL) |
            IRQ(GEN_FIFO_NOT_FULL) |
            IRQ(GEN_FIFO_EMPTY) |
            IRQ(RX_FIFO_EMPTY) |
            IRQ(RX_FIFO_FULL) |
            IRQ(RX_FIFO_NOT_EMPTY));
}

void qspi_dma_read_exp(uint8_t *buf, uint32_t sz, uint32_t exp_val)
{
    struct qspi_ctx ctx = {
        .data_bus_select = 1,
        .lower_cs = 1,
        .mode = SPI_MODE_1,
        .base = QSPI_BASE,
    };
    bool exp = true;

    /* Setup dma */
    qspi_dma_setup((uint64_t) buf, sz);

    gqspi_dma_cs(&ctx, true);

    /* Command */
    gqspi_write(&ctx, CMD_READ);

    /* Address */
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);
    gqspi_write(&ctx, 0x00);

    /* Read into rxfifo (exponential) */
    gqspi_read_into_rx_fifo(&ctx, exp, exp_val);

    gqspi_dma_cs(&ctx, false);

    /* Wait for completion */
    while (qspi_dma_rd_xfer_completed() == false);
}

void check_qspi_dma_read_128()
{
    uint8_t buf[128];
    uint8_t i;

    LOG1("%s", __func__);

    chip_erase();

    qspi_dma_read(buf, sizeof(buf));

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != 0xFF) {
            err_msg("buf[%u] == %u != 0xFF\n", i, buf[i]);
        }
    }

    qspi_write_data(sizeof(buf));

    qspi_dma_read(buf, sizeof(buf));

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != i) {
            err_msg("buf[%u] == %u != %u\n", i, buf[i], i);
        }
    }

    LOG2(" [OK]\n");
}

void check_qspi_dma_read_128_irqs()
{
    uint8_t buf[128];
    uint8_t i;

    LOG1("%s", __func__);

    chip_erase();

    qspi_dma_read_irq(buf, sizeof(buf));

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != 0xFF) {
            err_msg("buf[%u] == %u != 0xFF\n", i, buf[i]);
        }
    }

    qspi_write_data(sizeof(buf));

    qspi_dma_read_irq(buf, sizeof(buf));

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != i) {
            err_msg("buf[%u] == %u != %u\n", i, buf[i], i);
        }
    }

    LOG2(" [OK]\n");
}

void check_qspi_dma_read_128_exponent()
{
    uint8_t exp_val = 7; /* 1 << 7 = 128 */
    uint8_t buf[128];
    uint8_t i;

    LOG1("%s", __func__);

    chip_erase();

    qspi_dma_read_exp(buf, sizeof(buf), exp_val);

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != 0xFF) {
            err_msg("buf[%u] == %u != 0xFF\n", i, buf[i]);
        }
    }

    qspi_write_data(sizeof(buf));

    qspi_dma_read_exp(buf, sizeof(buf), exp_val);

    /* Verify data */
    for (i = 0;  i < sizeof(buf); i++) {
        if (buf[i] != i) {
            err_msg("buf[%u] == %u != %u\n", i, buf[i], i);
        }
    }

    LOG2(" [OK]\n");
}

void check_dma_mode()
{
    LOG0("%s\n", __func__);

    check_qspi_dma_read_128();
    check_qspi_dma_read_128_exponent();
    check_qspi_dma_read_128_irqs();

    LOG2("\n");
}

void check_qspi()
{
    printf("%s\n", __func__);

    check_gqspi();
    check_gqspi_irqs();

    check_linear_address_mode();
    //check_dma_mode();
}
