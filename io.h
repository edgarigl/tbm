#ifdef PHYS_ADDR_T_64BIT
typedef uint64_t phys_addr_t;
#else
typedef uintptr_t phys_addr_t;
#endif

static inline void writeb(phys_addr_t paddr, unsigned char v)
{
	unsigned char *p = (void *) paddr;
	barrier();
	*p = v;
}

static inline void writel(phys_addr_t paddr, unsigned int v)
{
	unsigned int *p = (void *) paddr;
	barrier();
	*p = v;
}

static inline unsigned char readb(phys_addr_t paddr)
{
	const unsigned char *p = (void *) paddr;
	unsigned char v;

	barrier();
	v = *p;
	return v;
}

static inline unsigned int readl(const phys_addr_t paddr)
{
	const unsigned int *p = (void *) paddr;
	unsigned int v;

	barrier();
	v = *p;
	return v;
}
