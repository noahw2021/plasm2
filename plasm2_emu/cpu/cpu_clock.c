//
//  cpu_clock.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"
#include "mmu/mmu.h"
#include "../decoder/decoder.h"
#include "../psin2/psin2.h"
#include "../emu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cpu_clock(void) {

	if ( 
		 !i->flags_s.HF // Do not clock if we are halted
		&& !i->flags_s.NF // Skip this cycle, due to no-execute
		) {
		time(&cpuctx->SystemSeconds);
	} else {
		if (i->flags_s.NF) {
			i->flags_s.NF = 0;
			int Psin2Id = Psin2iGetInstructionByOpcode(mmu_read1(i->ip));
			BYTE TotalRead = (Psin2iGetTotalSize(Psin2Id) / 8);
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

	BYTE ThisInstruction = mmu_read1(i->ip++);
	if (emuctx->DebuggerEnabled)
		decoder_go(ThisInstruction);

	Instructions[ThisInstruction]();

	return;
}
