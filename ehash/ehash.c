/*
 * Thin hashtable.
 *
 * Copyright (C) 2010 Edgar E. Iglesias
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include <string.h>
#include <errno.h>

#include <ehash/ehash.h>

#define D(x)

static inline int clz32(uint32_t val)
{
#if (__GNUC__ < 4) && (__GNUC_MINOR__ < 3)
    int cnt = 0;

    /* Binary search for clz.  */
    if (!(val & 0xFFFF0000U)) {
        cnt += 16;
        val <<= 16;
    }
    if (!(val & 0xFF000000U)) {
        cnt += 8;
        val <<= 8;
    }
    if (!(val & 0xF0000000U)) {
        cnt += 4;
        val <<= 4;
    }
    if (!(val & 0xC0000000U)) {
        cnt += 2;
        val <<= 2;
    }
    if (!(val & 0x80000000U)) {
        cnt++;
        val <<= 1;
    }
    if (!(val & 0x80000000U)) {
        cnt++;
    }
    return cnt;
#else
    return __builtin_clz(val);
#endif
}
unsigned int roundup_pow2(unsigned int x)
{
	unsigned int rx;

	rx = (-1U >> (clz32(x)));
	rx++;
	return rx;
}

static unsigned int sdbm(const unsigned char *str, size_t len)
{
        size_t i;
        unsigned int hash = 0;
        int c;

        for (i = 0; i < len; i++)
        {
                c = *str;
                hash = c + (hash << 6) + (hash << 16) - hash;
                str++;
        }
        return hash;
}

static inline unsigned int ehash_index(struct ehash_t *hash,
				       const void *key, size_t keylen)
{
	unsigned int hi;

	hi = sdbm(key, keylen);

	/* hash->nr_buckets is always a power of 2.  */
	hi &= hash->nr_buckets - 1;
	return hi;
}

struct ehash_t *ehash_new(int buckets)
{
	struct ehash_t *eh;

	/* We see the buckets argument as a hint. Round it up to the nearest
	   power of 2. The number of buckets must always be a power of 2.
	   See ehash_index.  */
	if (buckets == 0)
		/* Sanity check and fallback to default.  */
		buckets = 16;
	else
		/* Roundup to nearest power of two.  */
		buckets = roundup_pow2(buckets);

	eh = calloc(1, sizeof *eh + (sizeof (eh->buckets[0]) * buckets));
	eh->nr_buckets = buckets;
	return eh;
}

void ehash_free(struct ehash_t *hash)
{
	int i;
	if (hash) {
		for (i = 0; i < hash->nr_buckets; i++)
		{
			struct ehash_entry_t *n;

			n = hash->buckets[i];
			while (n) {
				free(n->key);
				n = n->next;
			}
		}
		free (hash);
	}
}

void ehash_insert_e(struct ehash_t *hash,
		    const void *key, size_t keylen,
		    struct ehash_entry_t *e)
{
	unsigned int hi;

	if (!keylen)
		keylen = strlen((char *)key);

	hi = ehash_index(hash, key, keylen);

	/* dup the key.  */
	e->keylen = keylen;
	e->key = malloc (keylen);
	memcpy(e->key, key, keylen);
	e->next = hash->buckets[hi];
	hash->buckets[hi] = e;
	hash->objects++;
}

int ehash_remove(struct ehash_t *hash,
		 struct ehash_entry_t *e)
{
	struct ehash_entry_t *n;
	unsigned int hi;

	hi = ehash_index(hash, e->key, e->keylen);
	n = hash->buckets[hi];

	/* Fast match when e is the only or first guy in the bucket.  */
	if (n == e) {
		hash->objects--;
		hash->buckets[hi] = e->next;
		free(e->key);
		return 0;
	}

	while (n->next && n->next != e)
		n = n->next;

	if (n->next == e) {
		hash->objects--;
		n->next = e->next;
		free(e->key);
		return 0;
	}
	return -ENOENT;
}

void *ehash_lookup_e(struct ehash_t *hash,
		     const void *key, size_t keylen, off_t offset)
{
	struct ehash_entry_t *n;
	unsigned int hi;

	if (!keylen)
		keylen = strlen((char *)key);

	hi = ehash_index(hash, key, keylen);
	n = hash->buckets[hi];

	while (n
	       && !(keylen == n->keylen && memcmp(key, n->key, keylen) == 0))
		n = n->next;

	if (n)
		return (char *) n - offset;
	return NULL;
}

void *ehash_iterate_e(struct ehash_t *hash,
		      struct ehash_iterator_t *it,
		      void **key, size_t *keylen, off_t offset)
{
	struct ehash_entry_t *n;
	if (it->pos)
	{
		n = it->pos->next;
	}
	else
		n = hash->buckets[it->hi];

	while (!n && it->hi < hash->nr_buckets)
		n = hash->buckets[++it->hi];

	if (!n)
		return NULL;
	else {
		if (key)
			*key = n->key;
		if (keylen)
			*keylen = n->keylen;
		it->pos = n;
		return (char *) n - offset;
	}
}
