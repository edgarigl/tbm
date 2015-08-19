#ifndef __DEVTREE_H__
#define __DEVTREE_H__
#include <libfdt/libfdt.h>

static inline const void *dt_get_property(void *fdt, int offset,
				const char *name, int *plen) {
	const struct fdt_property *prop;

	prop = fdt_get_property(fdt, offset, name, plen);
	return prop ? prop->data : NULL;
}

struct fdt_match {
	const char *compat;
};

struct fdt_probe {
	bool (*probe)(void *fdt, int offset, const char *compat);
	struct fdt_match *match;
};

void *dt_map(void *fdt, int node, int map_nr);
void devtree_setup(void *fdt);

#define __fdt_probe(x) \
struct fdt_probe *_fdt_probes ## x __attribute__((section (".fdt_probe"), used)) = &x
#endif
