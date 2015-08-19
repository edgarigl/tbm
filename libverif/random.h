#ifndef __RANDOM_H__
#define __RANDOM_H__
struct random_dist
{
	unsigned int weight;
	unsigned int value;
};

void random_fill(unsigned int *seedp, void *buf, size_t len);
#define RANDOM_OBJ(s, x) random_fill(s, (&x), sizeof (x))

unsigned int random_dist_fill(unsigned int *seedp, struct random_dist *rd);

struct random_ctx;
struct random_ctx {
	struct random_ctx *next;
	const char *name;

	unsigned int checkpoint_seed;
	unsigned int seed;
};

void random_ctx_register(struct random_ctx *r);
void random_ctx_checkpoint_all(void);
void random_ctx_checkpoint(struct random_ctx *r);
void random_set_master_seed(unsigned int seed);
unsigned int random_get_master_seed(void);
unsigned int random_get_prev_master_seed(void);
static inline void random_ctx_init(struct random_ctx *r, const char *name)
{
	r->name = name;
	r->seed = 0;
	random_ctx_register(r);
}
#endif
