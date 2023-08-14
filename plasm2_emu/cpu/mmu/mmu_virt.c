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

u64 mmu_translate(u64 VirtualAddress, byte Reason) {
	if (i->flags_s.VF) {
		for (int i = 0; i < mmuctx->PageCount; i++) {
			if (InRange(VirtualAddress, mmuctx->Pages[i].Virtual, mmuctx->Pages[i].Virtual + mmuctx->Pages[i].Size)) {
				if ((Reason & REASON_READ) && !mmuctx->Pages[i].Read)
					return 0;
				if ((Reason & REASON_WRTE) && !mmuctx->Pages[i].Write)
					return 0;
				if ((Reason & REASON_EXEC) && !mmuctx->Pages[i].Execute)
					return 0;
				if (VirtualAddress >= (mmuctx->Pages[i].Virtual + mmuctx->Pages[i].Size))
					return 0;

				return mmuctx->Pages[i].Physical + (VirtualAddress - mmuctx->Pages[i].Virtual);
			}
		}
		return 0;
	} else {
		VirtualAddress &= (PHYS_MEMSZ - 1);
		return VirtualAddress;
	}
}

u64 mmu_maxaddr(u64 VirtualAddress, byte Reason) {
	if (i->flags_s.VF) {
		for (int i = 0; i < mmuctx->PageCount; i++) {
			if (InRange(VirtualAddress, mmuctx->Pages[i].Virtual, mmuctx->Pages[i].Virtual + mmuctx->Pages[i].Size)) {
				if ((Reason & REASON_READ) && !mmuctx->Pages[i].Read)
					return 0;
				if ((Reason & REASON_WRTE) && !mmuctx->Pages[i].Write)
					return 0;
				if ((Reason & REASON_EXEC) && !mmuctx->Pages[i].Execute)
					return 0;
				if (VirtualAddress >= (mmuctx->Pages[i].Virtual + mmuctx->Pages[i].Size))
					return 0;

				return mmuctx->Pages[i].Virtual + mmuctx->Pages[i].Size - 1;
			}
		}
	} else {
		return PHYS_MEMSZ;
	}
	return 0x00;
}