//
//  xcc_timer.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"

WORD64 CpuTimerGetTime(void) {
	return (CpuCtx->SystemSeconds - CpuCtx->SystemBoot) & 0x0FFFFFFFFFFFFFFF;
}
