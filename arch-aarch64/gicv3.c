/*
 * GICV3 Driver
 *
 * Copyright (C) 2016 Xilinx Inc
 * Written by Francisco Iglesias
 *
 * References:
 * [1] GICv3_prd03_genc_010745_19.0.pdf
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <byteswap.h>
#include "gicv3.h"
#include "sys.h"
#include "plat.h"

struct handler_table {
	gicv3_handler h[256];
	void *opaque[256];
} handler_t;

static void wait_for(phys_addr_t reg, uint32_t mask)
{
	uint32_t r = readl(reg);

	while (r & mask) {
		r = readl(reg);
	}
}

void gicd_wait_for(uint32_t reg, uint32_t mask)
{
    wait_for(GICD_BASE + reg, mask);
}

void gicr_wait_for(uint32_t reg, uint32_t mask)
{
    wait_for(GICR_BASE + reg, mask);
}

void gicd_config()
{
    uint32_t r = GICD_READ_REG(GICD_CTLR);

    /* Disable interrupts before configuring affinity routing,
     * see page 30 in [1] */

    r &= ~(GICD_CTLR_EN_GRP0  | GICD_CTLR_EN_GRP1S | GICD_CTLR_EN_GRP1NS);
    GICD_WRITE_REG(GICD_CTLR, r);

    gicd_wait_for(GICD_CTLR, GICD_CTLR_RWP);

    /* Configure affinity routing and enable irq groups */
    r |= (GICD_CTLR_ARE_NS   |
          GICD_CTLR_ARE_S    |
          GICD_CTLR_EN_GRP0  |
          GICD_CTLR_EN_GRP1S |
          GICD_CTLR_EN_GRP1NS);

    GICD_WRITE_REG(GICD_CTLR, r);

    gicd_wait_for(GICD_CTLR, GICD_CTLR_RWP);
}

void gicr_config()
{
    uint32_t r = GICR_READ_REG(GICR_WAKER);

    /* Wake cpu */
    r &= ~(GICR_WAKER_ProcessorSleep);
    GICR_WRITE_REG(GICR_WAKER, r);
    gicr_wait_for(GICR_WAKER, GICR_WAKER_ChildrenAsleep);
}

void cpuif_config()
{
    uint32_t r;

    ICC_WRITE_REG(ICC_SRE_EL3, 0xF);
    ICC_WRITE_REG(ICC_SRE_EL2, 0xF);
    ICC_WRITE_REG(ICC_SRE_EL1, 0x7);

    ICC_WRITE_REG(ICC_PMR_EL1, 0xFF);

    ICC_READ_REG(r, ICC_CTLR_EL3);
    r &= ~(ICC_CTLR_EL3_EOIMODE_ALL);
    ICC_WRITE_REG(ICC_CTLR_EL3, r);

    ICC_WRITE_REG(ICC_IGRPEN0_EL1, 1);
    ICC_WRITE_REG(ICC_IGRPEN1_EL1, 1);
}

void gicd_set_routing(uint32_t irq)
{
    uint32_t r = 0;
    /*
     * Leave Interrupt Routing Mode = 0 [bit 40], meaning target list routing
     * (page 30 in [1]).
     *
     * Target cpu 0.0.0.0 = bit 0 set */

    GICD_WRITE_REG(GICD_IROUTER + irq*8, r);
}

void gicd_set_prio(uint32_t irq, uint32_t prio)
{
    uint32_t r = GICD_READ_REG(GICD_IPRIORITYR + (irq/4)*4);
    unsigned byte_offset = irq % 4;
    unsigned shift = byte_offset * 8;

    /* Clear prio for sgi */
    r &= ~(0xFF << shift);

    /* Set prio for irq/sgi */
    r |= (prio << shift);

    GICD_WRITE_REG(GICD_IPRIORITYR + (irq/4)*4, r);
}

void gicd_set_group0(uint32_t irq)
{
    uint32_t r = GICD_READ_REG(GICD_IGROUPR + (irq/32)*4);
    r &= ~(1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_IGROUPR + (irq/32)*4, r);

    r = GICD_READ_REG(GICD_IGRPMODR + (irq/32)*4);
    r &= ~(1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_IGRPMODR + (irq/32)*4, r);
}

void gicd_set_group1ns(uint32_t irq)
{
    uint32_t r = GICD_READ_REG(GICD_IGROUPR + (irq/32)*4);
    r |= (1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_IGROUPR + (irq/32)*4, r);

    r = GICD_READ_REG(GICD_IGRPMODR + (irq/32)*4);
    r &= ~(1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_IGRPMODR + (irq/32)*4, r);
}

void gicd_enable(uint32_t irq)
{
    uint32_t r = GICD_READ_REG(GICD_ISENABLER + (irq/32)*4);
    r |= (1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_ISENABLER + (irq/32)*4, r);
}

void gicd_disable(uint32_t irq)
{
    uint32_t r = GICD_READ_REG(GICD_ICENABLER + (irq/32)*4);
    r |= (1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_ICENABLER + (irq/32)*4, r);
}

void gicd_set_irq(uint64_t irq)
{
    uint32_t r = 0;
    /* Set pending for irq */
    r |= (1U << ((irq%32)));   /* 1 bit for each irq */
    GICD_WRITE_REG(GICD_ISPENDR + (irq/32)*4, r);
}

void gicr_set_prio(uint32_t sgi, uint32_t prio)
{
    uint32_t r = GICR_READ_REG(GICR_IPRIORITYR + sgi/4);
    unsigned byte_offset = sgi % 4;
    unsigned shift = (3-byte_offset) * 8;

    /* Clear prio for sgi */
    r &= ~(0xFF << shift);

    /* Set prio for irq/sgi */
    r |= (prio << shift);

    GICR_WRITE_REG(GICR_IPRIORITYR + sgi/4, r);
}

void gicr_set_group0(uint32_t sgi)
{
    uint32_t r = GICR_READ_REG(GICR_IGROUPR0);
    r &= ~(1U << sgi);
    GICR_WRITE_REG(GICR_IGROUPR0, r);

    r = GICR_READ_REG(GICR_IGRPMODR0);
    r &= ~(1U << sgi);
    GICR_WRITE_REG(GICR_IGRPMODR0, r);
}

void gicr_enable(uint32_t sgi)
{
    uint32_t r = GICR_READ_REG(GICR_ISENABLER0);
    r |= (1U << sgi);
    GICR_WRITE_REG(GICR_ISENABLER0, r);
}

void gicr_disable(uint32_t sgi)
{
    uint32_t r = GICR_READ_REG(GICR_ICENABLER0);
    r |= (1U << sgi);
    GICR_WRITE_REG(GICR_ICENABLER0, r);
}

void gicr_set_sgi(uint64_t sgi)
{
    uint64_t r = (sgi << 24);

    /*
     * Leave Interrupt Routing Mode = 0 [bit 40], meaning target list routing
     * (page 30 in [1]).
     *
     * Target cpu 0.0.0.0 = bit 0 set */
    r |= 1;

    ICC_WRITE_REG(ICC_SGI0R_EL1, r);
}

static void gicv3_fiq_handler(struct excp_frame *f)
{
    uint32_t r;

    ICC_READ_REG(r, ICC_IAR0_EL1);
    printf("Fiq %d received\n", r);

    if (handler_t.h[r]) {
        handler_t.h[r](handler_t.opaque[r]);
    }
    ICC_WRITE_REG(ICC_EOIR0_EL1, r);
}

static void gicv3_irq_handler(struct excp_frame *f)
{
    uint32_t r;

    ICC_READ_REG(r, ICC_IAR1_EL1);

    if (handler_t.h[r]) {
        handler_t.h[r](handler_t.opaque[r]);
    }
    ICC_WRITE_REG(ICC_EOIR1_EL1, r);
}

void gicv3_register_handler()
{
    aarch64_set_fiq_h(gicv3_fiq_handler);
    aarch64_set_irq_h(gicv3_irq_handler);
}

void gicv3_set_handler(uint32_t irq, gicv3_handler h, void *opaque)
{
    assert(irq <= 256);
    handler_t.h[irq] = h;
    handler_t.opaque[irq] = opaque;
}

void configure_gicv3()
{
    gicd_config();
    gicr_config();
    cpuif_config();
}


