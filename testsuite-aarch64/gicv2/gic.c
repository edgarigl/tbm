#include "gic.h"
#include "trace.h"

static inline int ctz32(uint32_t val)
{
    return val ? __builtin_ctz(val) : 32;
}

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

    if ((!irq->group) || info->cbpr) {
        bpr = info->bpr + 1;
    } else {
        bpr = info->abpr;
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

void gic_configure(const struct gic_info *info)
{
    const struct gic_irq_info *irqs = info->irqs;
    uint32_t c_ctrl = 0;
    int prio_bits_num;

    prio_bits_num = get_prio_bits_num();
    DPRINTF("implemented prio bits: %d\n", prio_bits_num);

    c_ctrl |= info->en_grp0 << 0;
    c_ctrl |= info->en_grp1 << 1;
    c_ctrl |= info->grp0_to_fiq << 3;
    c_ctrl |= info->cbpr << 4;
    c_ctrl |= info->eoi_mode << 9;  /* EOImodeS */
    c_ctrl |= info->eoi_mode << 10; /* EOImodeNS */

    writel(GIC_DIST_BASE + GICD_CTRL, 3);
    writel(GIC_CPU_BASE + GICC_CTRL, c_ctrl);
    writel(GIC_CPU_BASE + GICC_PMR, 0xff);

    writel(GIC_CPU_BASE + GICC_BPR, info->bpr);

    if (info->en_grp1 && !info->cbpr) {
        assert(info->abpr > 0);
        writel(GIC_CPU_BASE + GICC_ABPR, info->abpr);
    }

    mb();

    while (irqs->irq != -1) {
        gic_configure_irq(info, irqs, prio_bits_num);
        irqs++;
    }
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
