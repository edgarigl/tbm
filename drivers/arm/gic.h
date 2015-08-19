#ifndef __GIC__
#define __GIC__

static inline void gicd_set_irq_group(char *base, unsigned int irq,
					bool group)
{
	unsigned int offset = (irq / 32) * 4;
	unsigned int bitoffset = irq % 32;
	uint32_t r;

	r = readl(base + GICD_IGROUPR + offset);
	r &= ~(1 << bitoffset);
	r |= group << bitoffset;
	writel(base + GICD_IGROUPR + offset, r);
}

static inline void gicd_set_sgi(char *base, unsigned cpu,
                                unsigned int irq)
{
	uint32_t r;

	r = 1 << (16 + cpu);
	r |= irq;
	writel(base + GICD_SGIR, r);
}

static inline void gicd_set_pending(char *base, unsigned int irq, bool v)
{
	unsigned int offset = (irq / 32) * 4;
	unsigned int bitoffset = irq % 32;
	uint32_t r;

	r = readl(base + GICD_ISPENDR + offset);
	r &= ~(1 << bitoffset);
	r |= v << bitoffset;
	writel(base + GICD_ISPENDR + offset, r);
}

static inline bool gicd_irq_is_pending(char *base, unsigned int irq)
{
	unsigned int offset = (irq / 32) * 4;
	unsigned int bitoffset = irq % 32;
	bool r;

	r = readl(base + GICD_ISPENDR + offset) & (1 << bitoffset);
	return r;
}

static inline void gicd_set_irq_target(char *base, unsigned int irq,
					unsigned int target)
{
	unsigned int wordoffset = irq & ~3;
	unsigned int byteoffset = irq % 4;
	uint32_t r;

	r = readl(base + GICD_ITARGETSR + wordoffset);
	r |= 1 << (byteoffset * 8 + target);
	writel(base + GICD_ITARGETSR + wordoffset, r);
}

static inline void gicd_enable_irq(char *base, unsigned int irq)
{
	unsigned int offset = (irq / 32) * 4;
	unsigned int bitoffset = irq % 32;
	uint32_t r;

	r = readl(base + GICD_ISENABLER + offset);
	r |= 1 << bitoffset;
	writel(base + GICD_ISENABLER + offset, r);
}
#endif
