//
//  kb_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "kb.h"
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <string.h>
#include <stdlib.h>

WORD64 KeysDown[4];
PKB_CTX KbCtx;

#pragma warning(disable: 6387)

void KbClock(void) {
	if (KbCtx->NotifyUp) {
		mmu_push(KbCtx->NotifyUp);
		CpuInstructionINT(KbCtx->KeyUp);
		KbCtx->NotifyUp = 0;
	}
	if (KbCtx->NotifyDown) {
		mmu_push(KbCtx->NotifyDown);
		CpuInstructionINT(KbCtx->KeyDown);
		KbCtx->NotifyDown = 0;
	}
    
    return;
}

void KbInit(void) {
	KbCtx = malloc(sizeof(KB_CTX));
	memset(KbCtx, 0, sizeof(KB_CTX));
	return;
}

void KbShutdown(void) {
	free(KbCtx);
	return;
}
