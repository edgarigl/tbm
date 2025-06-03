#include "ronaldo-version.h"

struct alloc_areas {
	struct alloc_ctx *ddr;
	struct alloc_ctx *ocm;
};

extern struct alloc_areas plat_mem;
void plat_init_secondaries(void);
