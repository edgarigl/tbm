#if !defined(GIC_DIST_BASE_ADDR) || !defined(GIC_CPU_BASE_ADDR)
# error Please specify GIC_DIST_BASE_ADDR and GIC_CPU_BASE_ADDR in the configuration file
#endif

#define GIC_DIST_BASE ((void *) GIC_DIST_BASE_ADDR)
#define GIC_CPU_BASE ((void *) GIC_CPU_BASE_ADDR)

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
#define GICC_DIR    0x1000

