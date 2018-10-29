#define _MINIC_SOURCE

#include <stdbool.h>
#include <stdint.h>

#include "testcalls.h"
#include "sys.h"
#include "irq.h"
#include "test_entry.h"
#include "trace.h"
#include "switch.h"
#include "gic.h"
#include "handler.h"

#define VMCR_CTRL_MASK  0x0000021fUL

#define VMCR_ABPR_MASK  0x00000007UL
#define VMCR_ABPR_SHIFT 18
#define ABPR_SHIFT      0

#define VMCR_BPR_MASK   0x00000007UL
#define VMCR_BPR_SHIFT  21
#define BPR_SHIFT       0

#define VMCR_PMR_MASK   0x0000001fUL
#define VMCR_PMR_SHIFT  27
#define PMR_SHIFT       3

static inline void assert_aliased_vmcr(size_t reg, size_t reg_sh,
                                       uint32_t mask, size_t vmcr_sh)
{
    uint32_t target = (gicv_read(reg) >> reg_sh) & mask;
    uint32_t alias = (gich_read(GICH_VMCR) >> vmcr_sh) & mask;

    DPRINTF("target = %x, alias = %x, vmcr: %x\n", target, alias, gich_read(GICH_VMCR));
    assert(target == alias);
}

static inline void check_aliased_vmcr(size_t reg, size_t reg_sh,
                                      uint32_t mask, size_t vmcr_sh,
                                      uint32_t test_val)
{
    uint32_t vmcr;

    gicv_write(reg, test_val << reg_sh);
    assert_aliased_vmcr(reg, reg_sh, mask, vmcr_sh);

    gicv_write(reg, 0);
    assert_aliased_vmcr(reg, reg_sh, mask, vmcr_sh);

    vmcr = gich_read(GICH_VMCR) & ~(mask << vmcr_sh);
    gich_write(GICH_VMCR, vmcr | (test_val << vmcr_sh));
    assert_aliased_vmcr(reg, reg_sh, mask, vmcr_sh);

    vmcr = gich_read(GICH_VMCR) & ~(mask << vmcr_sh);
    gich_write(GICH_VMCR, vmcr);
    assert_aliased_vmcr(reg, reg_sh, mask, vmcr_sh);
}

static void test_gic_virt_vmcr(void)
{
    const uint32_t target_ctrl =
        (1 << 0)     /* EnableGrp0 */
        | (1 << 1)   /* EnableGrp1 */
        | (1 << 3)   /* FIQEn */
        | (1 << 4)   /* CBPR */
        | (1 << 9);  /* EOIMode */

    uint32_t ctrl, vmcr;

    gicv_write(GICC_CTRL, target_ctrl);

    ctrl = gicv_read(GICC_CTRL);
    assert(ctrl == target_ctrl);

    vmcr = gich_read(GICH_VMCR);

    /* Check aliased GICV_CTRL bits in GICH_VMCR */
    assert((ctrl & VMCR_CTRL_MASK) == (vmcr & VMCR_CTRL_MASK));

    gich_write(GICH_VMCR, vmcr & ~(VMCR_CTRL_MASK));

    vmcr = gich_read(GICH_VMCR);
    ctrl = gicv_read(GICC_CTRL);

    assert((vmcr & VMCR_CTRL_MASK) == 0);
    assert((ctrl & VMCR_CTRL_MASK) == 0);

    DPRINTF("aliased ABPR\n");
    check_aliased_vmcr(GICC_ABPR, ABPR_SHIFT, VMCR_ABPR_MASK, VMCR_ABPR_SHIFT, 5);
    DPRINTF("aliased BPR\n");
    check_aliased_vmcr(GICC_BPR, BPR_SHIFT, VMCR_BPR_MASK, VMCR_BPR_SHIFT, 4);
    DPRINTF("aliased PMR\n");
    check_aliased_vmcr(GICC_PMR, PMR_SHIFT, VMCR_PMR_MASK, VMCR_PMR_SHIFT, 0x15);
}


static void test_gic_virt_inject_virt_irq(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = 0,
        .virt_id = VIRT_TIMER_IRQ,
    };

    static const struct handler_action HYP_VIRT_IRQ_TEST_ACTIONS[] = {
        { timer_assert_el },    /* assert we are in the expected EL for this timer */
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_ack },          /* ack the timer */

        { token_assert_self, HA_PARAM(false) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },
        { token_set_self },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct handler_action VM_VIRT_IRQ_TEST_ACTIONS[] = {
        { timer_assert_el },    /* assert we are in the expected EL for this timer */
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,      /* Tick divisor based on a period of 1s
                                           (so here 0.5s) */
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = HYP_VIRT_IRQ_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
                .el = 1,
                .lvl = 0,
                .actions = VM_VIRT_IRQ_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_inject_phys_irq_noeoi(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = true,
        .grp1 = false,
        .prio = 0,
        .phys_id = PHYS_TIMER_IRQ,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action HYP_ACTIONS[] = {
        { timer_assert_el },    /* assert we are in the expected EL for this timer */
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_ack },          /* ack the timer */

        { token_assert_self, HA_PARAM(false) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },
        { token_set_self },

        { gic_eoi },            /* GIC end of interrupt */
        HA_END
    };

    static const struct handler_action VM_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,      /* Tick divisor based on a period of 1s */
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = HYP_ACTIONS,
                .vcpu_actions = VM_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}




static void test_gic_virt_inject_phys_irq_eoi(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = true,
        .grp1 = false,
        .prio = 0,
        .phys_id = PHYS_TIMER_IRQ,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action HYP_ACTIONS[] = {
        { timer_assert_el },    /* assert we are in the expected EL for this timer */
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_ack },          /* ack the timer */

        { token_assert_self, HA_PARAM(false) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },
        { token_set_self },

        { gic_eoi },            /* GIC end of interrupt */
        HA_END
    };

    static const struct handler_action VM_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,      /* Tick divisor based on a period of 1s */
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = HYP_ACTIONS,
                .vcpu_actions = VM_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_inject_phys_irq_different_ids(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = true,
        .grp1 = false,
        .prio = 0,
        .phys_id = PHYS_TIMER_IRQ,
        .virt_id = VIRT_TIMER_IRQ,
    };

    static const struct handler_action HYP_ACTIONS[] = {
        { timer_assert_el },    /* assert we are in the expected EL for this timer */
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_ack },          /* ack the timer */

        { token_assert_self, HA_PARAM(false) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },
        { token_set_self },

        { gic_eoi },            /* GIC end of interrupt */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct handler_action VM_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,      /* Tick divisor based on a period of 1s */
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = HYP_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
                .el = 1,
                .lvl = 0,
                .actions = VM_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_vgrp0d(void)
{
    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_VGRP0D) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_VGRP0D) },

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },
        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_VGRP0D) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_no_vgrp0d(void)
{
    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .vgrp0die = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}

static void test_gic_virt_maint_vgrp1d(void)
{
    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_VGRP1D) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_VGRP1D) },

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },
        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_VGRP1D) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_no_vgrp1d(void)
{
    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .vgrp1die = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = true,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_vgrp0e(void)
{
    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_VGRP0E) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_VGRP0E) },

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },
        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_VGRP0E) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_no_vgrp0e(void)
{
    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .vgrp0eie = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


static void test_gic_virt_maint_vgrp1e(void)
{
    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_VGRP1E) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_VGRP1E) },

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },
        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_VGRP1E) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = true,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}



static void test_gic_virt_maint_no_vgrp1e(void)
{
    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        { timer_rearm },        /* rearm the timer */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          ,  target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0          , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0          , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .vgrp1eie = true,
            },

            .vcpu = {
                .en_grp0 = false,
                .en_grp1 = false,
                .grp0_to_fiq = false,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .actions = TIMER_ACTIONS,
                .tick_divisor = 4,
                .el = 2,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


static void test_gic_virt_maint_np(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = 0,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_NP) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_NP) },

        { token_assert_false_lvl, HA_PARAM(0) },
        { token_set_lvl, HA_PARAM(0) },

        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { timer_ack },
        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_NP) },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .npie = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}

/*
 * This test checks the effect of writting an invalid vIRQ in V_EOIR when
 * EOIMode is false.
 *   - If V_RPR < 255, a prio drop should happend when writting to V_EOIR, and
 *     EOICount should be incremented,
 *   - If V_RPR == 255, no prio drop should happen and EOICount is not
 *     incremented.
 */
static void test_gic_virt_invalid_virq_no_eoimode(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = 0,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { virt_assert_eoicount, HA_PARAM(0) },

        { gic_assert_running_prio, HA_PARAM(0) },
        { gic_eoi_inexistant_irq }, /* Should trigger a prio drop */
        { gic_assert_running_prio, HA_PARAM(255) },

        { virt_assert_eoicount, HA_PARAM(1) },

        { gic_eoi_inexistant_irq }, /* Idle prio already, no more prio drop */
        { gic_assert_running_prio, HA_PARAM(255) },

        { virt_assert_eoicount, HA_PARAM(1) },  /* Should not have been incremented */

        { virt_reset_eoicount },
        { virt_assert_eoicount, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },

        { timer_ack },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


/*
 * This test checks the effect of writting an invalid vIRQ in V_EOIR and V_DIR
 * when EOIMode is true.
 *   - If V_RPR < 255, a prio drop should happend when writting to V_EOIR,
 *     EOICount should not be incremented,
 *   - If V_RPR == 255, no prio drop should happen, EOICount should not be
 *     incremented,
 *   - When writting to V_DIR, EOICount should be incremented.
 */
static void test_gic_virt_invalid_virq_eoimode(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = 0,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(true) },
        { token_reset_self },

        { virt_assert_eoicount, HA_PARAM(0) },

        { gic_assert_running_prio, HA_PARAM(0) },

        /*
         * First invalid vIRQ EOIR/DIR:
         *  - prio drop on EOIR
         *  - EOICount inc on DIR
         */
        { gic_eoi_inexistant_irq }, /* Should trigger a prio drop */
        { gic_assert_running_prio, HA_PARAM(255) },
        { virt_assert_eoicount, HA_PARAM(0) }, /* EOIMode true -> no inc */

        { gic_deactivate_inexistant_irq },
        { virt_assert_eoicount, HA_PARAM(1) },  /* DIR -> Should have been
                                                   incremented */

        /*
         * Second invalid vIRQ EOIR/DIR:
         *  - No effect on EOIR
         *  - EOICount inc on DIR
         */
        { gic_eoi_inexistant_irq }, /* Idle prio already, no more prio drop */
        { gic_assert_running_prio, HA_PARAM(255) },
        { virt_assert_eoicount, HA_PARAM(1) }, /* no inc */

        { gic_deactivate_inexistant_irq },
        { virt_assert_eoicount, HA_PARAM(2) },  /* DIR -> Should have been
                                                   incremented (even though no
                                                   prio drop happened) */

        { virt_reset_eoicount },
        { virt_assert_eoicount, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { token_assert_self, HA_PARAM(false) },
        { token_set_self },

        { timer_ack },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


static void test_gic_virt_maint_lrenp(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = 0,
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_LRENP) },

        { token_assert_true_lvl, HA_PARAM(0) },

        { virt_assert_eoicount, HA_PARAM(1) },
        { virt_reset_eoicount },

        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { token_assert_self, HA_PARAM(false) },
        { token_set_self },

        { gic_eoi_inexistant_irq },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { timer_ack },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
                .lrenpie = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
            },

            [TIMER_HYP] = {
                .enabled = false,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


static void test_gic_virt_maint_u(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQS[] = {
        [0] = {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = PHYS_TIMER_IRQ,
        },
        [1] = {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = VIRT_TIMER_IRQ,
        },
        [2] = {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = HYP_TIMER_IRQ,
        },
        [3] = {
            .hw = false,
            .grp1 = false,
            .prio = 1,
            .phys_id = 0,
            .virt_id = SEC_TIMER_IRQ,
        },
    };

    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_U) },
        { virt_disable_maint_irq, HA_PARAM(GICH_MISR_U) },

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { token_assert_self, HA_PARAM(true) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action VM_SEC_TEST_ACTIONS[] = {
        { token_assert_self, HA_PARAM(false) },
        { token_set_self },

        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_U) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { timer_ack },

        { token_assert_self, HA_PARAM(false) },

        { virt_inject_irq, HA_PARAM(INJECTED_IRQS) },
        { virt_inject_irq, HA_PARAM(INJECTED_IRQS+1) },
        { virt_inject_irq, HA_PARAM(INJECTED_IRQS+2) },
        { virt_inject_irq, HA_PARAM(INJECTED_IRQS+3) },

        { virt_enable_maint_irq, HA_PARAM(GICH_MISR_U) },
        { token_set_self },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
                .lvl = 0,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = false,
                .lvl = 0,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
                .lvl = 0,
                .vcpu_actions = VM_SEC_TEST_ACTIONS,
            },
        },
    };

    GIC_TEST(&INFO);
}

static void test_gic_virt_maint_eoi(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ = {
        .hw = false,
        .grp1 = false,
        .prio = 0,
        .phys_id = (1 << 9), /* EOI mantenance interrupt */
        .virt_id = PHYS_TIMER_IRQ,
    };

    static const struct handler_action MAINT_ACTIONS[] = {
        { virt_assert_maint_irq, HA_PARAM(GICH_MISR_EOI) },

        { virt_assert_eisr_entry, HA_PARAM(1 << 0) }, /* entry 0 should be set */
        { virt_clear_eoi_in_lr, HA_PARAM(0) },
        { virt_assert_eisr_entry, HA_PARAM(0) }, /* no entry should be set */

        { token_assert_true_lvl, HA_PARAM(0) },
        { token_reset_lvl, HA_PARAM(0) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { token_assert_self, HA_PARAM(true) },

        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_assert_lvl },   /* assert we are in the expected preemption level */
        { timer_assert_el },   /* assert we are in the expected preemption level */

        { timer_ack },

        { token_assert_self, HA_PARAM(false) },

        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ) },
        { token_set_self },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        { MAINTENANCE_IRQ, 0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = false,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .maintenance_irq_actions = MAINT_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 2,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
                .lvl = 0,
            },

            [TIMER_HYP] = {
                .enabled = false,
                .lvl = 0,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
                .lvl = 0,
            },
        },
    };

    GIC_TEST(&INFO);
}

/* This test checks that virtual interrupts with a low group priority get
 * preempted by higher ones.
 *
 * Low group priority handlers wait for their token to be set by higher ones.
 * If no preemption occur, then the test will be stuck forever.
 */
static void test_gic_virt_preempt(void)
{
    /* Those IRQs are in different priority groups */
    static const struct virt_inject_irq_params INJECTED_IRQ[] = {
        {
            .hw = false,
            .grp1 = false,
            .prio = 8,
            .phys_id = 0,
            .virt_id = PHYS_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 4,
            .phys_id = 0,
            .virt_id = VIRT_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = HYP_TIMER_IRQ,
        },
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { timer_assert_lvl },
        { token_reset_self },

        { nested_irq_enable },

        /* Wait for the current level token to be set by higher level timer.
         * Maximum level timer has its token always set and so does not wait
         * here. */
        { token_wait_self },

        { nested_irq_disable },

        { gic_eoi },
        { gic_deactivate_irq },

        /* This will unblock the parent */
        { token_set_parent },
        HA_END
    };

#define TIMER_ACTIONS(name, inject_idx)                            \
    static const struct handler_action name[] = {                  \
        { timer_ack },                                             \
                                                                   \
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[inject_idx]) },  \
                                                                   \
        { gic_eoi },                                               \
        { gic_deactivate_irq },                                    \
        { timer_set_divisor, HA_PARAM(1) },                        \
        { timer_rearm },                                           \
        HA_END                                                     \
    }

    TIMER_ACTIONS(PHYS_ACTIONS, 0);
    TIMER_ACTIONS(VIRT_ACTIONS, 1);
    TIMER_ACTIONS(HYP_ACTIONS, 2);
#undef TIMER_ACTIONS

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 0     , 2         , 0        },
        { VIRT_TIMER_IRQ, 0          , 0     , 1         , 0        },
        { HYP_TIMER_IRQ,  0          , 0     , 0         , 0        },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .lvl = 0,
                .el = 3,
                .actions = PHYS_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 3,
                .lvl = 1,
                .el = 3,
                .actions = VIRT_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = true,
                .tick_divisor = 2,
                .lvl = 2,
                .el = 3,
                .actions = HYP_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}


/* This test checks that virtual interrupts in the same priority group do not
 * preempt one another
 *
 * If a timer incorrectly preempts another one, the nested IRQ level will be
 * greater than 0 and the test will fail.
 */
static void test_gic_virt_no_preempt(void)
{
    /* Those IRQs are in the same priority group */
    static const struct virt_inject_irq_params INJECTED_IRQ[] = {
        {
            .hw = false,
            .grp1 = false,
            .prio = 2,
            .phys_id = 0,
            .virt_id = PHYS_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 1,
            .phys_id = 0,
            .virt_id = VIRT_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = HYP_TIMER_IRQ,
        },
    };

    static const struct handler_action VM_TEST_ACTIONS[] = {
        { timer_assert_lvl },

        { nested_irq_enable },
        { wait_us, HA_PARAM(300) },
        { nested_irq_disable },

        { gic_eoi },
        { gic_deactivate_irq },
        HA_END
    };

#define TIMER_ACTIONS(name, inject_idx)                            \
    static const struct handler_action name[] = {                  \
        { timer_ack },                                             \
                                                                   \
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[inject_idx]) },  \
                                                                   \
        { gic_eoi },                                               \
        { gic_deactivate_irq },                                    \
        { timer_set_divisor, HA_PARAM(1) },                        \
        { timer_rearm },                                           \
        HA_END                                                     \
    }

    TIMER_ACTIONS(PHYS_ACTIONS, 0);
    TIMER_ACTIONS(VIRT_ACTIONS, 1);
    TIMER_ACTIONS(HYP_ACTIONS,  2);
#undef TIMER_ACTIONS

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ, 0          , 0     , 2         , 0        },
        { VIRT_TIMER_IRQ, 0          , 0     , 1         , 0        },
        { HYP_TIMER_IRQ,  0          , 0     , 0         , 0        },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .lvl = 0,
                .el = 3,
                .actions = PHYS_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 3,
                .lvl = 0,
                .el = 3,
                .actions = VIRT_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = true,
                .tick_divisor = 2,
                .lvl = 0,
                .el = 3,
                .actions = HYP_ACTIONS,
                .vcpu_actions = VM_TEST_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}

/* This test checks that preemption occurs on priority drop for virtual
 * interrupts */
static void test_gic_virt_prio_drop(void)
{
    static const struct virt_inject_irq_params INJECTED_IRQ[] = {
        {
            .hw = false,
            .grp1 = false,
            .prio = 3,
            .phys_id = 0,
            .virt_id = PHYS_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 2,
            .phys_id = 0,
            .virt_id = VIRT_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 1,
            .phys_id = 0,
            .virt_id = HYP_TIMER_IRQ,
        },

        {
            .hw = false,
            .grp1 = false,
            .prio = 0,
            .phys_id = 0,
            .virt_id = SEC_TIMER_IRQ,
        },
    };

    static const struct handler_action VM_ACTIONS[] = {
        { timer_assert_lvl },
        { timer_assert_el },
        { token_reset_self },

        { nested_irq_enable },

        { token_assert_self, HA_PARAM(false) },
        { gic_eoi },
        /* Preemption should occur here */
        { token_assert_self, HA_PARAM(true) },

        { token_set_parent },

        { nested_irq_disable },
        { gic_deactivate_irq },
        HA_END
    };

    static const struct handler_action VM_LAST_LVL_ACTIONS[] = {
        { timer_assert_lvl },
        { timer_assert_el },

        { token_set_parent },

        { gic_eoi },
        { gic_deactivate_irq },
        HA_END
    };


    static const struct handler_action TIMER_ACTIONS[] = {
        { timer_ack },

        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[0]) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[1]) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[2]) },
        { virt_inject_irq, HA_PARAM(&INJECTED_IRQ[3]) },

        { timer_rearm },
        { gic_eoi },            /* GIC end of interrupt */
        { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
        HA_END
    };

    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { PHYS_TIMER_IRQ,  0         , 1     , 0         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .virt_mode = true,

        .gic = {
            .irqs = GIC_IRQ_INFO,
            .cpu = {
                .en_grp0 = true,
                .en_grp1 = true,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },

            .virt = {
                .en = true,
            },

            .vcpu = {
                .en_grp0 = true,
                .en_grp1 = false,
                .grp0_to_fiq = true,
                .eoi_mode = true,
                .cbpr = false,
                .bpr = 4,
                .abpr = 5,
            },
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 4,
                .el = 1,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 3,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
                .actions = TIMER_ACTIONS,
                .vcpu_actions = VM_LAST_LVL_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = false,
                .lvl = 2,
                .el = 1,
                .vcpu_actions = VM_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = false,
                .lvl = 1,
                .el = 1,
                .vcpu_actions = VM_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
                .lvl = 0,
                .el = 1,
                .vcpu_actions = VM_ACTIONS,
            },
        },
    };

    GIC_TEST(&INFO);
}

__testcall(test_gic_virt_inject_virt_irq);
__testcall(test_gic_virt_inject_phys_irq_noeoi);
__testcall(test_gic_virt_inject_phys_irq_eoi);
__testcall(test_gic_virt_inject_phys_irq_different_ids);
__testcall(test_gic_virt_invalid_virq_no_eoimode);
__testcall(test_gic_virt_invalid_virq_eoimode);
__testcall(test_gic_virt_maint_vgrp0d);
__testcall(test_gic_virt_maint_no_vgrp0d);
__testcall(test_gic_virt_maint_vgrp1d);
__testcall(test_gic_virt_maint_no_vgrp1d);
__testcall(test_gic_virt_maint_vgrp0e);
__testcall(test_gic_virt_maint_no_vgrp0e);
__testcall(test_gic_virt_maint_vgrp1e);
__testcall(test_gic_virt_maint_no_vgrp1e);
__testcall(test_gic_virt_maint_np);
__testcall(test_gic_virt_maint_lrenp);
__testcall(test_gic_virt_maint_u);
__testcall(test_gic_virt_maint_eoi);
__testcall(test_gic_virt_vmcr);
__testcall(test_gic_virt_preempt);
__testcall(test_gic_virt_no_preempt);
__testcall(test_gic_virt_prio_drop);
