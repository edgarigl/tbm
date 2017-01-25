/* FIXME: We shouldn't be ifdefing here.  */
#ifdef __xen__
#define GIC_DIST_BASE ((void *) 0x3001000)
#define GIC_CPU_BASE ((void *) 0x3002000)
#else
#define GIC_DIST_BASE ((void *) 0xf9010000)
#define GIC_CPU_BASE ((void *) 0xf9020000)
#endif

#define GICD_CTRL       0x0
#define GICD_IGROUPR    0x80
#define GICD_ISENABLER  0x100
#define GICD_ISPENDR    0x200
#define GICD_ISACTIVER  0x300
#define GICD_ITARGETSR  0x800
#define GICD_SGIR       0xf00
#define GICD_CPENDSGIR  0xf10

#define GICC_CTRL          0x0
#define GICC_CTRL_FIQ_EN   (1 << 3)

#define GICC_PMR        0x4
#define GICC_IAR	0x0c
#define GICC_EOIR	0x10
#define GICC_DIR	0x1000

