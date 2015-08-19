#include <stdio.h>
#include <stdbool.h>
#include <libfdt.h>
#include <devtree.h>

extern struct fdt_probe *__fdt_probe_start;
extern struct fdt_probe *__fdt_probe_end;

/* FIXME: Optimize.  */
int32_t dt_nr_cells(void *fdt, int node, const char *name)
{
	const int32_t *nr_cells;

	node = fdt_parent_offset(fdt, node);
	do {
		int lenp;
		nr_cells = dt_get_property(fdt, node, name, &lenp);
		if (nr_cells) {
			return fdt32_to_cpu(*nr_cells);
		}

		node = fdt_parent_offset(fdt, node);
	} while (node >= 0);

	return -1;
}

int32_t dt_nr_address_cells(void *fdt, int node)
{
	int32_t r;
	r = dt_nr_cells(fdt, node, "#address-cells");
	return r == -1 ? 1 : r;
}

int32_t dt_nr_size_cells(void *fdt, int node)
{
	int32_t r;
	r = dt_nr_cells(fdt, node, "#size-cells");
	return r == -1 ? 1 : r;
}

void *dt_map(void *fdt, int node, int map_nr)
{
	int32_t addr_cells = dt_nr_address_cells(fdt, node);;
	int32_t size_cells = dt_nr_size_cells(fdt, node);
	const struct fdt_property *prop;
	uint32_t *data;
	uint64_t addr = 0;
	uint64_t size = 0;
	unsigned int nr_of_mappings;
	unsigned int i;
	int len;

	prop = fdt_get_property(fdt, node, "reg", &len);
	if (!prop)
		return NULL;
	len /= 4;
	nr_of_mappings = len / (addr_cells + size_cells);
	if (map_nr >= nr_of_mappings)
		return NULL;

	data = (void *) prop->data;
	data += map_nr * (addr_cells + size_cells);
	for (i = 0; i < addr_cells; i++) {
		addr <<= 32;
		addr |= fdt32_to_cpu(*data++);
	}
	for (i = 0; i < size_cells; i++) {
		size <<= 32;
		size |= fdt32_to_cpu(*data++);
	}
	return (void *) addr;
}

void dt_scan_compat(void *fdt, int offset, const char *compat)
{
	struct fdt_probe **pos = &__fdt_probe_start;
	struct fdt_probe **end = &__fdt_probe_end;

	while (pos < end) {
		struct fdt_probe *f = *pos;
		bool match = false;
		unsigned int i = 0;

		while (f->match[i].compat) {
//			printf("cmp %s\n", compat);
			if (strcmp(compat, f->match[i].compat) == 0) {
				match = true;
				break;
			}
			i++;
		}
		if (match) {
			match = f->probe(fdt, offset, compat);
			if (match) {
				return;
			}
		}
		pos++;
	}
}

void dt_scan_compat_strings(void *fdt, int offset,
			const struct fdt_property *prop, int len)
{
	const char *str = prop->data;
	const char *end = prop->data + len;
	unsigned int i = 0;

	do {
		dt_scan_compat(fdt, offset, str);
		str += strlen(str) + 1;
		i++;
	} while (str < end);
}

void dt_scan_level(void *fdt)
{
	int offset = 0, depth = 0;
	const struct fdt_property *prop;
	int len;

	offset = fdt_next_node(fdt, offset, &depth);
	while (offset >= 0 && depth >= 0) {
		prop = fdt_get_property(fdt, offset, "compatible", &len);
		if (prop) {
			dt_scan_compat_strings(fdt, offset, prop, len);
		}
		offset = fdt_next_node(fdt, offset, &depth);
	}
}

void devtree_setup(void *fdt)
{
	dt_scan_level(fdt);
}
