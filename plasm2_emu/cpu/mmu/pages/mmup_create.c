//
//  mmup_create.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../../cpu.h"
#include "../mmu.h"

WORD64 MmuCreatePage(WORD64 PhysicalAddress, WORD64 Size, BYTE Permissions) {
	if (MmuCtx->PageCount + 1 > MmuCtx->MaxPageCount) {
		CpuCsmSendMessage(CSM_PTFAILNOTSIZE, MmuCtx->PageCount + 1LLU);
		return 0;
	}
	
	MmuCtx->Pages[MmuCtx->PageCount].Permissions = Permissions;
	MmuCtx->Pages[MmuCtx->PageCount].Size = Size;
	MmuCtx->Pages[MmuCtx->PageCount].Physical = PhysicalAddress;
	MmuCtx->Pages[MmuCtx->PageCount].Virtual = i->ControlRegisters.VirtualStackPointer;
	MmuCtx->Pages[MmuCtx->PageCount].Active = 1;
	i->ControlRegisters.VirtualStackPointer += Size + 0x4200;

	MmuCtx->PageCount++;

	return MmuCtx->Pages[MmuCtx->PageCount - 1].Virtual;
}
