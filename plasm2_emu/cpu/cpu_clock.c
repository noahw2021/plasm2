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
    CpuCtx->LastTrackedNanoSecond = CpuTimerGetPreciseTimeNanoseconds();
    while (CpuCtx->NextTickNanoSecond > CpuCtx->LastTrackedNanoSecond) {
#if defined(__unix__) || defined(__MACH__)
#ifdef HAVE_NANOSLEEP
        struct timespec NextClockTime, ThisTime;
        NextClockTime.tv_sec = 0;
        NextClockTime.tv_nsec = CpuCtx->NextTickNanoSecond - CpuCtx->LastTrackedNanoSecond;
        nanosleep(&NextClockTime, &ThisTime);
#endif
#elif _WIN32
/* @TODO : NtDelayExecution for nanosleep =D
 This is a large CPU hole, that there doesn't seem to be a great solution to.
 When I'm on my Windows desktop, I'll attempt to divide the NextClockTime nanoseconds
 into 100ns chunks that a function such as NtDelayExecution can deal with.
 - nw 11/27/23
 */
#endif
        CpuCtx->LastTrackedNanoSecond = CpuTimerGetPreciseTimeNanoseconds();
    }
    
    CpuCtx->NextTickNanoSecond = CpuCtx->LastTrackedNanoSecond + (NS_PER_S / CpuCtx->ClocksPerSecond);
    
    CpuCtx->SystemTicks++;
    
    if (ECtx->flags_s.NF) {
        ECtx->flags_s.NF = 0;
        int Psin2Id = Psin2iGetInstructionByOpcode(MmuRead1(ECtx->ip));
        BYTE TotalToRead = (Psin2iGetTotalSize(Psin2Id) / 8);
        ECtx->ip += TotalToRead;
        return;
    }
   

	BYTE ThisInstruction = MmuRead1(ECtx->ip++);
	if (EmuCtx->DebuggerEnabled)
		DecoderGo(ThisInstruction);
    
	Instructions[ThisInstruction]();

	return;
}
