#include "reg-smmu.h"

#define R_SMMU500_SMMU_GATS1PR	(0x110)
#define R_SMMU500_SMMU_GATS1PW	(0x118)
#define R_SMMU500_SMMU_GATS12PR	(0x130)
#define R_SMMU500_SMMU_GATS12PW	(0x138)
#define R_SMMU500_SMMU_GPAR	(0x180)
#define R_SMMU500_SMMU_GATSR	(0x188)

bool smmu_map(bool s1, bool s2, bool write, uintptr_t va, uintptr_t *pa);
void smmu_init_ctx(struct mmu_ctx *s1_mmu, struct mmu_ctx *s2_mmu,
                        unsigned int s1_cb, unsigned int s2_cb,
                        unsigned int el, bool enable);
