#if !defined(GIC_DIST_BASE_ADDR) || !defined(GIC_CPU_BASE_ADDR)
# error Please specify GIC_DIST_BASE_ADDR and GIC_CPU_BASE_ADDR in the configuration file
#endif

#define GIC_DIST_BASE GIC_DIST_BASE_ADDR
#define GIC_CPU_BASE GIC_CPU_BASE_ADDR
#ifdef GIC_VIFACE_BASE_ADDR
#define GIC_VIFACE_BASE GIC_VIFACE_BASE_ADDR
#endif
#ifdef GIC_VCPU_BASE_ADDR
#define GIC_VCPU_BASE GIC_VCPU_BASE_ADDR
#endif

#define GICD_CTRL       0x0
#define GICD_IGROUPR    0x80
#define GICD_ISENABLER  0x100
#define GICD_ICENABLER  0x180
#define GICD_ISPENDR    0x200
#define GICD_ISACTIVER  0x300
#define GICD_IPRIORITYR 0x400
#define GICD_ITARGETSR  0x800
#define GICD_SGIR       0xf00
#define GICD_CPENDSGIR  0xf10

#define GICC_CTRL          0x0
#define GICC_CTRL_FIQ_EN   (1 << 3)

#define GICC_PMR    0x4
#define GICC_BPR    0x8
#define GICC_IAR    0x0c
#define GICC_EOIR   0x10
#define GICC_RPR    0x14
#define GICC_ABPR   0x1c
#define GICC_AIAR   0x20
#define GICC_APR    0xd0
#define GICC_IIDR   0xfc
#define GICC_DIR    0x1000

#define GICH_HCR    0x000
#define GICH_VTR    0x004
#define GICH_VMCR   0x008
#define GICH_MISR   0x010
#define GICH_EISR0  0x020
#define GICH_EISR1  0x024
#define GICH_ELSR0  0x030
#define GICH_ELSR1  0x034
#define GICH_LR     0x100

#define GICH_MISR_EOI     (1 << 0)
#define GICH_MISR_U       (1 << 1)
#define GICH_MISR_LRENP   (1 << 2)
#define GICH_MISR_NP      (1 << 3)
#define GICH_MISR_VGRP0E  (1 << 4)
#define GICH_MISR_VGRP0D  (1 << 5)
#define GICH_MISR_VGRP1E  (1 << 6)
#define GICH_MISR_VGRP1D  (1 << 7)
