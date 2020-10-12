/*
Copyright (c) 2009, Edgar E. Iglesias <edgar.iglesias@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

/*
 * Minimalistic allocator. Performs slowly but consumes small amounts of
 * memory and has a low code footprint.
 *
 * Written by Edgar E. iglesias.
 *
 */
#define _MINIC_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FATALD
#define FATALD(x)
#endif

#define D(x)
struct allocation {
	struct allocation *next;
	size_t size;
};

struct alloc_ctx _alloc_default_ctx;

#if PROFILE_MALLOC
void _alloc_ctx_set_event_cb(struct alloc_ctx *ctx,
                            void (*f)(int type, void *p, size_t a1))
{
	ctx->event_cb = f;
}

void _alloc_set_event_cb(void (*f)(int type, void *p, size_t a1))
{
	_alloc_ctx_set_event_cb(&_alloc_default_ctx, f);
}
#endif

static void alloc_event(struct alloc_ctx *ctx, int type, void *p, size_t a1)
{
#if PROFILE_MALLOC
	if (ctx->event_cb)
		ctx->event_cb(type, p, a1);
#endif
}

static inline unsigned long alloc_start(struct allocation *a)
{
	return (unsigned long) a;
}

static inline unsigned long alloc_end(struct allocation *a)
{
	return alloc_start(a) + a->size;
}

static inline void *alloc_data(struct allocation *a)
{
	return (void *)(alloc_start(a) + sizeof *a);
}

static inline struct allocation *alloc_from_data(void *p)
{
	struct allocation *a = p;
	return a - 1;
}

unsigned int _alloc_ctx_init(struct alloc_ctx *ctx, void *addr, unsigned long size)
{
	ctx->mem = (unsigned long) addr;
	ctx->mem_size = size;
#if PROFILE_MALLOC
	ctx->mem_max_alloc = 0;
	ctx->mem_alloc = 0;
	ctx->event_cb = 0;
#endif
	/* align to 64b boundary */
	ctx->mem += 7;
	ctx->mem &= ~7;

	D(printf("alloc_init: %lx: %d bytes\n", (unsigned long) ctx->mem, (int) size));

	/* Mark the first entry as free.  */
	memset((void *)ctx->mem, 0, sizeof (struct allocation));

	/* To support aligned_alloc, we need a first allocation.  */
	_malloc_ctx(ctx, 8);
	return 0;
}

unsigned int alloc_init(void *addr, unsigned long size)
{
	return _alloc_ctx_init(&_alloc_default_ctx, addr, size);
}

void _alloc_ctx_exit(struct alloc_ctx *ctx)
{
#if PROFILE_MALLOC
	alloc_event(ctx, ALLOC_EV_EXIT_LEVEL, NULL, ctx->mem_alloc);
	ctx->mem_alloc = 0;
#endif
}

void alloc_exit(void)
{
	_alloc_ctx_exit(&_alloc_default_ctx);
}

void *_aligned_alloc_ctx(struct alloc_ctx *ctx, size_t alignment, size_t size)
{
	struct allocation *a, *new = NULL;
	size_t gap = 0;
	size_t align_offset;
	void *p = NULL;


	D(printf("%s(%d, %d) enter\n", __func__, (int) alignment, (int) size));
	FATALD(if (!size) printf("WARN: %s(%lu)\n", __func__,
			(unsigned long)size));

	/* Align allocation size to 8 bytes.  */
	size += 7;
	size &= ~7;
	alignment += 7;
	alignment &= ~7;

	a = (void *) ctx->mem;

	while (a) {
		D(printf("a=%p a->next=%p\n", a, a->next));
		if (a->next) {
			/* Compute the gap.  */
			gap = alloc_start(a->next) - alloc_end(a);
		} else {
			/* This is the last entry.  */
			gap = ctx->mem + ctx->mem_size;
			gap -= alloc_end(a);
		}

		align_offset = 0;
		if (alignment) {
			unsigned long x;

			x = alloc_end(a);
			x &= alignment - 1;
			align_offset = alignment - x;
			if (x == 0 || align_offset < sizeof *a) {
				align_offset = alignment + align_offset - sizeof *a;
			} else {
				align_offset -= sizeof *a;
			}
		}

		/* Is there more room?  */
		if (gap >= (size + sizeof *a + align_offset)) {
			struct allocation *t;

			/* Allocate here.  */
			new = (void *) alloc_end(a) + align_offset;

			/* link into the list.  */
			t = a->next;
			a->next = new; /* a maybe same as new.  */
			new->next = t;

			new->size = size + sizeof *new;
			p = alloc_data(new);
#if PROFILE_MALLOC
			ctx->mem_alloc += new->size;
			if (ctx->mem_max_alloc < ctx->mem_alloc) {
				alloc_event(ctx, ALLOC_EV_ALLOC_LEVEL,
					    p, ctx->mem_alloc);
				ctx->mem_max_alloc = ctx->mem_alloc;
			}
#endif
			break;
		}
		a = a->next;
	}

	if (!p || !size) {
		alloc_event(ctx, ALLOC_EV_ALLOC, p, size);
	}

	FATALD(if (!p) printf("%s(%lu) = NULL\n",
			 __func__, (unsigned long)size));
	D(printf("%s(%d) = %p (new=%p, new->next=%p)\n", __func__,
			(int) size, p, new, new ? new->next : 0));
	return p;
}

void *aligned_alloc(size_t alignment, size_t size)
{
	return _aligned_alloc_ctx(&_alloc_default_ctx, alignment, size);
}

void *_malloc_ctx(struct alloc_ctx *ctx, size_t size)
{
	return _aligned_alloc_ctx(ctx, 0, size);
}

void *malloc(size_t size)
{
	return _malloc_ctx(&_alloc_default_ctx, size);
}

void _free_ctx(struct alloc_ctx *ctx, void *ptr)
{
	struct allocation *f, *a, *prev = NULL;

	D(printf("%s(%p) mem=%p\n", __func__, ptr, (void*) ctx->mem));
	if (ptr == NULL)
		return;

	f = alloc_from_data(ptr);

	a = (void *) ctx->mem;
	while (a) {
		D(printf("a=%p a->size=%d a->next=%p f=%p\n",
		         a, (int) a->size, a->next, f));
		if (a == f) {
			/* Unlink and set to zero if first.  */
#if PROFILE_MALLOC
			ctx->mem_alloc -= f->size;
#endif
			alloc_event(ctx, ALLOC_EV_FREE, ptr, f->size);
			if (prev) {
				prev->next = a->next;
				D(printf("free OK prev->next=%p\n",
						prev->next));
			} else {
				a->size = 0;
				D(printf("free OK a->size=0\n"));
			}
			return;
		}
		prev = a;
		a = a->next;
	}
	alloc_event(ctx, ALLOC_EV_FREE, ptr, 0);
	FATALD(printf("%s(%p) NOT FOUND!\n", __func__, ptr));
}

void free(void *ptr)
{
	_free_ctx(&_alloc_default_ctx, ptr);
}

void *_calloc_ctx(struct alloc_ctx *ctx, size_t nmemb, size_t size)
{
	void *p;

	p = _malloc_ctx(ctx, nmemb * size);
	if (p)
		memset(p, 0, nmemb * size);
	return p;
}

void *calloc(size_t nmemb, size_t size)
{
	return _calloc_ctx(&_alloc_default_ctx, nmemb, size);
}
