/*
 * Smoke testsuite for arm-gicv3.
 *
 * Copyright (C) 2016 Xilinx Inc
 * Written by Francisco Iglesias
 *
 * References:
 * [1] GICv3_prd03_genc_010745_19.0.pdf
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <byteswap.h>

#include "testcalls.h"
#include "sys.h"
#include "plat.h"
#include "arch-aarch64/gicv3.h"

#define ENABLE_LOGS 1

#if ENABLE_LOGS
#define LOG0(fmt, ...) \
do { printf(" - " fmt , ## __VA_ARGS__); } while (0)

#define LOG1(fmt, ...) \
do { printf("     * " fmt , ## __VA_ARGS__); } while (0)

#define LOG2(fmt, ...) \
do { printf(fmt , ## __VA_ARGS__); } while (0)

#else
#define LOG0(fmt, ...)
#define LOG1(fmt, ...)
#define LOG2(fmt, ...)
#endif

#define D(x)


#define N_SGI 16
#define SGI_4 4
#define PPI_9 (N_SGI + 9)
#define SPI_32 32

#define EL3 3

#define GROUP0 (0)
#define GROUP1 (1ULL << 60)

/*******************************************************/
/* Helper Functions */
static void fiq_h_for_spi_32(struct excp_frame *f)
{
    uint32_t r;

    ICC_READ_REG(r, ICC_IAR0_EL1);

    /* Clear interrupt by writing r to ICC_EOIR0_EL1 [1] */
    ICC_WRITE_REG(ICC_EOIR0_EL1, r);

    LOG1("GOT SPI %u\n", r);

    assert(r == SPI_32);
}

static void irq_h_for_spi_32(struct excp_frame *f)
{
    uint32_t r;

    ICC_READ_REG(r, ICC_IAR1_EL1);

    /* Clear interrupt by writing r to ICC_EOIR0_EL1 [1] */
    ICC_WRITE_REG(ICC_EOIR1_EL1, r);

    LOG1("GOT SPI %u\n", r);

    assert(r == SPI_32);
}

void setup_aarch64_ns_el1()
{
    unsigned int el;
    uint32_t scr;
    uint64_t hcr;

    LOG1("%s\n", __func__);

    aarch64_mrs(scr, "scr_el3");
    scr &= ~(SCR_FIQ);
    scr |= SCR_EA | SCR_RW | SCR_NS;
    aarch64_msr("scr_el3", scr);

    asm volatile ("msr\tsctlr_el2, %0\n" : : "r" (0));
    aarch64_drop_el(EL2, SPXH);
    el = aarch64_current_el();
    D(printf("2-EL=%d\n", el));
    assert(el == EL2);

    aarch64_mrs(hcr, "hcr_el2");
    hcr |= HCR_RW | HCR_AMO;
    aarch64_msr("hcr_el2", hcr);

    asm volatile ("msr\tsctlr_el1, %0\n" : : "r" (0));
    aarch64_drop_el(EL1, SPXH);
    el = aarch64_current_el();
    D(printf("1-EL=%d\n", el));
    assert(el == EL1);

    mb();
    __asm__ __volatile__ ("isb\n");
}

void setup_aarch64_ns_el2()
{
    unsigned int el;
    uint32_t scr;

    LOG1("%s\n", __func__);

    aarch64_mrs(scr, "scr_el3");
    scr &= ~(SCR_FIQ);
    scr |= SCR_EA | SCR_RW | SCR_NS;
    aarch64_msr("scr_el3", scr);

    asm volatile ("msr\tsctlr_el2, %0\n" : : "r" (0));
    aarch64_drop_el(EL2, SPXH);
    el = aarch64_current_el();
    D(printf("2-EL=%d\n", el));
    assert(el == EL2);

    mb();
    __asm__ __volatile__ ("isb\n");
}

void setup_aarch64_el3()
{
    unsigned int el;

    LOG1("%s\n", __func__);

    aarch64_raise_el(EL2);
    el = aarch64_current_el();
    D(printf("EL=%d\n", el));
    assert(el == EL2);

    aarch64_raise_el(EL3);
    el = aarch64_current_el();
    D(printf("EL=%d\n", el));
    assert(el == EL3);

    mb();
    __asm__ __volatile__ ("isb\n");
}

/******************************************************************************/
/* SPI tests */

void check_gicv3_spi_generation_from_gicd_as_irq()
{
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0x0);
    gicd_set_group1ns(SPI_32);
    gicd_enable(SPI_32);

    aarch64_set_irq_h(irq_h_for_spi_32);

    setup_aarch64_ns_el1();

    local_cpu_di();
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR1_EL1);
    LOG1("hppi = %u\n", hppi);
    assert(hppi == SPI_32);

    nr_irqs = aarch64_get_nr_irqs(EL1);

    local_cpu_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_irqs(EL1);
    local_cpu_di();

    assert(nr_irqs_2 > nr_irqs);

    aarch64_set_irq_h(NULL);

    setup_aarch64_el3();
}

void check_gicv3_disabled_spi_not_reaching_cpu_as_irq()
{
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0x0);
    gicd_set_group1ns(SPI_32);
    gicd_disable(SPI_32);

    aarch64_set_irq_h(irq_h_for_spi_32);

    setup_aarch64_ns_el1();

    local_cpu_di();
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR1_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_irqs = aarch64_get_nr_irqs(EL1);

    local_cpu_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_irqs(EL1);
    local_cpu_di();

    assert(nr_irqs_2 == nr_irqs);

    aarch64_set_irq_h(NULL);

    setup_aarch64_el3();
}

void check_gicv3_masking_spi_through_priority_as_irq()
{
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0xFF);
    gicd_set_group1ns(SPI_32);
    gicd_enable(SPI_32);

    aarch64_set_irq_h(irq_h_for_spi_32);

    setup_aarch64_ns_el1();

    local_cpu_di();
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR1_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_irqs = aarch64_get_nr_irqs(EL1);

    local_cpu_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_irqs(EL1);
    local_cpu_di();

    assert(nr_irqs_2 == nr_irqs);

    aarch64_set_irq_h(NULL);

    setup_aarch64_el3();
}

void check_gicv3_spi_generation_from_gicd_as_fiq()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0x0);
    gicd_set_group0(SPI_32);
    gicd_enable(SPI_32);

    aarch64_set_fiq_h(fiq_h_for_spi_32);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u\n", hppi);
    assert(hppi == SPI_32);

    nr_irqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_irqs_2 > nr_irqs);

    aarch64_set_fiq_h(NULL);
}

void check_gicv3_disabled_spi_not_reaching_cpu_as_fiq()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0x0);
    gicd_set_group0(SPI_32);
    gicd_disable(SPI_32);

    aarch64_set_fiq_h(fiq_h_for_spi_32);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_irqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_irqs_2 == nr_irqs);

    aarch64_set_fiq_h(NULL);
}

void check_gicv3_masking_spi_through_priority_as_fiq()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_irqs, nr_irqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure IRQ */
    gicd_set_routing(SPI_32);
    gicd_set_prio(SPI_32, 0xFF);
    gicd_set_group0(SPI_32);
    gicd_enable(SPI_32);

    aarch64_set_fiq_h(fiq_h_for_spi_32);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicd_set_irq(SPI_32);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_irqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_irqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_irqs_2 == nr_irqs);

    aarch64_set_fiq_h(NULL);
}

/******************************************************************************/
/* SGI tests */

static void fiq_h_for_sgi4(struct excp_frame *f)
{
    uint32_t r;

    ICC_READ_REG(r, ICC_IAR0_EL1);

    /* Clear interrupt by writing r to ICC_EOIR0_EL1 [1] */
    ICC_WRITE_REG(ICC_EOIR0_EL1, r);

    LOG1("GOT FIQ %u\n", r);

    assert(r == SGI_4);
}

void check_gicv3_masking_sgi_through_priority()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_fiqs, nr_fiqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure SGI */
    gicr_set_prio(SGI_4, 0xFF);
    gicr_set_group0(SGI_4);
    gicr_enable(SGI_4);

    aarch64_set_fiq_h(fiq_h_for_sgi4);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicr_set_sgi(SGI_4);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_fiqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_fiqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_fiqs_2 == nr_fiqs);

    aarch64_set_fiq_h(NULL);
}

void check_gicv3_disabled_sgi_not_reaching_cpu()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_fiqs, nr_fiqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure SGI */
    gicr_set_prio(SGI_4, 0);
    gicr_set_group0(SGI_4);
    gicr_disable(SGI_4);

    aarch64_set_fiq_h(fiq_h_for_sgi4);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicr_set_sgi(SGI_4);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u (1023 == no irq/sgi pending)\n", hppi);
    assert(hppi == 1023);

    nr_fiqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_fiqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_fiqs_2 == nr_fiqs);

    aarch64_set_fiq_h(NULL);
}

void check_gicv3_sgi()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_fiqs, nr_fiqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Configure SGI */
    gicr_set_prio(SGI_4, 0);
    gicr_set_group0(SGI_4);
    gicr_enable(SGI_4);

    aarch64_set_fiq_h(fiq_h_for_sgi4);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    gicr_set_sgi(SGI_4);
    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u\n", hppi);
    assert(hppi == 4);

    nr_fiqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_fiqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_fiqs_2 > nr_fiqs);

    aarch64_set_fiq_h(NULL);
}

static void fiq_h_for_ppi9(struct excp_frame *f)
{
	uint32_t r;

    ICC_READ_REG(r, ICC_IAR0_EL1);

    /* Clear interrupt by writing r to ICC_EOIR0_EL1 [1] */
    ICC_WRITE_REG(ICC_EOIR0_EL1, r);
    ICC_WRITE_REG(ICH_LR0_EL2, 0);

	LOG1("GOT FIQ %u\n", r);

    assert(r == PPI_9);
}

void check_gicv3_maintenance_irq()
{
    uint32_t scr;
    uint32_t hppi;
    unsigned int nr_fiqs, nr_fiqs_2;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    /* Enable PPI 9 (maint irq) */
    gicr_set_prio(PPI_9, 0);
    gicr_set_group0(PPI_9);
    gicr_enable(PPI_9);

    aarch64_set_fiq_h(fiq_h_for_ppi9);

    aarch64_mrs(scr, "scr_el3");
    scr |= SCR_FIQ;
    aarch64_msr("scr_el3", scr);
    mb();
    __asm__ __volatile__ ("isb\n");

    /* Enable vcpuif */
    ICC_WRITE_REG(ICH_HCR_EL2, 0x1);
    /* Generate maintanance irq */
    ICC_WRITE_REG(ICH_LR0_EL2, ICH_LR_EL2_EOI);

    mb();
    __asm__ __volatile__ ("isb\n");

    ICC_READ_REG(hppi, ICC_HPPIR0_EL1);
    LOG1("hppi = %u\n", hppi);
    assert(hppi == PPI_9);

    nr_fiqs = aarch64_get_nr_fiqs(EL3);

    local_cpu_fiq_ei();
    mb();
    __asm__ __volatile__ ("isb\n");
    udelay(60);

    nr_fiqs_2 = aarch64_get_nr_fiqs(EL3);
    local_cpu_fiq_di();

    assert(nr_fiqs_2 > nr_fiqs);

    aarch64_set_fiq_h(NULL);
}

void check_gicv3_virq()
{
    uint32_t elrsr;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    aarch64_set_fiq_h(fiq_h_for_ppi9);

    /* Enable vcpuif */
    ICC_WRITE_REG(ICH_HCR_EL2, 0x1);
    /* Enable group0 + group1 and priority mask 0xFF */
    ICC_WRITE_REG(ICH_VMCR_EL2, (0xFF << 24) | 0x3);

    /* Check no vfiq is set */
    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xf);

    /* Generate virq */
    ICC_WRITE_REG(ICH_LR0_EL2, ICH_LR_EL2_STATE_PEND | GROUP1 | 0x1); /* VIRQ */

    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xe);

    ICC_WRITE_REG(ICH_LR0_EL2, 0);
    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xf);
}

void check_gicv3_vfiq()
{
    uint32_t elrsr;

    LOG0("%s\n", __func__);
    local_cpu_fiq_di();

    aarch64_set_fiq_h(fiq_h_for_ppi9);

    /* Enable vcpuif */
    ICC_WRITE_REG(ICH_HCR_EL2, 0x1);
    /* Enable group0 + group1 and priority mask 0xFF */
    ICC_WRITE_REG(ICH_VMCR_EL2, (0xFF << 24) | 0x3);

    /* Check no vfiq is set */
    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xf);

    /* Generate vfiq */
    ICC_WRITE_REG(ICH_LR0_EL2, ICH_LR_EL2_STATE_PEND | GROUP0 | 0x1); /* VFIQ */

    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xe);

    ICC_WRITE_REG(ICH_LR0_EL2, 0);
    ICC_READ_REG(elrsr, ICH_ELRSR_EL2);
    LOG1("elrsr: 0x%x\n", elrsr);
    assert(elrsr == 0xf);
}

void check_gicv3()
{
    printf("%s\n", __func__);

    configure_gicv3();

    check_gicv3_sgi();
    check_gicv3_disabled_sgi_not_reaching_cpu();
    check_gicv3_masking_sgi_through_priority();

    check_gicv3_spi_generation_from_gicd_as_irq();
    check_gicv3_disabled_spi_not_reaching_cpu_as_irq();
    check_gicv3_masking_spi_through_priority_as_irq();

    check_gicv3_spi_generation_from_gicd_as_fiq();
    check_gicv3_disabled_spi_not_reaching_cpu_as_fiq();
    check_gicv3_masking_spi_through_priority_as_fiq();

    check_gicv3_maintenance_irq();
    check_gicv3_vfiq();
    check_gicv3_virq();
}

__testcall(check_gicv3);
