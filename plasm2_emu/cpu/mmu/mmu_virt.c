#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
mmu_virt.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

WORD64 mmu_translate(WORD64 VirtualAddress, BYTE Reason, WORD64 MaxSize) {
	if (i->flags_s.VF) {
		for (int p = 0; p < mmuctx->PageCount; p++) {
			if (InRange(VirtualAddress, mmuctx->Pages[p].Virtual, mmuctx->Pages[p].Virtual + mmuctx->Pages[p].Size)) {
				if ((Reason & REASON_READ) && !mmuctx->Pages[p].Read)
					return 0;
				if ((Reason & REASON_WRTE) && !mmuctx->Pages[p].Write)
					return 0;
				if ((Reason & REASON_EXEC) && !mmuctx->Pages[p].Execute)
					return 0;
                
				if (MaxSize != SIZE_WATCHDOG) {
					if (VirtualAddress + MaxSize >= (mmuctx->Pages[p].Virtual + mmuctx->Pages[p].Size))
						return 0;
				} else {
					if (VirtualAddress + MaxSize >= (mmuctx->Pages[p].Virtual + mmuctx->Pages[p].Size))
						return 0;
					
					i->flags_s.MF = 1;
					i->pti.pml = VirtualAddress + MaxSize;
				}

				return mmuctx->Pages[p].Physical + (VirtualAddress - mmuctx->Pages[p].Virtual);
			}
		}
		return 0;
	} else {
		VirtualAddress &= (PHYS_MEMSZ - 1);
		return VirtualAddress;
	}
}
