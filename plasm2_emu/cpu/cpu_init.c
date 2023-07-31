#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
cpu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

cpuctx_t* cpuctx;

// invalid opcode
void __cpui_invopc(void) {
	i->flags_s.HF = 1;
	printf("[ERR]: Invalid opcode presented. Continuing...\n");
	return;
}


void cpu_init(void) {
	cpuctx = malloc(sizeof(cpuctx_t));
	memset(cpuctx, 0, sizeof(cpuctx_t));

	time(&cpuctx->SystemBoot);
	cpuctx->ClocksPerSecond = BASE_CLOCK;

	for (int i = 0; i < 256; i++) {
		if (Instructions[i] == NULL)
			Instructions[i] = __cpui_invopc;
	}

	mmu_init();
	return;
}

void cpu_shutdown(void) {
	mmu_shutdown();

	free(cpuctx);
	return;
}