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

void sdbg_send(void) {
	memset(sdbgctx->CollectionBufferOut, 0, 256);
	
	byte* RealPointer = (byte*)cpuctx->PhysicalMemory + mmu_translate(sdbgctx->VirtualPointer, REASON_READ, sdbgctx->VirtualSize);
	if (RealPointer == cpuctx->PhysicalMemory) { // failed 
		sdbgctx->Active = 1;
		return;
	}
	memcpy(sdbgctx->CollectionBufferOut, RealPointer, sdbgctx->VirtualSize);

	sdbgctx->Active = 1;
}

void sdbg_setsendloc(u64 Location) {
	sdbgctx->VirtualPointer = Location;
	return;
}

void sdbg_setsendsz(u64 Size) {
	sdbgctx->VirtualSize = Size;
	return;
}
