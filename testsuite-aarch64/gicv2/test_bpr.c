#define _MINIC_SOURCE

#include <stdbool.h>
#include <stdint.h>

#include "sys.h"
#include "irq.h"
#include "test_entry.h"
#include "trace.h"
#include "switch.h"
#include "gic.h"

#define BPR_TEST_VAL 3
#define ABPR_TEST_VAL 4

#define BPR_TEST_MAX_VAL 7

static inline void bpr_test_secure_rw(int test_val)
{
    /* Write a valid value to BPR */
    writel(GIC_CPU_BASE + GICC_BPR, test_val);
    mb();
    ibarrier();

    assert(readl(GIC_CPU_BASE + GICC_BPR) == test_val);

    /* Write a valid value to ABPR */
    writel(GIC_CPU_BASE + GICC_ABPR, ABPR_TEST_VAL);
    mb();
    ibarrier();

    assert(readl(GIC_CPU_BASE + GICC_ABPR) == ABPR_TEST_VAL);
}

static inline void bpr_test_ns_abpr_raz_wi(void)
{
    /* ABPR is RAZ/WI for NS accesses */
    writel(GIC_CPU_BASE + GICC_ABPR, ABPR_TEST_VAL+2);
    mb();
    ibarrier();

    assert(readl(GIC_CPU_BASE + GICC_ABPR) == 0);
}

static inline void bpr_test_ns_rw_no_cbpr(int test_val)
{
    assert(readl(GIC_CPU_BASE + GICC_BPR) == test_val);

    writel(GIC_CPU_BASE + GICC_BPR, test_val + 1);
    mb();
    ibarrier();

    assert(readl(GIC_CPU_BASE + GICC_BPR) == test_val + 1);
}

static inline void bpr_test_ns_rw_cbpr(int test_val)
{
    const int expected = (test_val == 7) ? test_val : test_val + 1;

    /* Should read "secure value" + 1 */
    assert(readl(GIC_CPU_BASE + GICC_BPR) == expected);

    writel(GIC_CPU_BASE + GICC_BPR, expected - 1);
    mb();
    ibarrier();

    /* Write ignored when CBPR is enabled */
    assert(readl(GIC_CPU_BASE + GICC_BPR) == expected);
}

static void gic_test_bpr_el1(void *arg)
{
    uintptr_t phase = (uintptr_t) arg;

    switch (phase) {
    case 0:
        bpr_test_ns_rw_no_cbpr(ABPR_TEST_VAL);
        break;

    case 1:
        bpr_test_ns_rw_cbpr(BPR_TEST_VAL);
        break;

    case 2:
        bpr_test_ns_rw_cbpr(BPR_TEST_MAX_VAL);
        break;
    }

    bpr_test_ns_abpr_raz_wi();
}

void test_gic_bpr(void)
{
    uint32_t c_ctrl = 0;

    assert(aarch64_current_el() == 3);

    printf("\n\nGIC BPR tests\n");

    /* Enable CPU interface */
    c_ctrl |= 1 << 0;   /* en_grp0 */
    c_ctrl |= 1 << 1;   /* en_grp1 */

    writel(GIC_CPU_BASE + GICC_CTRL, c_ctrl);

    bpr_test_secure_rw(BPR_TEST_VAL);
    switch_to_el1(gic_test_bpr_el1, (void *)0);

    /* Read back the values written by EL1 */
    assert(readl(GIC_CPU_BASE + GICC_BPR) == BPR_TEST_VAL); /* no changes */
    assert(readl(GIC_CPU_BASE + GICC_ABPR) == ABPR_TEST_VAL + 1);

    /* Enable CBPR */
    c_ctrl |= 1 << 4;   /* cbpr */
    writel(GIC_CPU_BASE + GICC_CTRL, c_ctrl);
    mb();
    ibarrier();

    /* No behavior change here */
    bpr_test_secure_rw(BPR_TEST_VAL);
    switch_to_el1(gic_test_bpr_el1, (void *)1);

    /* Read back the values written by EL1 */
    assert(readl(GIC_CPU_BASE + GICC_BPR) == BPR_TEST_VAL); /* no changes */
    assert(readl(GIC_CPU_BASE + GICC_ABPR) == ABPR_TEST_VAL); /* no changes */

    /* Test with max value */
    bpr_test_secure_rw(BPR_TEST_MAX_VAL);
    switch_to_el1(gic_test_bpr_el1, (void *)2);

    /* Disable CPU interface */
    writel(GIC_CPU_BASE + GICC_CTRL, 0);

    printf("GIC BPR tests passed\n");
}
