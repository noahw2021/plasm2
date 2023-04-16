#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
/*
cpu_clock.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void cpu_clock(void) {
	byte ThisInstruction = mmu_read1(++i->ip);

}