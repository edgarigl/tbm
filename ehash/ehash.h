#include <stdint.h>
#include <stddef.h>

#include  <sys/types.h>

struct ehash_entry_t
{
	struct ehash_entry_t *next;
	void *key;
	size_t keylen;
};

struct ehash_t
{
	/* Nr of objects in the table.  */
	size_t objects;
	/* Nr of buckets.  */
	size_t nr_buckets;
	struct ehash_entry_t * buckets[1];
};

struct ehash_iterator_t
{
	unsigned int hi;
	struct ehash_entry_t *pos;
};

struct ehash_t *ehash_new(int buckets);
void ehash_free(struct ehash_t *eh);
void ehash_insert_e(struct ehash_t *hash,
		    const void *k, size_t klen,
		    struct ehash_entry_t *e);
int ehash_remove(struct ehash_t *hash,
		 struct ehash_entry_t *e);
void *ehash_lookup_e(struct ehash_t *hash,
		     const void *key, size_t keylen, off_t offset);

static inline size_t ehash_size(struct ehash_t *hash)
{
	return hash->objects;
}

static inline void ehash_iterator_reset(struct ehash_iterator_t *i)
{
	i->hi = 0;
	i->pos = 0;
}
void *ehash_iterate_e(struct ehash_t *hash,
		      struct ehash_iterator_t *it,
		      void **key, size_t *keylen, off_t offset);

#define ehash_insert(h, k, klen, entry, member) \
	ehash_insert_e((h), (k), (klen), &(entry)->member)

#define ehash_lookup(h, k, klen, e, member) \
	ehash_lookup_e((h), (k), (klen), offsetof(typeof (*e), member))

#define ehash_iterate(h, it, kp, klenp, e, member) \
	ehash_iterate_e((h), (it), \
			(void*) (kp), (klenp), \
			offsetof(typeof (*e), member))
