#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sys.h"
#include "bitops.h"

#include "cache.h"

#define D(x) x

void a64_dcache_invalidate(struct aarch64_cache_descr *cd, bool flush)
{
	unsigned int i;
	unsigned int way, set;
	unsigned int way_shift = 0;

	ibarrier();
	mb();
	for (i = 0; i < CACHE_MAX_LEVEL; i++) {
		uint32_t r;

		if (cd->level[i].type == CLIDR_CACHE_NONE) {
			break;
		}

		switch (cd->level[i].D.ways) {
		case 4:
			way_shift = 32 - 2;
			break;
		case 16:
			way_shift = 32 - 4;
			break;
		default:
			assert(0);
		}


		for (set = 0; set < cd->level[i].D.sets; set++) {
			for (way = 0; way < cd->level[i].D.ways; way++) {
				r = i;
				r |= way << way_shift;
				r |= set << cd->level[i].D.log2_linesize;
//				D(printf("D$ flush set=%d way=%d r=%x\n", set, way, r));
				if (flush)
					__asm__ __volatile__ ("dc cisw, %0\n" : : "r" (r));
				else
					__asm__ __volatile__ ("dc isw, %0\n" : : "r" (r));
			}
		}
	}
	ibarrier();
	mb();
}

static void a64_cache_discover_level(struct aarch64_cache_sizes *cs, uint32_t ccsidr)
{
	cs->log2_linesize = bit_field32(ccsidr, 0, 3) + 2;
	cs->log2_linesize += 2; /* words to bytes.  */
	cs->linesize = 1 << cs->log2_linesize;

	cs->ways = bit_field32(ccsidr, 3, 10) + 1;
	cs->sets = bit_field32(ccsidr, 13, 14) + 1;

	cs->size = cs->ways * cs->sets * cs->linesize;
}

void a64_cache_discover(struct aarch64_cache_descr *cd)
{
	uint32_t clidr;
	uint32_t ccsidr;
	unsigned int i;

	aarch64_mrs(clidr, "clidr_el1");
	for (i = 0; i < CACHE_MAX_LEVEL; i++) {
		unsigned int type = clidr & CLIDR_CACHE_TYPE_MASK;

		cd->level[i].type = type;
		if (type == CLIDR_CACHE_NONE) {
			break;
		}

		aarch64_msr("CSSELR_EL1", i << 1);
		ibarrier();
		aarch64_mrs(ccsidr, "CCSIDR_EL1");

		a64_cache_discover_level(&cd->level[i].D, ccsidr);
		printf("D$ L%d %dKB linesize=%ub ways=%d sets=%d\n",
			i + 1, cd->level[i].D.size / 1024,
			cd->level[i].D.linesize, cd->level[i].D.ways, cd->level[i].D.sets);
		if (type == CLIDR_CACHE_SPLIT) {
			aarch64_msr("CSSELR_EL1", (i << 1) | 1);
			ibarrier();
			aarch64_mrs(ccsidr, "CCSIDR_EL1");
		}
		a64_cache_discover_level(&cd->level[i].I, ccsidr);

		printf("I$ L%d %dKB linesize=%ub ways=%d sets=%d\n",
			i + 1, cd->level[i].I.size / 1024,
			cd->level[i].I.linesize, cd->level[i].I.ways, cd->level[i].I.sets);
		clidr >>= 3;
	}
}
