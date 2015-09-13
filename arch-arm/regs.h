
#define SCR_NS        (1U << 0)
#define SCR_IRQ       (1U << 1)
#define SCR_FIQ       (1U << 2)
#define SCR_EA        (1U << 3)
#define SCR_SMD       (1U << 7)
#define SCR_HCE       (1U << 8)
#define SCR_SIF       (1U << 9)
#define SCR_RW        (1U << 10)
#define SCR_ST        (1U << 11)
#define SCR_TWI       (1U << 12)
#define SCR_TWE       (1U << 13)

static inline uint32_t a32_get_scr(void)
{
	uint32_t scr;
	__asm__ __volatile__ ("mrc\tp15, 0, %0, c1, c1, 0\n" : "=r" (scr));
	return scr;
}

static inline void a32_set_scr(uint32_t scr)
{
	__asm__ __volatile__ ("mcr\tp15, 0, %0, c1, c1, 0\n" : : "r" (scr));
}

static inline uint64_t a32_get_hcr(void)
{
	union {
		uint32_t u32[2];
		uint64_t u64;
	} hcr;
	__asm__ __volatile__ ("mrc\tp15, 4, %0, c1, c1, 0\n" : "=r" (hcr.u32[0]));
	__asm__ __volatile__ ("mrc\tp15, 4, %0, c1, c1, 4\n" : "=r" (hcr.u32[1]));
	return hcr.u64;
}

static inline void a32_set_hcr(uint64_t v)
{
	union {
		uint32_t u32[2];
		uint64_t u64;
	} hcr = { .u64 = v };
	__asm__ __volatile__ ("mcr\tp15, 4, %0, c1, c1, 0\n" : : "r" (hcr.u32[0]));
	__asm__ __volatile__ ("mcr\tp15, 4, %0, c1, c1, 4\n" : : "r" (hcr.u32[1]));
}

static inline uint64_t a32_get_vmpidr(void)
{
	uint32_t v;
	__asm__ __volatile__ ("mrc\tp15, 4, %0, c0, c0, 5\n" : "=r" (v));
	return v;
}

static inline void a32_set_vmpidr(uint32_t v)
{
	__asm__ __volatile__ ("mcr\tp15, 4, %0, c0, c0, 5\n" : : "r" (v));
}

static inline uint64_t a32_get_vttbr(void)
{
	uint32_t vttbr_lo;
	uint32_t vttbr_hi;
	uint64_t vttbr;

	__asm__ __volatile__ ("mrrc\tp15, 6, %0, %1, c2\n" : "=r" (vttbr_lo), "=r" (vttbr_hi));
	vttbr = vttbr_hi;
	vttbr <<= 32;
	vttbr |= vttbr_lo;
	return vttbr;
}

static inline void a32_set_vttbr(uint64_t v)
{
	uint32_t vttbr_lo = v;
	uint32_t vttbr_hi = v >> 32;
	__asm__ __volatile__ ("mcrr\tp15, 6, %0, %1, c2\n" : : "r" (vttbr_lo), "r" (vttbr_hi));
}
