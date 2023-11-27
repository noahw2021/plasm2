//
//  cpu_clock.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"
#include "mmu/mmu.h"
#include "../decoder/decoder.h"
#include "../psin2/psin2.h"
#include "../emu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void CpuClock(void) {

	if ( 
		 !ECtx->flags_s.HF // Do not clock if we are halted
		&& !ECtx->flags_s.NF // Skip this cycle, due to no-execute
		) {
		time(&CpuCtx->SystemSeconds);
	} else {
		if (ECtx->flags_s.NF) {
			ECtx->flags_s.NF = 0;
			int Psin2Id = Psin2iGetInstructionByOpcode(MmuRead1(ECtx->ip));
			BYTE TotalRead = (Psin2iGetTotalSize(Psin2Id) / 8);
			ECtx->ip += TotalRead;
		}
		return;
	}

	if (ECtx->flags_s.VF &&
		ECtx->flags_s.MF &&
		ECtx->ip >= ECtx->ControlRegisters.PageMaxLocation
		) {
		CpuCsmSendMessage(CSM_XPAGETOOSMALL, ECtx->ip);
	}

	BYTE ThisInstruction = MmuRead1(ECtx->ip++);
	if (EmuCtx->DebuggerEnabled)
		DecoderGo(ThisInstruction);

    if (ThisInstruction == 0x00) {
        if (MmuRead1(ECtx->ip) == 0x00 &&
            MmuRead1(ECtx->ip) == 0x00
        ) {
            // we might be in a zero loop
            FILE* Memout = fopen("memout.bin", "wb");
            fwrite(CpuCtx->PhysicalMemory, CpuCtx->PhysicalMemorySize,
                   1, Memout);
            fclose(Memout);
            printf("[WARN]: CPU appears to be stuck.\n Continue?");
            SDL_Delay(500);
        }
    }
    
	Instructions[ThisInstruction]();

	return;
}
