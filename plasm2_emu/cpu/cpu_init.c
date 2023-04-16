#include "cpu.h"
#include <stdlib.h>
#include <string.h>
/*
cpu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

cpuctx_t* cpuctx;

void cpu_init(void) {
	cpuctx = malloc(sizeof(cpuctx_t));
	memset(cpuctx, 0, sizeof(cputctx_t));

	cpuctx->ClocksPerSecond = BASE_CLOCK;
	return;
}

void cpu_shutdown(void) {
	free(cpuctx);
	return;
}