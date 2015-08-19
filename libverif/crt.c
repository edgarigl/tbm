#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libverif/verif.h"
#include "libverif/crt.h"

#define D(x)

static const size_t crt_types_sizes[] = {
	[CRT_UINTPTR] = sizeof (uintptr_t),
	[CRT_UINT32] = sizeof (uint32_t),
	[CRT_UINT64] = sizeof (uint64_t),
};

#define CRT_CONSTR_INT_CODE(seedp, type, valptr, crt) do {			\
	if (crt->type.include.nr) {						\
		uint16_t t = rand_r(seedp);					\
		t %= crt->type.include.nr;					\
		valptr->type = crt->type.include.values[t];			\
	} else {								\
		if (crt->type.mask) valptr->type &= crt->type.mask;		\
		if (valptr->type < crt->type.min) valptr->type = crt->type.min;	\
	}									\
	crt->type.stats.ones |= valptr->type;					\
	crt->type.stats.zeroes |= ~(valptr->type);				\
} while (0)

#define CRT_COMPLETENESS_INT_CODE(type, crt, cmp) do {				\
	typeof(crt->type.stats.ones) mask_1 = ~0;				\
	typeof(crt->type.stats.ones) mask_0 = ~0;				\
	unsigned int bits;							\
	unsigned int max_1;							\
	unsigned int max_0;							\
	if (crt->type.include.nr) {						\
		return COMPLETENESS_MAX;					\
	} else {								\
		if (crt->type.mask) mask_1 &=  crt->type.mask;			\
		if (crt->type.mask) mask_0 &=  ~crt->type.mask;			\
	}									\
	max_1 = __builtin_popcountl(mask_1);					\
	max_0 = __builtin_popcountl(mask_0);					\
	bits = __builtin_popcountl(crt->type.stats.ones & mask_1);		\
	if (bits == max_1)							\
		cmp += COMPLETENESS_MAX;					\
	else									\
		cmp += bits * (COMPLETENESS_MAX / max_1);			\
	bits = __builtin_popcountl(crt->type.stats.zeroes & mask_0);		\
	if (bits == max_0)							\
		cmp += COMPLETENESS_MAX;					\
	else									\
		cmp += bits * (COMPLETENESS_MAX / max_0);			\
	cmp /= 2; /* nr of collected stats.  */					\
} while (0)

static struct crt_runner *first_runner = NULL;

static void crt_obj(struct crt_runner *r, struct crt_constr *crt,
			void *obj)
{
	const size_t size = crt_types_sizes[crt->type];
	crtval *ptr = (void *) ((char *)obj + crt->offset);
	unsigned int *seedp = &r->rand.seed;

	assert(crt->type != CRT_NONE);
	random_fill(seedp, ptr, size);
	if (!crt->valid) {
		return;
	}

	switch (crt->type) {
	case CRT_UINTPTR:
		CRT_CONSTR_INT_CODE(seedp, uintptr, ptr, crt);
		break;
	case CRT_UINT32:
		CRT_CONSTR_INT_CODE(seedp, uint32, ptr, crt);
		break;
	case CRT_UINT64:
		CRT_CONSTR_INT_CODE(seedp, uint64, ptr, crt);
		break;
	default:
		assert(0);
		break;
	}
}

void crt_objs(struct crt_runner *r, struct crt_constr *crt,
		void *obj)
{
	unsigned int i = 0;
	while (crt[i].type != CRT_NONE) {
		crt_obj(r, crt + i, obj);
		i++;
	}
}

unsigned int crt_completeness_obj(struct crt_runner *r, struct crt_constr *crt)
{
	unsigned int completeness = 0;

	if (!crt->valid)
		return COMPLETENESS_MAX;

	switch (crt->type) {
	case CRT_UINTPTR:
		CRT_COMPLETENESS_INT_CODE(uintptr, crt, completeness);
		break;
	case CRT_UINT32:
		CRT_COMPLETENESS_INT_CODE(uint32, crt, completeness);
		break;
	case CRT_UINT64:
		CRT_COMPLETENESS_INT_CODE(uint64, crt, completeness);
		break;
	default:
		assert(0);
		break;
	}
	return completeness;
}

unsigned int crt_completeness_objs(struct crt_runner *r, struct crt_constr *crt)
{
	unsigned int i = 0;
	unsigned int compl = 0;
	while (crt[i].type != CRT_NONE) {
		unsigned int c;
		c = crt_completeness_obj(r, crt + i);
		D(printf("completeness[%d]=%u %u\n", i, c, compl));
		compl += c;
		i++;
	}
	D(printf("completeness[%d]=%u %u\n", i, compl, compl / i));
	return compl / i;
}

void crt_runner_init(struct crt_runner *r)
{
	bin_init(&r->bins, NORMAL);
	random_ctx_init(&r->rand, r->name);

	r->next = first_runner;
	first_runner = r;
}

void crt_run_one_func(struct crt_runner *r, crtr_cb f)
{
	if (r->prep_cb) {
		r->prep_cb(r, r->user);
	}
	f(r, r->user);
	if (r->teardown_cb) {
		r->teardown_cb(r, r->user);
	}
}

void crt_run_one(struct crt_runner *r)
{
	r->cur_seed = r->rand.seed;
	crt_run_one_func(r, r->cb);
	r->stats.runs++;
}

unsigned int crt_completeness_one(struct crt_runner *r)
{
	if (r->completeness_cb) {
		return r->completeness_cb(r, r->user);
	} else {
		return 0;
	}
}

unsigned int crt_completeness_all(void)
{
	return 1;
}

void crt_run_all(void)
{
	uint64_t runs = 0;
	unsigned int i;

	while (true) {
		struct crt_runner *r = first_runner;
		if (!r)
			break;
		if ((runs & 0x2ff) == 0) {
			random_ctx_checkpoint_all();
			printf("master %x\n", random_get_master_seed());
			while (r) {
				r->compl = crt_completeness_one(r);
				printf("\t%s\t%x compl=%u\n", r->name, r->rand.seed, r->compl);
				r = r->next;
			}
			r = first_runner;
		}

		i = 0;
		while (r) {
			if (r->compl == COMPLETENESS_MAX) {
				r = r->next;
				continue;
			}
			i++;
			crt_run_one(r);
			r = r->next;
		}
		if (i == 0)
			break;
		runs += i;
	}
	printf("CRT: All done.\n");
}

void crt_fail_1(struct crt_runner *r, const char *filename, unsigned int line)
{
	r->stats.fails++;
	printf("FAIL: %s at %s:%d runs=%llu failures=%llu\n",
		r->name, filename, line, r->stats.runs, r->stats.fails);
	printf("seeds: master%x checkpoint=%x current=%x\n",
		random_get_prev_master_seed(), r->rand.checkpoint_seed, r->cur_seed);
}
