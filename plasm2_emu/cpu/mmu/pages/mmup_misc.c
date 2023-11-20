#include "../../cpu.h"
#include "../mmu.h"
/*
mmup_misc.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void mmu_setptstart(WORD64 New) {
	i->pti.ps = New;
	mmuctx->Pages = (void*)((BYTE*)cpuctx->PhysicalMemory + i->pti.ps); // PM usage good: (reason: trusted instruction)
	mmuctx->MaxPageCount = (i->pti.pe - i->pti.ps) / sizeof(mmuctx->Pages[0]);
	return;
}

void mmu_setptend(WORD64 New) {
	i->pti.pe = New;
	mmuctx->Pages = (void*)((BYTE*)cpuctx->PhysicalMemory + i->pti.ps); // PM usage good: (reason: trusted instruction)
	mmuctx->MaxPageCount = (i->pti.pe - i->pti.ps) / sizeof(mmuctx->Pages[0]);
	return;
}
