static inline void writeb(void *pv, unsigned char v)
{
	unsigned char *p = pv;
	barrier();
	*p = v;
}

static inline void writel(void *pv, unsigned int v)
{
	unsigned int *p = pv;
	barrier();
	*p = v;
}

static inline unsigned char readb(const void *pv)
{
	const unsigned char *p = pv;
	unsigned char v;

	barrier();
	v = *p;
	return v;
}

static inline unsigned int readl(const void *pv)
{
	const unsigned int *p = pv;
	unsigned int v;

	barrier();
	v = *p;
	return v;
}
