//
//  cpu_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CPU_CTX* CpuCtx;

// invalid opcode
void __cpui_invopc(void) {
	i->flags_s.HF = 1;
	printf("[ERR]: Invalid opcode presented. Continuing...\n");
	return;
}

#pragma warning(disable: 6011 6387)

void CpuInit(void) {
	CpuCtx = malloc(sizeof(CPU_CTX));
	memset(CpuCtx, 0, sizeof(CPU_CTX));

	time(&CpuCtx->SystemBoot);
	CpuCtx->SystemSeconds = CpuCtx->SystemBoot;
	CpuCtx->ClocksPerSecond = BASE_CLOCK;

	for (int i = 0; i < 256; i++) {
		if (Instructions[i] == NULL)
			Instructions[i] = __cpui_invopc;
	}

	mmu_init();
	return;
}

void CpuShutdown(void) {
	mmu_shutdown();

	free(CpuCtx);
	return;
}
