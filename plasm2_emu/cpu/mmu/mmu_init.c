#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
mmu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

mmuctx_t* mmuctx;

#pragma warning(disable: 6387)

void mmu_init(void) {
	mmuctx = malloc(sizeof(mmuctx_t));
	memset(mmuctx, 0, sizeof(mmuctx_t));

	cpuctx->PhysicalMemorySize = PHYS_MEMSZ;
	cpuctx->PhysicalMemory = malloc(cpuctx->PhysicalMemorySize);
	
	i->sp = 0x3A0;

	return;
}

void mmu_shutdown(void) {
	free(mmuctx);
}
