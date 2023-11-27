#include "../../cpu.h"
#include "../mmu.h"
/*
mmup_create.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

WORD64 mmu_createpage(WORD64 PhysicalAddress, WORD64 Size, BYTE Permissions) {
	if (mmuctx->PageCount + 1 > mmuctx->MaxPageCount) {
		CpuCsmSendMessage(CSM_PTFAILNOTSIZE, mmuctx->PageCount + 1LLU);
		return 0;
	}
	
	mmuctx->Pages[mmuctx->PageCount].Permissions = Permissions;
	mmuctx->Pages[mmuctx->PageCount].Size = Size;
	mmuctx->Pages[mmuctx->PageCount].Physical = PhysicalAddress;
	mmuctx->Pages[mmuctx->PageCount].Virtual = i->pti.vsp;
	mmuctx->Pages[mmuctx->PageCount].Active = 1;
	i->pti.vsp += Size + 0x4200;

	mmuctx->PageCount++;

	return mmuctx->Pages[mmuctx->PageCount - 1].Virtual;
}
