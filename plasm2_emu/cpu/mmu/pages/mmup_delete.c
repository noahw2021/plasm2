#include "../../cpu.h"
#include "../mmu.h"
/*
mmup_delete.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void mmu_deletepage(u64 VirtualAddress) {
	for (int c = 0; c < mmuctx->PageCount; c++) {
		if (InRange(VirtualAddress, mmuctx->Pages[c].Virtual, mmuctx->Pages[c].Virtual + mmuctx->Pages[c].Size)) {
			mmuctx->Pages[c].Active = 0;
			break;
		}
	}
	return;
}