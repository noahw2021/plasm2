//
//  mmup_misc.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../../cpu.h"
#include "../mmu.h"

void MmuSetPageTableStart(WORD64 New) {
	i->ControlRegisters.PageStart = New;
	MmuCtx->Pages = (void*)((BYTE*)CpuCtx->PhysicalMemory + i->ControlRegisters.PageStart); // PM usage good: (reason: trusted instruction)
	MmuCtx->MaxPageCount = (i->ControlRegisters.PageEnd - i->ControlRegisters.PageStart) / sizeof(MmuCtx->Pages[0]);
	return;
}

void MmuSetPageTableEnd(WORD64 New) {
	i->ControlRegisters.PageEnd = New;
	MmuCtx->Pages = (void*)((BYTE*)CpuCtx->PhysicalMemory + i->ControlRegisters.PageStart); // PM usage good: (reason: trusted instruction)
	MmuCtx->MaxPageCount = (i->ControlRegisters.PageEnd - i->ControlRegisters.PageStart) / sizeof(MmuCtx->Pages[0]);
	return;
}
