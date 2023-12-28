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
	if (ECtx->Security.SecurityLevel < 2)
		ECtx->ControlRegisters.CSMHandler = Handler;
	else
		ECtx->Flags.XF = 1;
    
	return;
}

void CpuCsmSendMessage(BYTE Code, WORD64 AddtData) {
	if (ECtx->Security.SecurityLevel >= 3)
		ECtx->Flags.XF = 1;
	
    for (int c = 0; c < REGCOUNT_SPEC; c++)
		MmuPush(ECtx->SystemRs[c]);
	
    MmuPush(Code);
	MmuPush(AddtData);
	CpuInstructionCLL(ECtx->ControlRegisters.CSMHandler);
    
	return;
}
