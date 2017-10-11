#include "gic.h"

/* Does not take into account the implementation specific max priority value.
 * Rely on the fact that bits [7:4] are always implemented */
static void gic_irq_set_prio(int irq, unsigned int group,
                             unsigned int subgroup, unsigned int subprio)
{
    uint8_t prio;

    assert(group < 2);
    assert(subgroup < 4);
    assert(subprio < 4);

    prio = ((subgroup & 0x7) << 1) | (subprio & 0x1);
    prio <<= 4;

    /* Always set group 1 IRQs to have a lower prio */
    prio &= 0x7f;
    prio |= group << 7;

    gicd_set_irq_prio(GIC_DIST_BASE, irq, prio);
}

static void gic_configure_irq(const struct gic_irq_info *irq)
{
    gicd_set_irq_group(GIC_DIST_BASE, irq->irq, irq->group);
    gicd_set_irq_target(GIC_DIST_BASE, irq->irq, irq->cpu_target);
    gic_irq_set_prio(irq->irq, irq->group, irq->subgroup, irq->subprio);
}

void gic_configure(const struct gic_info *info)
{
    const struct gic_irq_info *irqs = info->irqs;
    uint32_t c_ctrl = 0;

    c_ctrl |= info->en_grp0 << 0;
    c_ctrl |= info->en_grp1 << 1;
    c_ctrl |= info->grp0_to_fiq << 3;
    c_ctrl |= info->eoi_mode << 9;  /* EOImodeS */
    c_ctrl |= info->eoi_mode << 10; /* EOImodeNS */

    writel(GIC_DIST_BASE + GICD_CTRL, 3);
    writel(GIC_CPU_BASE + GICC_CTRL, c_ctrl);
    writel(GIC_CPU_BASE + GICC_PMR, 0xff);

    /* subgroup: [7:5], subprio: [4:0] */
    writel(GIC_CPU_BASE + GICC_BPR, 4);
    writel(GIC_CPU_BASE + GICC_ABPR, 4);

    while (irqs->irq != -1) {
        gic_configure_irq(irqs);
        irqs++;
    }
}
