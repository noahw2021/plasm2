//
//  sdbg_natives.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "sdbg.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <string.h>

void SdbgSend(void) {
	memset(sdbgctx->CollectionBufferOut, 0, 256);
	
	BYTE* RealPointer = (BYTE*)CpuCtx->PhysicalMemory + MmuTranslate(sdbgctx->VirtualPointer, REASON_READ, sdbgctx->VirtualSize);
	if (RealPointer == CpuCtx->PhysicalMemory) { // failed 
		sdbgctx->Active = 1;
		return;
	}
	memcpy(sdbgctx->CollectionBufferOut, RealPointer, sdbgctx->VirtualSize);

	sdbgctx->Active = 1;
}

void SdbgSetLocation(WORD64 Location) {
	sdbgctx->VirtualPointer = Location;
	return;
}

void SdbgSetSize(WORD64 Size) {
	sdbgctx->VirtualSize = Size;
	return;
}
