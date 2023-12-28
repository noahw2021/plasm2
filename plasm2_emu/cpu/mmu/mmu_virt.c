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
    if (ECtx->Flags.VF) {
		for (int p = 0; p < MmuCtx->PageCount; p++) {
            PMMU_PAGE ThisPage = &MmuCtx->Pages[p];
			
            if (InRange(VirtualAddress, ThisPage->Virtual,
                ThisPage->Virtual + ThisPage->Size)
            ) {
                if (!(EmuCtx->Flags & EMUFLAG_NOSECURE)) {
                    if ((Reason & REASON_READ) && !ThisPage->Flags.Read)
                        return 0;
                    if ((Reason & (REASON_WRTE | REASON_EXEC))) {
                        if (!ThisPage->Flags.Secure)
                            return 0;
                        
                        if (ThisPage->Flags.Selector &&
                            (Reason & REASON_WRTE)
                        ) {
                            return 0;
                        }
                        
                        if (!ThisPage->Flags.Selector && 
                            (Reason & REASON_EXEC)
                        ) {
                            return 0;
                        }
                    }
                    
                    if (MaxSize != SIZE_WATCHDOG) {
                        if (VirtualAddress + MaxSize >=
                            (ThisPage->Virtual + ThisPage->Size)
                        ) {
                            return 0;
                        }
                    } else {
                        if (VirtualAddress + MaxSize >= 
                            (ThisPage->Virtual + ThisPage->Size)
                        ) {
                            return 0;
                        }
                    }
				}
                
                ECtx->Flags.MF = 1;
                ECtx->ControlRegisters.PageMaxLocation = 
                    VirtualAddress + MaxSize;

				return ThisPage->Physical + 
                    (VirtualAddress - ThisPage->Virtual);
			}
		}
		return 0;
	} else {
		VirtualAddress &= (PHYS_MEMSZ - 1);
		return VirtualAddress;
	}
}
