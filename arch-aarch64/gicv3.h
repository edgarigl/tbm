/*
 * GICV3 Header
 *
 * Copyright (C) 2016 Xilinx Inc
 * Written by Francisco Iglesias
 *
 * References:
 * [1] GICv3_prd03_genc_010745_19.0.pdf
 */
#ifndef _GICV3_H_
#define _GICV3_H_

#define GICD_BASE 0xF9000000
#define GICR_BASE 0xF9080000

#define GICD_CTLR            0x0000
#define GICD_TYPER           0x0004
#define GICD_IIDR            0x0008
#define GICD_STATUSR         0x0010
#define GICD_SETSPI_NSR      0x0040
#define GICD_CLRSPI_NSR      0x0048
#define GICD_SETSPI_SR       0x0050
#define GICD_CLRSPI_SR       0x0058
#define GICD_SEIR            0x0068
#define GICD_IGROUPR         0x0080
#define GICD_ISENABLER       0x0100
#define GICD_ICENABLER       0x0180
#define GICD_ISPENDR         0x0200
#define GICD_ICPENDR         0x0280
#define GICD_ISACTIVER       0x0300
#define GICD_ICACTIVER       0x0380
#define GICD_IPRIORITYR      0x0400
#define GICD_ITARGETSR       0x0800
#define GICD_ICFGR           0x0C00
#define GICD_IGRPMODR        0x0D00
#define GICD_NSACR           0x0E00
#define GICD_SGIR            0x0F00
#define GICD_CPENDSGIR       0x0F10
#define GICD_SPENDSGIR       0x0F20
#define GICD_IROUTER         0x6000
#define GICD_IDREGS          0xFFD0

#define GICD_CTLR_ARE_S             (1U << 4)
#define GICD_CTLR_ARE_NS            (1U << 5)
#define GICD_CTLR_EN_GRP0           (1U << 0)
#define GICD_CTLR_EN_GRP1NS         (1U << 1)
#define GICD_CTLR_EN_GRP1S          (1U << 2)
#define GICD_CTLR_RWP               (1U << 31)

#define GICR_CTLR             0x0000
#define GICR_IIDR             0x0004
#define GICR_TYPER            0x0008
#define GICR_STATUSR          0x0010
#define GICR_WAKER            0x0014
#define GICR_SETLPIR          0x0040
#define GICR_CLRLPIR          0x0048
#define GICR_PROPBASER        0x0070
#define GICR_PENDBASER        0x0078
#define GICR_INVLPIR          0x00A0
#define GICR_INVALLR          0x00B0
#define GICR_SYNCR            0x00C0
#define GICR_IDREGS           0xFFD0

#define GICR_WAKER_ProcessorSleep    (1U << 1)
#define GICR_WAKER_ChildrenAsleep    (1U << 2)


#define GICR_SGI_OFFSET 0x10000

#define GICR_IGROUPR0         (GICR_SGI_OFFSET + 0x0080)
#define GICR_ISENABLER0       (GICR_SGI_OFFSET + 0x0100)
#define GICR_ICENABLER0       (GICR_SGI_OFFSET + 0x0180)
#define GICR_ISPENDR0         (GICR_SGI_OFFSET + 0x0200)
#define GICR_ICPENDR0         (GICR_SGI_OFFSET + 0x0280)
#define GICR_ISACTIVER0       (GICR_SGI_OFFSET + 0x0300)
#define GICR_ICACTIVER0       (GICR_SGI_OFFSET + 0x0380)
#define GICR_IPRIORITYR       (GICR_SGI_OFFSET + 0x0400)
#define GICR_ICFGR0           (GICR_SGI_OFFSET + 0x0C00)
#define GICR_ICFGR1           (GICR_SGI_OFFSET + 0x0C04)
#define GICR_IGRPMODR0        (GICR_SGI_OFFSET + 0x0D00)
#define GICR_NSACR            (GICR_SGI_OFFSET + 0x0E00)

#define GICD_READ_REG(reg) (readl(GICD_BASE + (reg)))
#define GICD_WRITE_REG(reg, val) (writel(GICD_BASE + (reg), val));

#define GICR_READ_REG(reg) (readl(GICR_BASE + (reg)))
#define GICR_WRITE_REG(reg, val) (writel(GICR_BASE + (reg), val));

#define ICC_SRE_EL1     "S3_0_C12_C12_5"
#define ICC_SRE_EL2     "S3_4_C12_C9_5"
#define ICC_SRE_EL3     "S3_6_C12_C12_5"
#define ICC_CTLR_EL3    "S3_6_C12_C12_4"
#define ICC_PMR_EL1     "S3_0_C4_C6_0"
#define ICC_CTLR_EL1    "S3_0_C12_C12_4"
#define ICC_CTLR_EL3    "S3_6_C12_C12_4"
#define ICC_IGRPEN0_EL1 "S3_0_C12_C12_6"
#define ICC_IGRPEN1_EL1 "S3_0_C12_C12_7"
#define ICC_SGI0R_EL1   "S3_0_C12_C11_7"
#define ICC_EOIR0_EL1   "S3_0_C12_C8_1"
#define ICC_EOIR1_EL1   "S3_0_C12_C12_1"
#define ICC_IAR0_EL1    "S3_0_C12_C8_0"
#define ICC_IAR1_EL1    "S3_0_C12_C12_0"
#define ICC_HPPIR0_EL1  "S3_0_C12_C8_2"
#define ICC_HPPIR1_EL1  "S3_0_C12_C12_2"

#define ICC_CTLR_EL3_EOIMODE_EL3 (1U << 2)
#define ICC_CTLR_EL3_EOIMODE_EL1S (1U << 3)
#define ICC_CTLR_EL3_EOIMODE_EL1NS (1U << 4)
#define ICC_CTLR_EL3_EOIMODE_ALL \
(ICC_CTLR_EL3_EOIMODE_EL3 | ICC_CTLR_EL3_EOIMODE_EL1S | ICC_CTLR_EL3_EOIMODE_EL1NS)


#define ICH_HCR_EL2     "S3_4_C12_C11_0"
#define ICH_LR0_EL2     "S3_4_C12_C12_0"
#define ICH_MISR_EL2    "S3_4_C12_C11_2"
#define ICH_VMCR_EL2    "S3_4_C12_C11_7"
#define ICH_ELRSR_EL2   "S3_4_C12_C11_5"

#define ICH_LR_EL2_EOI  (1ULL << 41)
#define ICH_LR_EL2_STATE_SHIFT 62
#define ICH_LR_EL2_STATE_PEND  (1ULL << ICH_LR_EL2_STATE_SHIFT)

#define ICC_READ_REG(V, SR) \
__asm__ __volatile__ ("mrs\t%0," SR "\n" : "=r" (V));

#define ICC_WRITE_REG(SR, V) \
__asm__ __volatile__ ("msr\t" SR ", %0\n" : : "r" (V))

typedef void (*gicv3_handler)(void *opaque);

void gicd_wait_for(uint32_t reg, uint32_t mask);
void gicr_wait_for(uint32_t reg, uint32_t mask);
void gicr_wait_for(uint32_t reg, uint32_t mask);
void gicd_config();
void gicr_config();
void cpuif_config();
void gicd_set_routing(uint32_t irq);
void gicd_set_prio(uint32_t irq, uint32_t prio);
void gicd_set_group0(uint32_t irq);
void gicd_set_group1ns(uint32_t irq);
void gicd_enable(uint32_t irq);
void gicd_disable(uint32_t irq);
void gicd_set_irq(uint64_t irq);
void gicr_set_prio(uint32_t sgi, uint32_t prio);
void gicr_set_group0(uint32_t sgi);
void gicr_enable(uint32_t sgi);
void gicr_disable(uint32_t sgi);
void gicr_set_sgi(uint64_t sgi);
void gicv3_register_handler();
void gicv3_set_handler(uint32_t irq, gicv3_handler h, void *opaque);
void configure_gicv3();
#endif
