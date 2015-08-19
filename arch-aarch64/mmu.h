#define MAIR_OUTER_DEVICE	(0x0 << 4)
#define MAIR_OUTER_MEM_WT_TRANS	(0x0 << 4)
#define MAIR_OUTER_MEM_NONCACHE	(0x4 << 4)
#define MAIR_OUTER_MEM_WB_TRANS	(0x4 << 4)
#define MAIR_OUTER_MEM_WT	(0x8 << 4)
#define MAIR_OUTER_MEM_WB	(0xc << 4)
#define MAIR_OUTER_W		(1 << 4)
#define MAIR_OUTER_R		(2 << 4)

#define MAIR_INNER_MEM_WT_TRANS	(0x0 << 0)
#define MAIR_INNER_MEM_NONCACHE	(0x4 << 0)
#define MAIR_INNER_MEM_WB_TRANS	(0x4 << 0)
#define MAIR_INNER_MEM_WT	(0x8 << 0)
#define MAIR_INNER_MEM_WB	(0xc << 0)
#define MAIR_INNER_W		(1 << 0)
#define MAIR_INNER_R		(2 << 0)

/* Gather, Reorder, Early-write ack.  */
#define MAIR_DEVICE_NG_NR_NE	(0x00 << 0)
#define MAIR_DEVICE_NG_NR_E	(0x04 << 0)
#define MAIR_DEVICE_NG_R_E	(0x08 << 0)
#define MAIR_DEVICE_G_R_E	(0x0c << 0)

#define TCR_SL0_4K_L2		(0ULL << 6)
#define TCR_SL0_4K_L1		(1ULL << 6)
#define TCR_SL0_4K_L0		(2ULL << 6)

#define TCR_IRGN0_NONCACHE	(0 << 8)
#define TCR_IRGN0_WB_WA		(1 << 8)
#define TCR_IRGN0_WT		(2 << 8)
#define TCR_IRGN0_WB		(3 << 8)

#define TCR_ORGN0_NONCACHE	(0 << 10)
#define TCR_ORGN0_WB_WA		(1 << 10)
#define TCR_ORGN0_WT		(2 << 10)
#define TCR_ORGN0_WB		(3 << 10)

#define TCR_SH0_NON_SHARE		(0ULL << 12)
#define TCR_SH0_OUTER_SHARE		(1ULL << 12)
#define TCR_SH0_INNER_SHARE		(2ULL << 12)

#define TCR_TG0_4K_MASK			(0ULL << 14)
#define TCR_TG0_16K_MASK		(1ULL << 14)
#define TCR_TG0_64K_MASK		(2ULL << 14)

#define TCR_PHYSSIZE_32_4GB_MASK	(0ULL << 16)
#define TCR_PHYSSIZE_36_64GB_MASK	(1ULL << 16)
#define TCR_PHYSSIZE_40_1TB_MASK	(2ULL << 16)
#define TCR_PHYSSIZE_42_4TB_MASK	(3ULL << 16)
#define TCR_PHYSSIZE_44_16TB_MASK	(4ULL << 16)
#define TCR_PHYSSIZE_48_256TB_MASK	(5ULL << 16)

#define PTE_ALLOCATED			(1 << 0)

struct ptdesc {
	union {
		uint64_t u64;
		uint32_t u32[2];
		struct {
			uint64_t pad : 55;
			unsigned int swdef : 4;
		} sw;
		struct {
			unsigned int is_valid:1;
			unsigned int is_table:1;
			unsigned int attrindex : 3;
			unsigned int ns : 1;
			unsigned int ap : 2;
			unsigned int sh : 2;
			unsigned int af : 1;
			unsigned int ng : 1;
			uintptr_t addr : 36;
			unsigned int contigous : 1;
			unsigned int pxn : 1;
			unsigned int xn : 1;
			unsigned int swdef : 4;
		} block_s1;
		struct {
			unsigned int is_valid:1;
			unsigned int is_table:1;
			unsigned int memattr : 4;
			unsigned int s2ap : 2;
			unsigned int sh : 2;
			unsigned int af : 1;
			unsigned int pad1 : 1;
			uintptr_t addr : 36;
			unsigned int contigous : 1;
			unsigned int pad2 : 1;
			unsigned int xn : 1;
			unsigned int swdef : 4;
		} block_s2;
		struct {
			unsigned int is_valid:1;
			unsigned int is_table:1;
			uintptr_t addr : 46;
		} table;
	};
};

enum pagekind {
	PAGEKIND_TINY_4K,
	PAGEKIND_MID_16K,
	PAGEKIND_HUGE_64K,
};

enum pagesize {
	PAGESIZE_4K = 4 * 1024,
	PAGESIZE_16K = 16 * 1024,
	PAGESIZE_64K = 64 * 1024,
};

enum mapsize {
	MAPSIZE_4K = 4ULL * 1024,
	MAPSIZE_16K = 16ULL * 1024,
	MAPSIZE_64K = 64ULL * 1024,
	MAPSIZE_2M = 2ULL * 1024 * 1024,
	MAPSIZE_1G = 1ULL * 1024 * 1024 * 1024,
};

struct pagetables
{
	struct ptdesc *root;
	struct ptdesc *pos;
	struct ptdesc *end;
};

struct mmu_ctx
{
	struct pagetables pt;
	enum pagekind pagekind;
	uintptr_t pagesize;
	uint64_t virtbits;
	uint64_t physbits;

	unsigned int grainsize;

	/* PTW start level.  */
	unsigned int startlevel;
	/* Staget 1 or 2.  */
	unsigned int stage;

	uint64_t mair;
	uint64_t tcr;
};

#define MAIR_IDX_DEVICE		0
#define MAIR_IDX_DEVICE_E	1
#define MAIR_IDX_MEM		2
#define MAIR_IDX_MEM_NONCACHE	3

#define MAP_SECURE	true
#define MAP_NONSECURE	false

void aarch64_mmu_compute_tcr(struct mmu_ctx *mmu, unsigned int el);
void aarch64_mmu_compute_mair(struct mmu_ctx *mmu, unsigned int el);
void aarch64_mmu_setup_mair(struct mmu_ctx *mmu, unsigned int el);
void aarch64_mmu_setup_tcr(struct mmu_ctx *mmu, unsigned int el);
void aarch64_mmu_setup(struct mmu_ctx *mmu, unsigned int el, bool enable);
uintptr_t aarch64_mmu_pt_setup(struct mmu_ctx *mmu,
                        enum pagekind pagekind,
			unsigned int stage,
			unsigned int virtbits,
			unsigned int physbits,
                        void *start, void *end);
void aarch64_mmu_pt_zap(struct mmu_ctx *mmu, unsigned int el);

#define PROT_READ	(1 << 0)
#define PROT_WRITE	(1 << 1)
#define PROT_EXEC	(1 << 2)
#define PROT_RW		(PROT_READ | PROT_WRITE)
#define PROT_RWE	(PROT_RW | PROT_EXEC)
bool aarch64_mmu_map(struct mmu_ctx *mmu,
                        uintptr_t va, uintptr_t pa,
                        uintptr_t len, unsigned int prot,
			unsigned int attridx,
			bool secure);

#define DD(x)

static inline void aarch64_mmu_tlb_flush(struct mmu_ctx *mmu, unsigned int el)
{
	switch (el) {
	case 1:
		__asm__ __volatile__ ("tlbi alle1\n");
		break;
	case 2:
		__asm__ __volatile__ ("tlbi alle2\n");
		break;
	case 3:
		__asm__ __volatile__ ("tlbi alle3\n");
		break;
	}
}

static inline uintptr_t aarch64_addr_to_offset(struct mmu_ctx *mmu,
						unsigned int level,
						uintptr_t addr)
{
	uintptr_t offset;
	unsigned int y;
	unsigned int stride = mmu->grainsize - 3;

	switch (mmu->pagesize)
	{
	case PAGESIZE_4K: {
		y = stride * (4 - level);
		offset = addr >> y;
		offset &= ~7ULL;
		break;
	}
	default:
		abort();
	}

	offset &= mmu->pagesize - 1;
	DD(printf("offset=%lx addr=%lx pagesize=%lx\n", offset, addr, mmu->pagesize));
	return offset;
}

static inline uintptr_t aarch64_block_addr(struct mmu_ctx *mmu,
					unsigned int level,
					uintptr_t addr)
{
	uintptr_t baddr;

	addr &= ((1ULL << 39) - 1);
	switch (mmu->pagesize)
	{
	case PAGESIZE_4K: {
		baddr = addr;
		baddr >>= 12;
		break;
	}
	default:
		abort();
	}
	return baddr;
}

static inline struct ptdesc *aarch64_ptdesc(struct mmu_ctx *mmu, struct ptdesc *table,
					unsigned int level,
					uintptr_t va)
{
	uintptr_t offset = aarch64_addr_to_offset(mmu, level, va);
	DD(printf("ptdesc: %p + %lx\n", table, offset));
	offset >>= 3;
	return table + offset;
}
