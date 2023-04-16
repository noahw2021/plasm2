#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
cpu_clock.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void cpu_clock(void) {
	time_t CurrentTime = time(NULL);
	if (CurrentTime >= (cpuctx->LastClockTime + (1000 / cpuctx->ClocksPerSecond))) { // 1000 is the clock resolution :)
		cpuctx->LastClockTime = CurrentTime;
	} else {
		return;
	}

	byte ThisInstruction = mmu_read1(++i->ip);
	Instructions[ThisInstruction]();
}