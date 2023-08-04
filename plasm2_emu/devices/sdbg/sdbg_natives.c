#include "sdbg.h"
#include "../cpu/cpu.h"
#include "../cpu/mmu/mmu.h"
/*
sdbg_natives.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void sdbg_send(void) {
	memset(sdbgctx->CollectionBufferOut, 0, 256);
	
	byte* RealPointer = (byte*)cpuctx->PhysicalMemory + mmu_translate(sdbgctx->VirtualPointer, REASON_READ);
	if (sdbgctx->VirtualSize > mmu_maxaddr(sdbgctx->VirtualSize, REASON_READ))
		return;
	memcpy(sdbgctx->CollectionBufferOut, RealPointer, sdbgctx->VirtualSize);

	sdbgctx->Active = 1;
}

void sdbg_setsendloc(u64 Location) {

}

void sdbg_setsendsz(u64 Size) {

}