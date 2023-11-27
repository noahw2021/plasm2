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
	if (i->Security.SecurityLevel < 2)
		i->ControlRegisters.CSMHandler = Handler;
	else
		i->flags_s.XF = 1;
	return;
}

void CpuCsmSendMessage(BYTE Code, WORD64 AddtData) {
	if (i->Security.SecurityLevel >= 3)
		i->flags_s.XF = 1;
	for (int c = 0; c < REGCOUNT_SPEC; c++)
		MmuPush(i->rs_spec[c]);
	MmuPush(Code);
	MmuPush(AddtData);
	CpuInstructionCLL(i->ControlRegisters.CSMHandler);
	return;
}
