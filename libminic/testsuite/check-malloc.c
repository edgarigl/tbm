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

#include <inttypes.h>
#include <assert.h>

#ifndef HAVE_ROM
#define FATALD(x)
#define PROFILE_MALLOC 1
#endif

#define alloc_init dut_alloc_init
#define alloc_exit dut_alloc_exit
#define _alloc_set_event_cb dut_alloc_set_event_cb
#define aligned_alloc dut_aligned_alloc
#define malloc dut_malloc
#define free dut_free
#define calloc dut_calloc
#include "../malloc.c"
#undef aligned_alloc
#undef malloc
#undef free
#undef calloc

#define fail_unless(x)                                         \
do                                                             \
{                                                              \
  if (!(x)) {                                                  \
    fprintf (stderr, "FAILED at %s:%d\n", __FILE__, __LINE__); \
    exit (EXIT_FAILURE);                                       \
  }                                                            \
} while (0);

#ifndef TORTURE_ORDER
#define TORTURE_ORDER	1024
#endif

unsigned int pre_guard;
unsigned int heap[4 * 1024 * 1024 / sizeof(int)];
unsigned int post_guard;

static void check_aligned_alloc(void)
{
	int i, sizep2;
	unsigned int level;

	printf("%s\n", __func__);
	dut_alloc_init(heap, sizeof heap);
	level = _alloc_default_ctx.mem_alloc;

	for (sizep2 = 2; sizep2 < 19; sizep2++) {
		for (i = 0; i < 100; i++) {
			unsigned int size = 1U << sizep2;
			void *p[3];
			unsigned long pi;
			unsigned int a;

			for (a = 0; a < (sizeof p / sizeof p[0]); a++) {
				p[a] = dut_aligned_alloc(size, size);
				assert(p[a]);
				pi = (unsigned long) p[a];
				assert((pi & (size - 1)) == 0);
			}

			for (a = 0; a < (sizeof p / sizeof p[0]); a++) {
				dut_free(p[a]);
			}
			assert(_alloc_default_ctx.mem_alloc == level);
		}
		putchar('.');
	}
	dut_alloc_exit();
	assert(_alloc_default_ctx.mem_alloc == 0);
	printf("%s OK\n", __func__);
}

static void check_small_alloc(void)
{
        int i, size;
        unsigned int level;

	printf("%s\n", __func__);
	dut_alloc_init(heap, sizeof heap);
	level = _alloc_default_ctx.mem_alloc;

        for (size = 1; size < 32; size++) {
                for (i = 0; i < 100; i++) {
                        void *p = dut_malloc(4);
                        assert(p);
                        dut_free(p);
                        assert(_alloc_default_ctx.mem_alloc == level);
                }
		putchar('.');
        }
	dut_alloc_exit();
	assert(_alloc_default_ctx.mem_alloc == 0);
	printf("%s OK\n", __func__);
}

static void check_malloc_zero(void)
{
	void *p;
        unsigned int level;

	printf("%s\n", __func__);
	dut_alloc_init(heap, sizeof heap);
	level = _alloc_default_ctx.mem_alloc;

	p = dut_malloc(0);
	assert(p);
	assert(_alloc_default_ctx.mem_alloc > level);
	dut_free(p);
	assert(_alloc_default_ctx.mem_alloc == level);

	dut_alloc_exit();
	assert(_alloc_default_ctx.mem_alloc == 0);
	printf("%s OK\n", __func__);
}

static void check_calloc(void)
{
	unsigned char *p;
	unsigned int i, j;

	printf("%s\n", __func__);
	dut_alloc_init(heap, sizeof heap);

	for (i = 16; i < (sizeof heap - 16) && i < 16 * 1024; i++) {
		p = dut_calloc(1, i);
		if (!p)
			break;
		for (j = 0; j < i; j++)
			assert(!p[j]);
		dut_free(p);
	}

	dut_alloc_exit();
	assert(_alloc_default_ctx.mem_alloc == 0);
	printf("%s OK\n", __func__);
}

static void check_torture(unsigned int modulo)
{
	unsigned int seed = 0;
	unsigned long int i;
	unsigned int len, pos = 0;
	unsigned int aligned;
	void *p_fifo[16] = {0};

	printf("%s seed=%x mod=%d\n", __func__, seed, modulo);
	dut_alloc_init(heap, sizeof heap);

	for (i = 0; i < (64000 * TORTURE_ORDER); i++) {
		len = rand_r(&seed) % modulo;
		aligned = rand_r(&seed) % 19;
		if (aligned < 3) {
			aligned = 0;
		} else {
			aligned = 1U << aligned;
		}

		p_fifo[pos] = dut_aligned_alloc(aligned, len);
		D(printf("p[%d] = alloc_aligned(%d, %d) = %p\n",
				pos, aligned, len, p_fifo[pos]));
		if ((i % (128 * TORTURE_ORDER)) == 0)
			fputc(p_fifo[pos] ? '+' : '.', stderr);

		/* Make sure to write in the entire area!  */
		if (p_fifo[pos])
			memset(p_fifo[pos], 0xed, len);

		/* Make sure the ptr is 32bit aligned.  */
		assert((((intptr_t) p_fifo[pos]) & 3) == 0);

		pos++;
		pos &= 15;

		/* Delayed free.  if i == 14, then pos == 15 at this point. */
		if (i >= 14) {
			unsigned int pi = (pos + 1) & 15;
			dut_free(p_fifo[pi]);
			D(printf("free p[%d] = %p\n", pi, p_fifo[pi]));
			p_fifo[pi] = NULL;
		}
	}

	pos = 0;
	for (i = 0; i < 16; i++) {
		dut_free(p_fifo[pos]);
		D(printf("free p[%d] = %p\n", pos, p_fifo[pos]));
		p_fifo[pos] = NULL;
		pos++;
	}

	printf("\n");
	dut_alloc_exit();
	assert(_alloc_default_ctx.mem_alloc == 0);
	printf("%s OK\n", __func__);
}

static int latest_type;
static void *latest_p;
static size_t latest_a;

static void ev_handler(int type, void *p, size_t a)
{
#if 0
	printf("type =%d\n", type);
	printf("p    =%p\n", p);
	printf("a    =%ld\n", (unsigned long) a);
#endif
	latest_type = type;
	latest_p = p;
	latest_a = a;
}

#define ASSERT_EV(type, p)        \
{                                      \
	assert(latest_type == type);   \
	assert(latest_p == p);         \
}

static void check_events(void)
{
	void *p, *p2;
	unsigned int level;

	printf("%s\n", __func__);
	dut_alloc_init(heap, sizeof heap);
	dut_alloc_set_event_cb(ev_handler);

	level = _alloc_default_ctx.mem_alloc;

	ev_handler(-1, 0, ~0);
	p = dut_malloc(1024);
	printf("level=%d latest_a=%ld\n", level, latest_a);
	ASSERT_EV(ALLOC_EV_ALLOC_LEVEL, p);
	assert(latest_a >= 1024);

	p2 = dut_malloc(1024);
	ASSERT_EV(ALLOC_EV_ALLOC_LEVEL, p2);
	assert(latest_a >= 2 * 1024);

	ev_handler(-1, NULL, ~0);
	dut_free(p2);
	/* No event.  */
	ASSERT_EV(ALLOC_EV_FREE, p2);
	dut_free(p);
	ASSERT_EV(ALLOC_EV_FREE, p);

	/* Alloc an amount that can't be statisfied.  */
	p = dut_malloc(sizeof heap + 1);
	assert(p == NULL);
	ASSERT_EV(ALLOC_EV_ALLOC, NULL);
	assert(latest_a >= sizeof heap + 1);

	/* Free of NULL is ok.  */
	ev_handler(-1, NULL, ~0);
	dut_free(NULL);
	ASSERT_EV(-1, NULL);

	ev_handler(-1, NULL, ~0);
	p = dut_malloc(0);
	assert(p); /* minic's malloc returns valid ptrs.  */
	ASSERT_EV(ALLOC_EV_ALLOC, p);
	assert(latest_a == 0);

	dut_alloc_exit();
	/* Unregister.  */
	dut_alloc_set_event_cb(NULL);
	printf("%s OK\n", __func__);
}

int main(void)
{
	unsigned int i;

	check_small_alloc();
	check_aligned_alloc();
	check_events();
	check_small_alloc();
	check_malloc_zero();
	check_calloc();

	check_torture(3);
	check_torture(5);
	check_torture(7);
	for (i = 8; i < (16 * 1024); i = i * 2)
		check_torture(i);
	check_torture(9 * 1024);
	return 0;
}
