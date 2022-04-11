/*
 * Smoke tests for the Everest CRL.
 * Intially written by Edgar E. Iglesias.
 * Further edited by Deepen M. Parmar.
 */

#define _MINIC_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "regops.h"

#include "plat-memmap.dtsh"
#include "drivers/versal/reg-crl.h"

#include "drivers/zynqmp/zdma.h"
#include "drivers/versal/reg-canfd.h"

static void check_rst_adma(void)
{
	uint32_t v;
	int i;

	printf("%s\n", __func__);
	// Release from reset.
	writel(MM_CRL + R_CRL_RST_ADMA, 0);

	for (i = 0; i < 8; i++) {
		phys_addr_t ch_base = MM_ADMA_CH0 + MM_ADMA_CH0_SIZE * i;
		writel(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2, 1);
		v = readl(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2);
		assert(v == 1);
	}

	for (i = 0; i < 8; i++) {
		printf("%s: ADMA%d\n", __func__, i);
		/* Reset.  */
		writel(MM_CRL + R_CRL_RST_ADMA, 1);
		writel(MM_CRL + R_CRL_RST_ADMA, 0);

		phys_addr_t ch_base = MM_ADMA_CH0 + MM_ADMA_CH0_SIZE * i;
		v = readl(ch_base + ZDMA_ZDMA_CH_SRC_DSCR_WORD2);
		assert(v == 0);
	}
	printf("%s: PASSED\n", __func__);
}

static void check_rst_gem(void)
{
	printf("%s\n", __func__);
	writel(MM_CRL + R_CRL_RST_GEM0, 0);
	writel(MM_CRL + R_CRL_RST_GEM1, 0);

	/* Writing to the registers whose value changes on reset. */

	/* Network config register */
	writel(MM_GEM0 + 0x04, 0xFFFFFFFF);
	writel(MM_GEM1 + 0x04, 0xFFFFFFFF);

	assert(readl(MM_GEM0 + 0x04) == 0xFFFFFFFF);
	assert(readl(MM_GEM1 + 0x04) == 0xFFFFFFFF);

	/* Resetting GEM */
	writel(MM_CRL + R_CRL_RST_GEM0, 1);
	writel(MM_CRL + R_CRL_RST_GEM1, 1);

	/* Asserting with the reset value. */
	assert(readl(MM_GEM0 + 0x04) == 0x00080000);
	assert(readl(MM_GEM0 + 0x04) == 0x00080000);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_ttcs(void)
{
	printf("%s\n", __func__);
	writel(MM_CRL + R_CRL_RST_TTC, 0);

	/* Writing to counter control register. */
	writel(MM_TTC0 + 0x14, 0x2F);
	writel(MM_TTC0 + 0x74, 0xF);
	writel(MM_TTC0 + 0x78, 0xF);

	assert(readl(MM_TTC0 + 0x14) == 0x2F);

	writel(MM_CRL + R_CRL_RST_TTC, 0x1);

	assert(readl(MM_TTC0 + 0x14) == 0x21);

	printf("%s: PASSED\n", __func__);
}

static void check_pll_locked(void)
{
	uint32_t v;

	printf("%s\n", __func__);
	v = readl(MM_CRL + R_CRL_PLL_STATUS);
	assert(v == 5);
}

static void check_rst_canfd(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_CAN0, 0);
	writel(MM_CRL + R_CRL_RST_CAN1, 0);

	writel(MM_CANFD0 + R_CANFD_MODE_SELECT_REGISTER, 0x00000000);
	writel(MM_CANFD1 + R_CANFD_MODE_SELECT_REGISTER, 0x00000000);

	writel(MM_CANFD0 + R_CANFD_SOFTWARE_RESET_REGISTER,
			CANFD_SOFTWARE_RESET_REGISTER_CEN_MASK);

	writel(MM_CANFD1 + R_CANFD_SOFTWARE_RESET_REGISTER,
			CANFD_SOFTWARE_RESET_REGISTER_CEN_MASK);

	assert((readl(MM_CANFD0 + R_CANFD_STATUS_REGISTER) &
				CANFD_STATUS_REGISTER_NORMAL_MASK) ==
			CANFD_STATUS_REGISTER_NORMAL_MASK);

	assert((readl(MM_CANFD1 + R_CANFD_STATUS_REGISTER) &
				CANFD_STATUS_REGISTER_NORMAL_MASK) ==
			CANFD_STATUS_REGISTER_NORMAL_MASK);

	writel(MM_CRL + R_CRL_RST_CAN0, 1);
	writel(MM_CRL + R_CRL_RST_CAN1, 1);

	uint32_t reg_value1 = readl(MM_CANFD0 + R_CANFD_STATUS_REGISTER);
	uint32_t reg_value2 = readl(MM_CANFD1 + R_CANFD_STATUS_REGISTER);

	assert(reg_value1 == 0x1);
	assert(reg_value2 == 0x1);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_usb(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_USB0, 0);

	assert((readl(MM_USB_XHCI + 0xc524)) == 0x0);
	/* Writing to those registers whose value change on reset. */

	/* Global Core Control register. */
	writel(MM_USB_XHCI + 0xc110, 0xFFFFFFFF);

	assert((readl(MM_USB_XHCI + 0xc110)) == 0xFFFFFFFF);

	writel(MM_CRL + R_CRL_RST_USB0, 1);
	writel(MM_CRL + R_CRL_RST_USB0, 0);

	/* Asserting after reset is applied. */
	assert((readl(MM_USB_XHCI + 0xc524)) == 0x0);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_spi(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_SPI0, 0);
	writel(MM_CRL + R_CRL_RST_SPI1, 0);

	/* Writing to config register. */
	writel(MM_SPI0, 0x840AFFFF);
	writel(MM_SPI1, 0x840AFFFF);

	/* Writing to Delay register. */
	writel(MM_SPI0 + 0x18, 0xFFFFFFFF);
	writel(MM_SPI1 + 0x18, 0xFFFFFFFF);


	assert(readl(MM_SPI0) == 0x840AFFFF);
	assert(readl(MM_SPI0 + 0x18) == 0xFFFFFFFF);

	assert(readl(MM_SPI1) == 0x840AFFFF);
	assert(readl(MM_SPI1 + 0x18) == 0xFFFFFFFF);

	writel(MM_CRL + R_CRL_RST_SPI0, 1);
	writel(MM_CRL + R_CRL_RST_SPI1, 1);

	assert(readl(MM_SPI0) == 0x00020000);
	assert(readl(MM_SPI0 + 0x18) == 0x00000000);

	assert(readl(MM_SPI1) == 0x00020000);
	assert(readl(MM_SPI1 + 0x18) == 0x00000000);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_i2c(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_I2C0, 0);
	writel(MM_CRL + R_CRL_RST_I2C1, 0);

	/* Writing to the control register. */
	writel(MM_PS_I2C0, 0xFF3F);
	writel(MM_PS_I2C1, 0xFF3F);

	assert(readl(MM_PS_I2C0) == 0xFF3F);
	assert(readl(MM_PS_I2C1) == 0xFF3F);

	writel(MM_CRL + R_CRL_RST_I2C0, 1);
	writel(MM_CRL + R_CRL_RST_I2C1, 1);

	assert(readl(MM_PS_I2C0) == 0x0000);
	assert(readl(MM_PS_I2C1) == 0x0000);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_wwdt(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_SWDT, 0);

	/* Writing to Functional control register. */
	writel(MM_WWDT + 0x08, 0xFFDF);

	/* Writing to First window config register. */
	writel(MM_WWDT + 0x0c, 0xFFFF);

	assert(readl(MM_WWDT + 0x08) == 0xFFDF);
	assert(readl(MM_WWDT + 0x0c) == 0xFFFF);

	writel(MM_CRL + R_CRL_RST_SWDT, 1);

	assert(readl(MM_WWDT + 0x08) == 0x00);
	assert(readl(MM_WWDT + 0x0c) == 0x00);

	printf("%s: PASSED\n", __func__);
}

static void check_rst_ocm(void)
{
	printf("%s\n", __func__);

	writel(MM_CRL + R_CRL_RST_OCM, 0);

	/* Writing to the control register for OCM. */
	writel(MM_OCM + 0x14, 0x7);

	/* Writing to the Fault Injection Data register. */
	writel(MM_OCM + 0x4c, 0xFFFFFFFF);

	assert(readl(MM_OCM + 0x14) == 0x07);
	assert(readl(MM_OCM + 0x4c) == 0xFFFFFFFF);

	writel(MM_CRL + R_CRL_RST_OCM, 1);

	assert(readl(MM_OCM + 0x14) == 0x0);
	assert(readl(MM_OCM + 0x4c) == 0x0);

	printf("%s: PASSED\n", __func__);
}

/* Cannot be tested now as APU cannot write to it by default. */
/*
   static void check_rst_ipi(void)
   {
   printf("%s\n", __func__);

   writel(MM_CRL + R_CRL_RST_IPI, 0);

   writel(MM_IPI, 0xFFFFFFFF);

   printf("%x\n", readl(MM_IPI));

   writel(MM_CRL + R_CRL_RST_IPI, 1);

   printf("%x\n", readl(MM_IPI));
   }
 */

/* This function cannot be tested as resetting FPD resets the APU. */
/*
   static void check_rst_fpd(void)
   {
   writel(MM_CRL + R_CRL_RST_FPD, 0);
   writel(MM_CRL + R_CRL_RST_FPD, 2);
   writel(MM_CRL + R_CRL_RST_FPD, 0);
   }
 */

void check_crl(void)
{
	check_pll_locked();
	check_rst_adma();
	check_rst_usb();
	check_rst_gem();
//	check_rst_ttcs();
//	check_rst_uarts();
//	check_rst_ocm();
//	check_rst_wwdt();
//	check_rst_canfd();
//	check_rst_spi();
//	check_rst_i2c();
}
