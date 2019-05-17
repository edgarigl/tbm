/*
 * ARM GICv2 test suite
 *
 * Copyright (c) 2018-2019 Luc Michel <luc.michel@greensocs.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#include "gic.h"
#include "trace.h"
#include "misc.h"

/* Returns the number of implemented priority bits in the GIC */
static int get_prio_bits_num(void)
{
    uint8_t res;

    gicd_set_irq_prio(GIC_DIST_BASE, 0, 0xff);
    mb();

    res = gicd_get_irq_prio(GIC_DIST_BASE, 0);

    gicd_set_irq_prio(GIC_DIST_BASE, 0, 0);
    mb();

    return 8 - ctz32(res);
}


static void gic_irq_set_prio(const struct gic_info *info,
                             const struct gic_irq_info *irq,
                             int prio_bits_num)
{
    uint8_t prio;
    int bpr;

    assert(irq->group < 2);

    if ((!irq->group) || info->cpu.cbpr) {
        bpr = info->cpu.bpr + 1;
    } else {
        bpr = info->cpu.abpr;
    }

    assert(irq->subgroup < (1 << (8-bpr)));
    assert(irq->subprio < (1 << bpr));

    prio = (irq->subgroup << bpr) | (irq->subprio << (8 - prio_bits_num));

    /* Always set irq->group 1 IRQs to have a lower prio */
    prio &= 0x7f;
    prio |= irq->group << 7;

    DPRINTF("irq %d, set prio %x\n", irq->irq, prio);
    gicd_set_irq_prio(GIC_DIST_BASE, irq->irq, prio);
}

static void gic_configure_irq(const struct gic_info *info,
                              const struct gic_irq_info *irq,
                              int prio_bits_num)
{
    gicd_set_irq_group(GIC_DIST_BASE, irq->irq, irq->group);
    gicd_set_irq_target(GIC_DIST_BASE, irq->irq, irq->cpu_target);
    gic_irq_set_prio(info, irq, prio_bits_num);
    gicd_enable_irq(GIC_DIST_BASE, irq->irq);
    mb();
}

static void gic_disable_irq(const struct gic_irq_info *irq)
{
    gicd_disable_irq(GIC_DIST_BASE, irq->irq);
    mb();
}

static void configure_dist(const struct gic_info *info)
{
    int prio_bits_num;
    const struct gic_irq_info *irqs = info->irqs;

    prio_bits_num = get_prio_bits_num();
    DPRINTF("implemented prio bits: %d\n", prio_bits_num);

    writel(GIC_DIST_BASE + GICD_CTRL, 3);

    while (irqs->irq != -1) {
        gic_configure_irq(info, irqs, prio_bits_num);
        irqs++;
    }

    mb();
}

static void configure_cpu_iface(const struct gic_cpu_info *info, phys_addr_t base)
{
    uint32_t c_ctrl = 0;

    c_ctrl |= info->en_grp0 << 0;
    c_ctrl |= info->en_grp1 << 1;
    c_ctrl |= info->grp0_to_fiq << 3;
    c_ctrl |= info->cbpr << 4;
    c_ctrl |= info->eoi_mode << 9;  /* EOImodeS */
    c_ctrl |= info->eoi_mode << 10; /* EOImodeNS */

    writel(base + GICC_CTRL, c_ctrl);
    writel(base + GICC_PMR, 0xff);

    writel(base + GICC_BPR, info->bpr);

    if (info->en_grp1 && !info->cbpr) {
        assert(info->abpr > 0);
        writel(base + GICC_ABPR, info->abpr);
    }

    mb();
}

#ifdef GIC_VIFACE_BASE
static bool configure_virt(const struct gic_virt_info *info)
{
    uint32_t h_hcr = 0;

    h_hcr |= info->en << 0;
    h_hcr |= info->uie << 1;
    h_hcr |= info->lrenpie << 2;
    h_hcr |= info->npie << 3;
    h_hcr |= info->vgrp0eie << 4;
    h_hcr |= info->vgrp0die << 5;
    h_hcr |= info->vgrp1eie << 6;
    h_hcr |= info->vgrp1die << 7;

    writel(GIC_VIFACE_BASE + GICH_HCR, h_hcr);

    return info->en;
}
#endif

void gic_configure(const struct gic_info *info)
{
    configure_dist(info);
    configure_cpu_iface(&info->cpu, GIC_CPU_BASE);

#ifdef GIC_VIFACE_BASE
    if (configure_virt(&info->virt)) {
        configure_cpu_iface(&info->vcpu, GIC_VCPU_BASE);
    }
#endif
}

void gic_teardown(const struct gic_info *info)
{
    const struct gic_irq_info *irqs = info->irqs;

    while (irqs->irq != -1) {
        gic_disable_irq(irqs);
        irqs++;
    }

    writel(GIC_CPU_BASE + GICC_CTRL, 0);
    writel(GIC_DIST_BASE + GICD_CTRL, 0);
    mb();
}
