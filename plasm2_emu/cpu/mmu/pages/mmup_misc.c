#include "../../cpu.h"
#include "../mmu.h"
/*
mmup_misc.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void mmu_setptstart(u64 New) {
	i->pti.ps = New;
	mmuctx->Pages = ((byte*)cpuctx->PhysicalMemory + i->pti.ps);
	mmuctx->MaxPageCount = (i->pti.pe - i->pti.ps) / sizeof(mmuctx->Pages[0]);
	return;
}

void mmu_setptend(u64 New) {
	i->pti.pe = New;
	mmuctx->Pages = ((byte*)cpuctx->PhysicalMemory + i->pti.ps);
	mmuctx->MaxPageCount = (i->pti.pe - i->pti.ps) / sizeof(mmuctx->Pages[0]);
	return;
}