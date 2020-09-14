#ifndef __BINS_H__
#define __BINS_H__
enum bin_done_mode {
	NORMAL,		/* Run until all bin targets are hit.  */
	FOREVER,	/* Run forever.  */
	FAST		/* Run run until each bin is hit noce.  */
};

struct bin_state {
	void *hash;

	enum bin_done_mode done_mode;
};

void bin_init(struct bin_state *bs, enum bin_done_mode done_mode);
void bin_set_done_mode(struct bin_state *bs, enum bin_done_mode);
void bin_require(struct bin_state *bs,
		const char *name, unsigned long long req_hits);
void bin_hit(struct bin_state *bs, const char *name);
void bin_hitf(struct bin_state *bs, const char *fmt, ...)
		__attribute__((format(printf, 2, 3)));
bool bin_alldone(struct bin_state *bs);
void bin_show_status(struct bin_state *bs, int show_all);

/* This hack is useful for ctrl+c handling.  */
extern struct bin_state *bs_global;
static inline void bin_set_bs_global(struct bin_state *bs)
{
	bs_global = bs;
}

static inline struct bin_state *bin_get_bs_global(void)
{
	return bs_global;
}
#endif
