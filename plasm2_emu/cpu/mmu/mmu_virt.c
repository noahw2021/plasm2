//
//  mmu_virt.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "mmu.h"
#include "../../emu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

WORD64 MmuTranslate(WORD64 VirtualAddress, BYTE Reason, WORD64 MaxSize) {
	if (ECtx->flags_s.VF) {
		for (int p = 0; p < MmuCtx->PageCount; p++) {
			if (InRange(VirtualAddress, MmuCtx->Pages[p].Virtual, MmuCtx->Pages[p].Virtual + MmuCtx->Pages[p].Size)) {
                if (!(EmuCtx->Flags & EMUFLAG_NOSECURE)) {
                    if ((Reason & REASON_READ) && !MmuCtx->Pages[p].Read)
                        return 0;
                    if ((Reason & REASON_WRTE) && !MmuCtx->Pages[p].Write)
                        return 0;
                    if ((Reason & REASON_EXEC) && !MmuCtx->Pages[p].Execute)
                        return 0;
                    if (MaxSize != SIZE_WATCHDOG) {
                        if (VirtualAddress + MaxSize >= (MmuCtx->Pages[p].Virtual + MmuCtx->Pages[p].Size))
                            return 0;
                    } else {
                        if (VirtualAddress + MaxSize >= (MmuCtx->Pages[p].Virtual + MmuCtx->Pages[p].Size))
                            return 0;
                    }
				}
                ECtx->flags_s.MF = 1;
                ECtx->ControlRegisters.PageMaxLocation = VirtualAddress + MaxSize;

				return MmuCtx->Pages[p].Physical + (VirtualAddress - MmuCtx->Pages[p].Virtual);
			}
		}
		return 0;
	} else {
		VirtualAddress &= (PHYS_MEMSZ - 1);
		return VirtualAddress;
	}
}
