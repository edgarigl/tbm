#define SMC_ATOMIC	0x80000000
#define SMC_A64		0x40000000

#define SMC_OWNER_OEM	0x02000000

#define SMC_TBM_BASE		(SMC_OWNER_OEM)
#define SMC_TBM_NOT		(SMC_TBM_BASE | 0)
#define SMC_TBM_RAISE_EL	(SMC_TBM_BASE | 1)
#define SMC_TBM_MAX		(SMC_TBM_RAISE_EL)

#define SMC_PSCI_BASE		0
#define SMC_PSCI_CPU_SUSPEND	(0xc4000001)
#define SMC_PSCI_CPU_OFF	(0x84000002)
#define SMC_PSCI_CPU_ON		(0xc4000003)
#define SMC_PSCI_MAX		SMC_PSCI_CPU_ON

#define PM_SIP_SVC                      0xC2000000

/* Reset configuration argument */
#define PM_RESET_ACTION_RELEASE 0U
#define PM_RESET_ACTION_ASSERT  1U
#define PM_RESET_ACTION_PULSE   2U


#define PSCI_E_SUCCESS          0
#define PSCI_E_NOT_SUPPORTED    -1
#define PSCI_E_INVALID_PARAMS   -2
#define PSCI_E_DENIED           -3
#define PSCI_E_ALREADY_ON       -4
#define PSCI_E_ON_PENDING       -5
#define PSCI_E_INTERN_FAIL      -6
#define PSCI_E_NOT_PRESENT      -7
#define PSCI_E_DISABLED         -8
#define PSCI_E_INVALID_ADDRESS  -9

#define PSCI_SUCCESS 0


