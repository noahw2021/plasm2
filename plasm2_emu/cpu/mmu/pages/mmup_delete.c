//
//  mmup_delete.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "../../cpu.h"
#include "../mmu.h"

void MmuDeletePage(WORD64 VirtualAddress) {
	for (int c = 0; c < MmuCtx->PageCount; c++) {
        PMMU_PAGE ThisPage = &MmuCtx->Pages[c];
        
		if (InRange(VirtualAddress, MmuCtx->Pages[c].Virtual, 
            MmuCtx->Pages[c].Virtual + MmuCtx->Pages[c].Size)
        ) {
			ThisPage->Flags.Active = 0;
			break;
		}
	}
	return;
}
