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
	
    PMMU_PAGE ThisPage = &MmuCtx->Pages[MmuCtx->PageCount];
    MmuCtx->PageCount++;
    
	ThisPage->Flags.Permissions = Permissions;
	ThisPage->Size = Size;
	ThisPage->Physical = PhysicalAddress;
	ThisPage->Virtual = ECtx->ControlRegisters.VirtualStackPointer;
	ThisPage->Flags.Active = 1;
	ECtx->ControlRegisters.VirtualStackPointer += Size + 0x4200;

    return ThisPage->Virtual;
}
