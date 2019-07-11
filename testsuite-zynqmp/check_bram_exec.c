/*

This test-code excersizes execution from a BRAM on the PL
or on the SystemC side of a Cosim setup.

Copyright (c) 2019, Xilinx Inc.
Written by Edgar E. Iglesias <edgar.iglesias@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#include <stdio.h>
#include <stdint.h>
#include "sys.h"
#include "testcalls.h"

static void a64_emit_add_x0_x0_1(uint32_t *mem)
{
	*mem = 0x91000400;
}

static void a64_emit_ret(uint32_t *mem)
{
	*mem = 0xd65f03c0;
}

void check_bram_exec(void)
{
	uint32_t (*func)(uint32_t x) = (void *)0xa0460000;
	uint32_t *insn = (uint32_t *) 0xa0460000;
	uint32_t val = 0;
	/* Ram size is 0x400. Each insn is 4 bytes.  */
	int max_insns = 0x400 / 4;
	int i;

	insn[0] = 0xdead;
	barrier();
	if (insn[0] != 0xdead) {
		printf("COSIM not available, no ram at %p\n", insn);
		return;
	}

	/* Emit a large function with max_insns - 1 worth of code.
	 * Leave one slot for the ret insn.  */
	for (i = 0; i < max_insns - 1; i++) {
		a64_emit_add_x0_x0_1(insn + i);
	}
	a64_emit_ret(insn + i);

	/* Run the code and check the result.  */
	val = func(val);
	printf("val=%d\n", val);
	assert(val == max_insns - 1);
}
