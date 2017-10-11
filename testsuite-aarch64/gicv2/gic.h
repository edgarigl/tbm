#ifndef _TESTSUITE_VEXPRESS_GIC_H
#define _TESTSUITE_VEXPRESS_GIC_H

#include <stdint.h>
#include <stdbool.h>

#include "sys.h"
#include "drivers/arm/gic.h"

struct gic_irq_info {
    int irq;
    int cpu_target;
    unsigned int group;
    unsigned int subgroup;
    unsigned int subprio;
};

#define GIC_IRQ_INFO_END { -1, 0, 0, 0, 0 }

struct gic_info {
    const struct gic_irq_info *irqs;

    bool en_grp0;
    bool en_grp1;
    bool grp0_to_fiq;
    bool eoi_mode;
};

static inline void gic_end_of_irq(uint32_t irq)
{
    writel(GIC_CPU_BASE + GICC_EOIR, irq);
}

static inline uint32_t gic_ack_irq(void)
{
    return readl(GIC_CPU_BASE + GICC_IAR);
}

static inline uint32_t gic_running_prio(void)
{
    return readl(GIC_CPU_BASE + GICC_RPR);
}

void gic_configure(const struct gic_info *info);

#endif
