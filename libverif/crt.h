#ifndef __CRT_H__
#define __CRT_H__

#include "libverif/random.h"
#include "libverif/bins.h"

enum crt_types {
	CRT_NONE = 0,
	CRT_UINT32,
	CRT_UINT64,
	CRT_UINTPTR,
};

#define COMPLETENESS_SHIFT (10)
#define COMPLETENESS_MAX (1U << 10)

#define _CRT_TYPE(t, tname) struct {		\
	t min; t max; t mask;			\
	struct {				\
		uint16_t nr;			\
		t *values;			\
	} include;				\
	struct {				\
		t ones;				\
		t zeroes;			\
	} stats;				\
} tname

#define CRT_UINTPTR(state, field, vld)				\
	.type = CRT_UINTPTR, .offset = offsetof(state, field),	\
	  .valid = vld
#define CRT_UINT32(state, field, vld)				\
	.type = CRT_UINT32, .offset = offsetof(state, field),	\
	  .valid = vld
#define CRT_UINT64(state, field, vld)				\
	.type = CRT_UINT64, .offset = offsetof(state, field),	\
	  .valid = vld

typedef union {
	uint32_t uint32;
	uint64_t uint64;
	uintptr_t uintptr;
} crtval;

typedef struct crt_constr {
	enum crt_types type;
	size_t offset;
	bool valid;

	union {
		_CRT_TYPE(uintptr_t, uintptr);
		_CRT_TYPE(uint32_t, uint32);
		_CRT_TYPE(uint64_t, uint64);
	};
} crt_constr;

struct crt_runner;

typedef void (*crtr_cb)(struct crt_runner *r, void *user);
typedef unsigned int (*crtr_completeness_cb)(struct crt_runner *r, void *user);
struct crt_runner {
	struct crt_runner *next;
	const char *name;
	struct bin_state bins;

	unsigned int cur_seed;
	struct random_ctx rand;

	struct {
		uint64_t runs;
		uint64_t fails;
	} stats;

	crtr_cb prep_cb;
	crtr_cb cb;
	crtr_cb teardown_cb;
	crtr_completeness_cb completeness_cb;
	unsigned int compl;
	void *user;
};

#define CRT_FAIL_GOTO_ON(r, expr, label) do {			\
		if (expr) {					\
			crt_fail_1(r, __FILE__, __LINE__);	\
			 goto label;				\
		}						\
	} while (0)

#define CRT_FAIL(r) crt_fail_1(r, __FILE__, __LINE__)
void crt_fail_1(struct crt_runner *r, const char *filename, unsigned int line);

static inline void *crt_runner_get_user(struct crt_runner *r) {
	return r->user;
}

unsigned int crt_completeness_obj(struct crt_runner *r, struct crt_constr *crt);
unsigned int crt_completeness_objs(struct crt_runner *r, struct crt_constr *crt);

void crt_runner_init(struct crt_runner *r);
static inline void crt_runner_set_seet(struct crt_runner *r, unsigned long seed) {
	r->cur_seed = r->rand.seed;
	r->rand.seed = seed;
}
void crt_run_one_func(struct crt_runner *r, crtr_cb f);
void crt_run_one(struct crt_runner *r);
void crt_run_all(void);
void crt_objs(struct crt_runner *r, struct crt_constr *crt, void *obj);
#endif
