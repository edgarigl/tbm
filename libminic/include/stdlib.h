#ifndef __stdlib_h__
#define __stdlib_h__

#include <limits.h>
#include <stddef.h>

#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1
void exit(int status);
int on_exit(void (*function)(int , void *), void *arg);

void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void *aligned_alloc(size_t alignment, size_t size);
void free(void *ptr);

#define RAND_MAX (INT_MAX)
int rand(void);
int rand_r(unsigned int *seedp);
void srand(unsigned int seed);


#ifndef PROFILE_MALLOC
#define PROFILE_MALLOC 0
#endif

#ifdef _MINIC_SOURCE
enum alloc_event {
	ALLOC_EV_ALLOC,
	ALLOC_EV_FREE,
	ALLOC_EV_ALLOC_LEVEL,
	ALLOC_EV_EXIT_LEVEL,
};

struct alloc_ctx {
	unsigned long mem;
	size_t mem_size;
#if PROFILE_MALLOC
	void (*event_cb)(int type, void *p, size_t a1);
	size_t mem_alloc;
	size_t mem_max_alloc;
#endif
};

extern struct alloc_ctx _alloc_default_ctx;

static inline struct alloc_ctx * _alloc_get_default_ctx(void)
{
	return &_alloc_default_ctx;
}

unsigned int alloc_init(void *addr, unsigned long size);
unsigned int _alloc_ctx_init(struct alloc_ctx *ctx, void *addr, unsigned long size);
void _alloc_set_event_cb(void (*f)(int type, void *p, size_t a1));
void _alloc_ctx_set_event_cb(struct alloc_ctx *ctx,
                             void (*f)(int type, void *p, size_t a1));
void *_malloc_ctx(struct alloc_ctx *ctx, size_t size);
void _free_ctx(struct alloc_ctx *ctx, void *ptr);
void alloc_exit(void);
void _alloc_ctx_exit(struct alloc_ctx *ctx);
#endif
#endif
