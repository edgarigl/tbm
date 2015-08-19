/*
 * Copyright (C) 2009 Edgar E. Iglesias <edgar@axis.com>
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "bins.h"
#include "ehash.h"

struct bin
{
	struct ehash_entry_t he; 

	const char *name;
	int accepted;
	unsigned long long hits;
	unsigned long long req_hits;
};

struct bin_state *bs_global = NULL;

static void exit_free(int exitcode, void *p)
{
	free(p);
}

void bin_init(struct bin_state *bs, enum bin_done_mode done_mode)
{
	if (bs->hash)
		return;

	bs->hash = ehash_new(50);
	assert(bs->hash);
	bs->done_mode = done_mode;
}

void bin_set_done_mode(struct bin_state *bs, enum bin_done_mode done_mode)
{
	assert(bs);
	bs->done_mode = done_mode;
}

static void bin_show(struct bin *b)
{
	int c = '-';

	if (b->hits)
		c = '/';

	if (b->accepted) {
		int overshot;

		c = '+';
		overshot = b->hits - b->req_hits;
		if (overshot > (b->hits / 2)) {
			c = '#';
		}
	}
	printf("%c req-hits=%8.8llu\thits=%8.8llu\t%s\n",
		c, b->req_hits, b->hits, b->name);
}

void bin_require(struct bin_state *bs,
		const char *name, unsigned long long req_hits)
{
	struct bin *b;

	b = calloc(1, sizeof *b);
	assert(b);
	b->name = strdup(name);
	b->req_hits = req_hits;

	/* Make sure it doesn't already exist.  */
	assert(!ehash_lookup(bs->hash, b->name, strlen(b->name) + 1, b, he));
	ehash_insert(bs->hash, b->name, strlen(b->name) + 1, b, he);

	/* Free at exit.  */
	on_exit(exit_free, (void *) b->name);
}

void bin_hit(struct bin_state *bs, const char *name)
{
	struct bin *b;
	int p_accepted;

	assert(bs);
	b = ehash_lookup(bs->hash, name, strlen(name) + 1, b, he);
	if (!b) {
		bin_require(bs, name, 0ULL);
		b = ehash_lookup(bs->hash, name, strlen(name) + 1, b, he);
		assert(b);
	}

	p_accepted = b->accepted;
	b->hits++;
	b->accepted = b->req_hits <= b->hits;
	if (b->accepted && !p_accepted)
		bin_show(b);
}

void bin_hitf(struct bin_state *bs, const char *fmt, ...)
{
	va_list ap;
	char *s;
	int r;

	assert(bs);
	va_start(ap, fmt);
	r = vasprintf(&s, fmt, ap);
	if (r > 0) {
		bin_hit(bs, s);
		free(s);
	}
	va_end(ap);
}

bool bin_alldone(struct bin_state *bs)
{
	struct ehash_iterator_t it;
	struct bin *b;
	unsigned char *key;
	size_t keylen;

	assert(bs);
	if (!bs->hash || bs->done_mode == FOREVER)
		return false;

	ehash_iterator_reset(&it);
	while ((b = ehash_iterate(bs->hash, &it, &key, &keylen, b, he))) {
		if (!b->hits && bs->done_mode == FAST)
			return false;
		if (b->hits < b->req_hits && bs->done_mode == NORMAL)
			return false;
	}
	return true;
}

void bin_show_status(struct bin_state *bs, int show_all)
{
	struct ehash_iterator_t it;
	struct bin *b;
	unsigned char *key;
	size_t keylen;

	if (!bs || !bs->hash)
		return;

	ehash_iterator_reset(&it);
	printf("\n");
	while ((b = ehash_iterate(bs->hash, &it, &key, &keylen, b, he))) {
		if (b->accepted) {
			bin_show(b);
		}
	}
	/* Still going, but hit at least once.  */
	ehash_iterator_reset(&it);
	while ((b = ehash_iterate(bs->hash, &it, &key, &keylen, b, he))) {
		if (!b->accepted && b->hits) {
			bin_show(b);
		}
	}
	/* Still going, never hit.  */
	ehash_iterator_reset(&it);
	while ((b = ehash_iterate(bs->hash, &it, &key, &keylen, b, he))) {
		if (!b->accepted && !b->hits) {
			bin_show(b);
		}
	}
	return;
}
