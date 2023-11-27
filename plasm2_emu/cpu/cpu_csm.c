//
//  cpu_csm.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>

void CpuCsmSetHandler(WORD64 Handler) {
	if (i->security_s.SecurityLevel < 2)
		i->pti.csm = Handler;
	else
		i->flags_s.XF = 1;
	return;
}

void CpuCsmSendMessage(BYTE Code, WORD64 AddtData) {
	if (i->security_s.SecurityLevel >= 3)
		i->flags_s.XF = 1;
	for (int c = 0; c < REGCOUNT_SPEC; c++)
		mmu_push(i->rs_spec[c]);
	mmu_push(Code);
	mmu_push(AddtData);
	CpuInstructionCLL(i->pti.csm);
	return;
}
