#ifndef __RDO_VERSION__
#define __RDO_VERSION__

#include <stdint.h>
#include "sys.h"
#include "reg-csu.h"

enum {
	RDO_SILICON = 0,
	RDO_REMUS = 1,
	RDO_VELOCE = 2,
	RDO_QEMU = 3
};

struct ronaldo_version
{
	union {
		struct {
			unsigned int ps_version : 4;
			unsigned int rtl_version : 8;
			unsigned int platform : 4;
			unsigned int platform_version : 5;
		};
		uint32_t u32;
	};
};

static inline struct ronaldo_version ronaldo_version(void)
{
	struct ronaldo_version v;
#ifdef __aarch64__
	const unsigned int current_el = aarch64_current_el();

	if (current_el == 3)
		v.u32 = readl(CSU_VERSION);
	else {
		// HAX, we should really be reading this out via EEMI.
		v.ps_version = 3;
		v.platform = RDO_SILICON;
	}
#else
	v.u32 = readl(CSU_VERSION);
#endif
	return v;
}

#endif
