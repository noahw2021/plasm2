#include "cpu.h"
#include "mmu/mmu.h"
#include "../decoder/decoder.h"
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
	if (
		CurrentTime >= (cpuctx->LastClockTime + (1000 / cpuctx->ClocksPerSecond)) // Avoid Jumping time
		&& !i->flags_s.HF // Do not clock if we are halted
		&& !i->flags_s.NF // Skip this cycle, due to no-execute
		) { // 1000 is the clock resolution :)
		cpuctx->LastClockTime = CurrentTime;
	} else {
		i->flags_s.NF = 0;
		return;
	}

	byte ThisInstruction = mmu_read1(i->ip++);

	Instructions[ThisInstruction]();

	return;
}