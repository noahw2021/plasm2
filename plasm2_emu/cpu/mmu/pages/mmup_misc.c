//
//  mmup_misc.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../../cpu.h"
#include "../mmu.h"

void MmuSetPageTableStart(WORD64 New) {
	ECtx->ControlRegisters.PageStart = New;
	MmuCtx->Pages = (void*)((BYTE*)CpuCtx->PhysicalMemory + ECtx->ControlRegisters.PageStart); // PM usage good: (reason: trusted instruction)
	MmuCtx->MaxPageCount = (ECtx->ControlRegisters.PageEnd - ECtx->ControlRegisters.PageStart) / sizeof(MmuCtx->Pages[0]);
	return;
}

void MmuSetPageTableEnd(WORD64 New) {
	ECtx->ControlRegisters.PageEnd = New;
	MmuCtx->Pages = (void*)((BYTE*)CpuCtx->PhysicalMemory + ECtx->ControlRegisters.PageStart); // PM usage good: (reason: trusted instruction)
	MmuCtx->MaxPageCount = (ECtx->ControlRegisters.PageEnd - ECtx->ControlRegisters.PageStart) / sizeof(MmuCtx->Pages[0]);
	return;
}
