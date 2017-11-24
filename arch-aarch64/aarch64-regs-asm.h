#define HCR_VM        0x1
#define HCR_SWIO      0x2
#define HCR_PTW       0x4
#define HCR_FMO       0x8
#define HCR_IMO       0x10
#define HCR_AMO       0x20
#define HCR_VF        0x40
#define HCR_VI        0x80
#define HCR_VSE       0x100
#define HCR_FB        0x200
#define HCR_DC        0x1000
#define HCR_TWI       0x2000
#define HCR_TWE       0x4000
#define HCR_TID0      0x8000
#define HCR_TID1      0x10000
#define HCR_TID2      0x20000
#define HCR_TID3      0x40000
#define HCR_TSC       0x80000
#define HCR_TIDCP     0x100000
#define HCR_TACR      0x200000
#define HCR_TSW       0x400000
#define HCR_TPC       0x800000
#define HCR_TPU       0x1000000
#define HCR_TTLB      0x2000000
#define HCR_TVM       0x4000000
#define HCR_TGE       0x8000000
#define HCR_TDZ       0x10000000
#define HCR_HCD       0x20000000
#define HCR_TRVM      0x40000000
#define HCR_RW        0x80000000
/* Only avalable in C.  */
#define HCR_CD        (1ULL << 32)
#define HCR_ID        (1ULL << 33)

#define SCR_NS        (1U << 0)
#define SCR_IRQ       (1U << 1)
#define SCR_FIQ       (1U << 2)
#define SCR_EA        (1U << 3)
#define SCR_SMD       (1U << 7)
#define SCR_HCE       (1U << 8)
#define SCR_SIF       (1U << 9)
#define SCR_RW        (1U << 10)
#define SCR_ST        (1U << 11)
#define SCR_TWI       (1U << 12)
#define SCR_TWE       (1U << 13)

#define SCTLR_M       (1U << 0)
#define SCTLR_A       (1U << 1)
#define SCTLR_C       (1U << 2)
#define SCTLR_SA      (1U << 3)
#define SCTLR_I       (1U << 12)

#define PAR_F		(1 << 0)
#define PAR_PTW		(1 << 8)
#define PAR_S		(1 << 9)

