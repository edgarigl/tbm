#include "testcalls.h"

#include "gicv2/test_entry.h"
#include "gicv2/handler.h"
#include "gicv2/irq.h"
#include "gicv2/test_bpr.h"

/*
 * Handler actions for preemption tests.
 *
 * Each preemption level waits for its token to be set by higher level.
 * Once the token is set, it rearms the timer and set the token for the lower level.
 * Each preemption level is checked against the exepected level (timer_assert_lvl).
 * If preemption does not occur, previous level will wait indefinitly for its
 * token and the test won't end.
 */
static const struct handler_action PREEMPT_TEST_ACTIONS[] = {
    { timer_assert_el },    /* assert we are in the expected EL for this timer */
    { timer_assert_lvl },   /* assert we are in the expected preemption level */
    { token_reset_self },   /* reset our token */
    { timer_ack },          /* ack the timer */

    { nested_irq_enable },  /* enable nested IRQs to allow preemption */

    { token_wait_self },    /* wfi until our token has been set by higher prio handler */

    { nested_irq_disable }, /* disable nested IRQs */

    { gic_eoi },            /* GIC end of interrupt */
    { gic_deactivate_irq }, /* GIC deactivate IRQ (only if eoi_mode is true) */
    { timer_rearm },        /* rearm the timer */
    { token_set_parent },   /* set the parent token to unblock the previous lvl IRQ waiting for us */
    HA_END
};

/*
 * Handler actions for tests were no preemption should occur.
 *
 * Instead of waiting for its token to be set, the current level will wait 1s,
 * duration supposed to be enough for higher priority timers to trigger.
 * If preemption occurs incorrectly, preemption level will be incorrect and
 * timer_assert_lvl will fail.
 */
static const struct handler_action NO_PREEMPT_TEST_ACTIONS[] = {
    { timer_assert_el },            /* assert we are in the expected EL for this timer */
    { timer_assert_lvl },           /* assert we are in the expected preemption level */
    { timer_ack },                  /* ack the timer */

    { nested_irq_enable },          /* enable nested IRQs to allow preemption */

    { wait_us, HA_PARAM(1000) },    /* wait 1s */

    { nested_irq_disable },         /* disable nested IRQs */

    { gic_eoi },                    /* GIC end of interrupt */
    { gic_deactivate_irq },         /* GIC deactivate IRQ (only if eoi_mode is true) */
    { timer_rearm },                /* rearm the timer */
    HA_END
};


/*
 * Handler actions for priority drop tests.
 *
 * The two following set of actions are used for priority drop tests.
 * Those tests enables the EIOmode flag in GICC_CTRL register that separates
 * "End of interrupt" from "Interrupt deactivation" in the GIC.
 *
 * On "End of interrupt", priority drop occurs and the CPU iface running
 * priority is set to the next higher priority pending interrupt. If such a
 * interrupt is pending when priority drop occurs, then the current IRQ should
 * be preempted.
 *
 * To test that, we set a high priority timer to trigger first. It waits 1s to
 * allow a lower priority timer to trigger also. Since it's lower prio, it
 * won't preempt the high prio one. However on "End of interrupt", we expect to
 * be immediatly preempted. This is ensured using the tokens.
 */

/* Actions for the high priority timers */
static const struct handler_action PRIO_DROP_HI_PRIO_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { token_reset_self },
    { timer_ack },

    { nested_irq_enable },

    { wait_us, HA_PARAM(1000) },        /* Let the time to low prio timer to trigger */
    { token_assert, HA_PARAM(false) },  /* ensure we have not been preempted yet */
    { gic_eoi },                        /* priority drop */
    { token_assert, HA_PARAM(true) },   /* ensure we have been preempted on prio drop */

    { nested_irq_disable },

    { timer_rearm },
    { gic_deactivate_irq },
    { token_set_parent },
    HA_END
};

/* Actions for the low priority timer */
static const struct handler_action PRIO_DROP_LO_PRIO_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { timer_ack },

    { token_set_parent },

    { gic_eoi },
    { timer_rearm },
    { gic_deactivate_irq },
    HA_END
};

/*
 * Handler actions for the sub-priority test
 *
 * We test sub-priority by setting three timers, one in a low group priority,
 * the two others in a higher group priority, one having a sub-priority higher
 * than the other:
 *   - The first to trigger is the low group priority and waits 1s in its
 *     handler, before enabling interrupts. This wait allows the two others to
 *     trigger. When it enable interrupts, it got preempted.
 *   - The second one should be the higher priority one and checks so by
 *     ensuring that the parent token is not set yet.
 *   - The third one set the parent token.
 */

/* First, low-priority group timer */
static const struct handler_action SUB_PRIO_1ST_TIMER_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { token_reset_self },
    { timer_ack },

    { wait_us, HA_PARAM(1000) },    /* Let the time to higher prio timers to trigger */
    { nested_irq_enable },          /* enable nested IRQs to allow preemption */

    { token_wait_self },            /* wfi until our token as been set by higher prio handler */

    { nested_irq_disable },

    { gic_eoi },
    { gic_deactivate_irq },
    { timer_rearm },
    HA_END
};

/* Second, high-priority group, high-subpriority timer */
static const struct handler_action SUB_PRIO_2ND_TIMER_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { timer_ack },

    { nested_irq_enable },

    { token_assert_parent, HA_PARAM(false) },  /* ensure we are serviced before the third timer */

    { nested_irq_disable },

    { gic_eoi },
    { gic_deactivate_irq },
    { timer_rearm },
    { token_set_parent },
    HA_END
};

/* Third, high-priority group, low-subpriority timer */
static const struct handler_action SUB_PRIO_3RD_TIMER_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { timer_ack },

    { nested_irq_enable },

    { token_set_parent },   /* set parent token */

    { nested_irq_disable },

    { gic_eoi },
    { gic_deactivate_irq },
    { timer_rearm },
    { token_set_parent },
    HA_END
};


/*
 * Handler actions for the "active pending" IRQ test
 *
 * This test ends the IRQ and immediatly rearm the timer. It then waits for the
 * timer to trigger. The GIC should not signal the IRQ since it is already
 * active and has not been deactivated yet.
 */
static const struct handler_action ACTIVE_PENDING_TEST_ACTIONS[] = {
    { timer_assert_el },
    { timer_assert_lvl },
    { timer_ack },

    { nested_irq_enable },

    { gic_eoi },
    { timer_rearm },

    /* Wait for the timer to trigger again. The corresponding IRQ will become
     * "active and pending" (because we haven't deactivated it yet). IRQs in
     * this state should not be signaled to the CPU. If it is incorrectly
     * signaled, the timer_assert_lvl test will fail. */
    { wait_us, HA_PARAM(1000) },

    { nested_irq_disable },

    { timer_ack },
    { timer_rearm },
    { gic_deactivate_irq },
    HA_END
};


/*
 * Premption test in group 0
 *
 * This tests uses two timers in group 0. A lower group priority one that get
 * preempted by the second one.
 */
static void test_gic_preempt_subgroup_in_grp0(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        /* IRQ          , target_cpu , group , sub-group , sub-prio */
        { VIRT_TIMER_IRQ, 0          , 0     , 0         ,0         },
        { PHYS_TIMER_IRQ, 0          , 0     , 1         ,0         },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,        /* Enable distribution of Group0 interrupts */
            .en_grp1 = false,       /* Disable distribution of Group1 interrupts */
            .grp0_to_fiq = true,    /* Redirect Group0 IRQs to FIQ line */
            .eoi_mode = false,      /* EOImode and EOImodeNS disabled */
            .cbpr = false,          /* CBPR bit in C_CTLR (use ABPR for group1 IRQs when false,
                                       BPR otherwise) */
            .bpr = 4,               /* Binary Point Register */
            .abpr = 5,              /* Aliased BPR (for group1 IRQs if CPBR is false) */
        },

        /* General interrupt handler actions to use for all timers */
        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,        /* Use this timer in the test */
                .tick_divisor = 2,      /* Tick divisor based on a period of 1s
                                           (so here 0.5s) */
                .el = 3,                /* The expected EL the IRQ handler
                                           should be in when servicing this
                                           timer */
                .lvl = 0,               /* The expected preemption level
                                           (== nested interrupt level) of the
                                           IRQ handler */
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 1,
                .el = 3,
                .lvl = 1,
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
 * Premption test in group 1
 *
 * This tests uses two timers in group 1. A lower group priority one that get
 * preempted by the second one.
 */
static void test_gic_preempt_subgroup_in_grp1(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        { PHYS_TIMER_IRQ, 0, 1, 1, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 1,
                .el = 1,
                .lvl = 1,
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
 * Premption test in group 1
 *
 * This tests uses two timers in group 1. A lower group priority one that get
 * preempted by the second one. CBPR field in CPU_CTRL is set so group1 IRQs
 * should use BPR instead of ABPR
 */
static void test_gic_preempt_subgroup_in_grp1_cbpr(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 1, 0 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = false,
            .cbpr = true,
            .bpr = 2,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 1,
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
 * Premption test in group 1
 *
 * This tests uses two timers in group 1. A lower group priority one that get
 * preempted by the second one. ABPR register is set to 4, which means that
 * subgroup field is 4 bits (and not 3 as it is for BPR)
 */
static void test_gic_preempt_subgroup_in_grp1_abpr_4(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 1, 0 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 4,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 1,
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
 * Premption test in both group 0 and group 1
 *
 * This tests uses four timers in both groups, each having a increasing group priority.
 * Low priority timers are set to trigger before higher priority ones.
 */
static void test_gic_preempt_group_subgroup(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 3, 0 },
        { VIRT_TIMER_IRQ, 0, 1, 2, 0 },
        { HYP_TIMER_IRQ, 0, 0, 1, 0 },
        { SEC_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = true,
            .grp0_to_fiq = true,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 8,
                .el = 1,
                .lvl = 0,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 1,
            },

            [TIMER_HYP] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 2,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = true,
                .tick_divisor = 1,
                .el = 3,
                .lvl = 3,
            },
        },
    };

    GIC_TEST(&INFO);
}

/*
 * Sub-priority test
 *
 * This test setups three timers, PHYS, VIRT and HYP.
 * Trigger order is PHYS and VIRT, HYP approximatly at the same time.
 * This test ensures that VIRT is serviced before HYP, as it has a higher
 * sub-priority.
 */
static void test_gic_subprio(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { HYP_TIMER_IRQ,  0, 0, 0, 1 },
        { VIRT_TIMER_IRQ, 0, 0, 0, 0 },
        { PHYS_TIMER_IRQ, 0, 0, 1, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 0,
                .actions = SUB_PRIO_1ST_TIMER_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 1,
                .actions = SUB_PRIO_2ND_TIMER_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 1,
                .actions = SUB_PRIO_3RD_TIMER_TEST_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = false,
            },
        },
    };

    GIC_TEST(&INFO);
}

/*
 * No premption test in group 0
 *
 * This tests uses two timers in group 0. No preemption should occur since the
 * two timers are in the same group priority, despite one having a sub-priority
 * higher than the other.
 */
static void test_gic_subprio_in_grp0_no_preempt(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 0, 1 },
        { VIRT_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 8,
                .el = 3,
                .lvl = 0,
                .actions = NO_PREEMPT_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 0,
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
 * No premption test in group 1
 *
 * This tests uses two timers in group 1. No preemption should occur since the
 * two timers are in the same group priority, despite one having a sub-priority
 * higher than the other.
 */
static void test_gic_subprio_in_grp1_no_preempt(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 0, 1 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = false,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .actions = PREEMPT_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
                .actions = NO_PREEMPT_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 0,
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
 * Priority drop and preemption test in group 0
 *
 * High priority timer is set to trigger before a low priority one.  Once the
 * first entered its handler, it waits for the second one to trigger (see
 * PRIO_DROP_HI_PRIO_TEST_ACTIONS). This should not generate preemption since
 * the second timer has a lower group priority. However on priority drop,
 * preemption should occur.
 */
static void test_gic_prio_drop_grp0(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 1, 0 },
        { VIRT_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
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
 * Priority drop and preemption test in group 1
 *
 * High priority timer is set to trigger before a low priority one.  Once the
 * first entered its handler, it waits for the second one to trigger (see
 * PRIO_DROP_HI_PRIO_TEST_ACTIONS). This should not generate preemption since
 * the second timer has a lower group priority. However on priority drop,
 * preemption should occur.
 */
static void test_gic_prio_drop_grp1(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 1, 0 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
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
 * Priority drop test for higher sub-priority timers in group 0
 *
 * This test ensures that we got preempted on priority drop by same-subgroup,
 * higher-subprio timers.
 */
static void test_gic_prio_drop_grp0_subprio_lo_hi(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 0, 1 },
        { VIRT_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
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
 * Priority drop test for higher sub-priority timers in group 0
 *
 * This test ensures that we got preempted on priority drop by same-subgroup,
 * lower-subprio timers.
 */
static void test_gic_prio_drop_grp0_subprio_hi_lo(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 0, 0 },
        { VIRT_TIMER_IRQ, 0, 0, 0, 1 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 3,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
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
 * Priority drop test for higher sub-priority timers in group 1
 *
 * This test ensures that we got preempted on priority drop by same-subgroup,
 * higher-subprio timers.
 */
static void test_gic_prio_drop_grp1_subprio_lo_hi(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 0, 1 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
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
 * Priority drop test for higher sub-priority timers in group 1
 *
 * This test ensures that we got preempted on priority drop by same-subgroup,
 * lower-subprio timers.
 */
static void test_gic_prio_drop_grp1_subprio_hi_lo(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 1, 0, 0 },
        { VIRT_TIMER_IRQ, 0, 1, 0, 1 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = false,
            .en_grp1 = true,
            .grp0_to_fiq = false,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 1,
                .lvl = 0,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 2,
                .el = 1,
                .lvl = 1,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
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
 * High priority and priority drop preemption mix
 *
 * This test mixes higher group priority preemption and priority drop
 * preemption.
 * Trigger order is PHYS, SEC, HYP, VIRT:
 *   - PHYS waits for its token to be set by SEC,
 *   - SEC and HYP do priority drop and expect to be preempted by higher level
 *     (resp. HYP and VIRT) and set the lower level token (resp. PHYS and SEC),
 *   - VIRT simply set the HYP token.
 */
static void test_gic_preempt_drop_mix(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 3, 0 },
        { VIRT_TIMER_IRQ, 0, 0, 1, 1 },
        { HYP_TIMER_IRQ, 0, 0, 1, 0 },
        { SEC_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
            .abpr = 5,
        },

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 16,
                .el = 3,
                .lvl = 0,
                .actions = PREEMPT_TEST_ACTIONS,
            },

            [TIMER_VIRT] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 3,
                .actions = PRIO_DROP_LO_PRIO_TEST_ACTIONS,
            },

            [TIMER_HYP] = {
                .enabled = true,
                .tick_divisor = 4,
                .el = 3,
                .lvl = 2,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },

            [TIMER_PHYS_SEC] = {
                .enabled = true,
                .tick_divisor = 8,
                .el = 3,
                .lvl = 1,
                .actions = PRIO_DROP_HI_PRIO_TEST_ACTIONS,
            },
        },
    };

    GIC_TEST(&INFO);
}


/*
 * Active and pending IRQ test
 *
 * No signaling should occur for an IRQ that is in the "active and pending"
 * state.
 *
 */
static void test_gic_active_pending_no_sig(void)
{
    static const struct gic_irq_info GIC_IRQ_INFO[] = {
        { PHYS_TIMER_IRQ, 0, 0, 0, 0 },
        GIC_IRQ_INFO_END
    };

    static const struct test_info INFO = {
        .gic = {
            .irqs = GIC_IRQ_INFO,
            .en_grp0 = true,
            .en_grp1 = false,
            .grp0_to_fiq = true,
            .eoi_mode = true,
            .cbpr = false,
            .bpr = 4,
        },

        .actions = ACTIVE_PENDING_TEST_ACTIONS,

        .timers = {
            [TIMER_PHYS] = {
                .enabled = true,
                .tick_divisor = 16,
                .el = 3,
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

__testcall(test_gic_preempt_group_subgroup);
__testcall(test_gic_preempt_subgroup_in_grp0);
__testcall(test_gic_preempt_subgroup_in_grp1);
__testcall(test_gic_preempt_subgroup_in_grp1_abpr_4);
__testcall(test_gic_preempt_subgroup_in_grp1_cbpr);

__testcall(test_gic_subprio);

__testcall(test_gic_subprio_in_grp0_no_preempt);
__testcall(test_gic_subprio_in_grp1_no_preempt);

__testcall(test_gic_prio_drop_grp0);
__testcall(test_gic_prio_drop_grp1);

__testcall(test_gic_prio_drop_grp0_subprio_lo_hi);
__testcall(test_gic_prio_drop_grp0_subprio_hi_lo);
__testcall(test_gic_prio_drop_grp1_subprio_lo_hi);
__testcall(test_gic_prio_drop_grp1_subprio_hi_lo);

__testcall(test_gic_preempt_drop_mix);

__testcall(test_gic_active_pending_no_sig);

__testcall(test_gic_bpr);
