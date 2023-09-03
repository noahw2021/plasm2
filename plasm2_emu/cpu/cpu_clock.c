#include "cpu.h"
#include "mmu/mmu.h"
#include "../decoder/decoder.h"
#include "../psin2/psin2.h"
#include "../emu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
cpu_clock.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void cpu_clock(void) {

	if ( 
		 !i->flags_s.HF // Do not clock if we are halted
		&& !i->flags_s.NF // Skip this cycle, due to no-execute
		) {
		time(&cpuctx->SystemSeconds);
	} else {
		if (i->flags_s.NF) {
			i->flags_s.NF = 0;
			int Psin2Id = psin2i_getinstructionbycd(mmu_read1(i->ip));
			byte opc = mmu_read1(i->ip);
			byte TotalRead = (psin2i_totalsize(Psin2Id) / 8);
			i->ip += TotalRead;
		}
		return;
	}

	if (i->flags_s.VF &&
		i->flags_s.MF &&
		i->ip >= i->pti.pml
		) {
		cpui_csm_msg(CSM_XPAGETOOSMALL, i->ip);
	}

	byte ThisInstruction = mmu_read1(i->ip++);
	if (emuctx->DebuggerEnabled)
 		decoder_go(ThisInstruction);

	Instructions[ThisInstruction]();

	return;


}