//
//  mmu_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

PMMU_CTX MmuCtx;

#pragma warning(disable: 6387)

void MmuInit(void) {
	MmuCtx = malloc(sizeof(MMU_CTX));
	memset(MmuCtx, 0, sizeof(MMU_CTX));

	CpuCtx->PhysicalMemorySize = PHYS_MEMSZ;
	CpuCtx->PhysicalMemory = malloc(CpuCtx->PhysicalMemorySize); 
    // PM usage good (reason: internal use only)
	memset(CpuCtx->PhysicalMemory, 0, PHYS_MEMSZ); 
    // PM usage good (reason: internal use only)

	ECtx->sp = 0x24F0;
    ECtx->ControlRegisters.StackPointerUpperBound = 0x25F0;
    ECtx->ControlRegisters.StackPointerLowerBound = 0x24F0;
    
    ECtx->ControlRegisters.PageStart = 0x2600;
    ECtx->ControlRegisters.PageEnd = 0x2600 + (256 * 0x20);
    MmuCtx->Pages = (PMMU_PAGE)CpuCtx->PhysicalMemory + 
        ECtx->ControlRegisters.PageStart;
    
	return;
}

void MmuShutdown(void) {
	free(CpuCtx->PhysicalMemory); 
    // PM usage good (reason: internal use only)
	free(MmuCtx);
}
