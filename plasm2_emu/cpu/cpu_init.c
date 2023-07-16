#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
/*
cpu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

cpuctx_t* cpuctx;
extern void* __pls_free;

void cpu_init(void) {
	cpuctx = malloc(sizeof(cpuctx_t));
	memset(cpuctx, 0, sizeof(cpuctx_t));

	cpuctx->ClocksPerSecond = BASE_CLOCK;

	mmu_init();
	return;
}

void cpu_shutdown(void) {
	mmu_shutdown();

	free(cpuctx);
	free(__pls_free);
	return;
}