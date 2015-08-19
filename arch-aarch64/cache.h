#define CACHE_MAX_LEVEL		3

#define CLIDR_CACHE_NONE	0
#define CLIDR_CACHE_SPLIT	3
#define CLIDR_CACHE_UNIFIED	4
#define CLIDR_CACHE_TYPE_MASK	7

struct aarch64_cache_sizes
{
	unsigned int size;
	unsigned int linesize;
	unsigned int ways;
	unsigned int sets;

	unsigned int log2_linesize;
};

struct aarch64_cache_level
{
	unsigned int type;
	struct aarch64_cache_sizes I;
	struct aarch64_cache_sizes D;
};

struct aarch64_cache_descr
{
	unsigned int levels;
	struct aarch64_cache_level level[CACHE_MAX_LEVEL];
};

static inline void a64_icache_invalidate(struct aarch64_cache_descr *cd)
{
	__asm__ __volatile__ ("ic iallu\n");
}

void a64_dcache_invalidate(struct aarch64_cache_descr *cd, bool flush);

static inline void aarch64_enable_caches(unsigned int el, bool i, bool d)
{
	uint32_t sctlr;

	sctlr = aarch64_sctlr(el);
	sctlr |= d ? SCTLR_C : 0;
	sctlr |= i ? SCTLR_I : 0;
	aarch64_set_sctlr(el, sctlr);
}

void a64_cache_discover(struct aarch64_cache_descr *cd);
