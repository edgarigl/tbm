#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "sys.h"
#include "testcalls.h"
#include "ronaldo-version.h"
#include "reg-crf.h"

static int counter = 0;
bool touchdown;

void _secondary_entry_el1(void);

void secondary_start(uint64_t x0)
{
	uint64_t r;

	touchdown = true;
	mb();
	ibarrier();
//	printf("secondary cnt=%d\n", counter);
	if ((counter & 15) == 0)
		printf("+");
	while (1) {
		/* Ready to go back to RESET.  */
		r = aarch64_smc(SMC_PSCI_CPU_OFF, 0,
				0, 0, 0, 0, 0, 0);
		assert(r == PSCI_E_SUCCESS);
		cpu_wfi();
	}
}

void check_psci_core(int cpu)
{
	uint64_t r;
	touchdown = false;
	mb();
	ibarrier();

	do {
		r = aarch64_smc(SMC_PSCI_CPU_ON, cpu,
				(uintptr_t)_secondary_entry_el1,
				0, 0, 0, 0, 0);
		if (r != PSCI_E_SUCCESS && r != PSCI_E_ALREADY_ON)
			printf("-");
	} while (r != PSCI_E_SUCCESS);
 
//	printf("SMC=%llx Wait for secondary %d\n", r, counter);
	while (!touchdown) {
		__asm__ __volatile__("yield");
		mb();
	}
	counter++;
}

void check_psci(void) {
	int i;

	printf("%s\n", __func__);
	check_psci_core(1);
	check_psci_core(2);
	check_psci_core(3);
	for (i = 0; i < 2 * 1000; i++) {
		unsigned int cpu = (rand() % 3) + 1;
		check_psci_core(cpu);
	}
	printf("\n%s Done\n", __func__);
}
