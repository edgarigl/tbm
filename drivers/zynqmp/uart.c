#include <stdint.h>
#include "sys.h"

#ifdef __zynqmp__
#include "ronaldo-map.dtsh"

#define CRL_APB_BASEADDR  ((char *) CRL_APB)
#define CRL_APB_RST_LPD_IOU2    ( ( CRL_APB_BASEADDR ) + 0X00000238 )
#define CRL_APB_RST_LPD_IOU2_UART0_RESET_MASK    0X00000002

#endif

#ifdef __pele__
#define UART0 0xE0000000
#endif

#define R_UART_CR    (0x00)
#define R_UART_MR    (0x04)
#define R_UART_IER   (0x08)
#define R_UART_IDR   (0x0C)
#define R_UART_IMR   (0x10)
#define R_UART_CISR  (0x14)
#define R_UART_BRGR  (0x18)
#define R_UART_RTRIG (0x20)
#define R_UART_SR    (0x2C)
#define UART_SR_INTR_RTRIG     0x00000001
#define UART_SR_INTR_REMPTY    0x00000002
#define UART_SR_INTR_TEMPTY    0x00000008
#define UART_SR_INTR_TFUL      0x00000010

#define R_UART_TX    (0x30)
#define R_UART_RX    (0x30)
#define R_UART_BDIV  (0x34)

static phys_addr_t global_console_base = UART0;

int uart_putchar(int c)
{
	phys_addr_t base = global_console_base;
	uint32_t status;

	/* Wait for an empty slot.  */
	do {
		status = readl(base + R_UART_SR);
	} while (status & UART_SR_INTR_TFUL);

	writel(base + R_UART_TX, c);

	/* Drain the Queue (keeps events synchronized).  */
	do {
		status = readl(base + R_UART_SR);
	} while (!(status & UART_SR_INTR_TEMPTY));
	return c;
}

void uart_init(void)
{
	phys_addr_t base = global_console_base;

#ifdef __ronaldo__
	uint32_t r;

	/* Release from reset.  */
	r = readl(CRL_APB_RST_LPD_IOU2);
	r &= ~CRL_APB_RST_LPD_IOU2_UART0_RESET_MASK;
	writel(CRL_APB_RST_LPD_IOU2, r);
#endif

#if 0
	/* Rely on Bootloader settings. */
	writel(base + R_UART_BRGR, 0x1f);
	writel(base + R_UART_BDIV, 0x06);
#endif
	writel(base + R_UART_MR, 0x20);
	writel(base + R_UART_CR, 0x17);
	writel(base + R_UART_IER, UART_SR_INTR_RTRIG);
	writel(base + R_UART_RTRIG, 1);
	writel(base + R_UART_CISR, ~0);
	mb();
}

#ifdef HAVE_FDT
#include "devtree.h"

static bool uart_probe(void *fdt, int node, const char *compat)
{
	phys_addr_t p;
	static bool bound = false;

	if (bound)
		return false;

	p = (phys_addr_t) dt_map(fdt, node, 0);
	printf("Bound %s at %lx\n", compat, p);
	global_console_base = p;
	bound = true;
	return true;
}

static struct fdt_probe fdt_uart = {
	.probe = uart_probe,
	.match = (struct fdt_match []) {
		{ .compat = "xlnx,xuartps" },
		{},
	},
};

__fdt_probe(fdt_uart);
#endif
