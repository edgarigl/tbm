/*
 * Code to supply random data
 *
 * Copyright (C) 2005 Edgar E. Iglesias <edgar@axis.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "random.h"

void random_fill(unsigned int *seedp, void *buf, size_t len) {
	unsigned char *d = buf;
	unsigned int i;

	for (i = 0; i < len; i++) {
		d[i] = rand_r(seedp);
	}
}

static inline unsigned int truncmax(unsigned int max, unsigned int value)
{
	unsigned int bit;

	for (bit = 31; bit > 0; bit--) {
		if (max & (1 << bit))
			break;
	}
	bit++;

	value &= (1 << bit) - 1;
	while (value >= max)
		value -= max;

	return value;
}

unsigned int random_dist_fill(unsigned int *seedp, struct random_dist *rd)
{
	unsigned int value;
	unsigned int max;
	unsigned int i;

	random_fill(seedp, &value, sizeof value);

	/* First pass to find max.  */
	i = 0; max = 0;
	while (rd[i].weight != ~0) {
		max += rd[i].weight;
		i++;
	}

	value = truncmax(max, value);

	i = 0;
	while (rd[i].weight != ~0) {
		max -= rd[i].weight;
		if (value >= max) {
			return rd[i].value;
		}
		i++;
	}
	printf("max=%d value=%d\n", max, value);
	assert(0);
	return 0;
}

static unsigned int master_seed[2];
static struct random_ctx *first_ctx;

void random_set_master_seed(unsigned int seed)
{
	master_seed[1] = master_seed[0];
	master_seed[0] = seed;
}

unsigned int random_get_master_seed(void)
{
	return master_seed[0];
}

unsigned int random_get_prev_master_seed(void)
{
	return master_seed[1];
}

void random_ctx_register(struct random_ctx *r)
{
	r->next = first_ctx;
	first_ctx = r;
}

void random_ctx_checkpoint(struct random_ctx *r)
{
	master_seed[1] = master_seed[0];
	r->checkpoint_seed = r->seed = rand_r(&master_seed[0]);
}

void random_ctx_checkpoint_all(void)
{
	struct random_ctx *r = first_ctx;

	while (r) {
		random_ctx_checkpoint(r);
		r = r->next;
	}
}
