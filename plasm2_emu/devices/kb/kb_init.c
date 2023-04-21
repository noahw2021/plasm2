#include "kb.h"
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <string.h>
#include <stdlib.h>
/*
kb_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/
u64 KeysDown[4];
kbctx_t* kbctx;

#pragma warning(disable: 6387)

void kb_clock(void) {
	if (kbctx->NotifyUp) {
		mmu_push(kbctx->NotifyUp);
		cpui_inst_int(kbctx->KeyUp);
		kbctx->NotifyUp = 0;
	}
	if (kbctx->NotifyDown) {
		mmu_push(kbctx->NotifyDown);
		cpui_inst_int(kbctx->KeyDown);
		kbctx->NotifyDown = 0;
	}
}

void kb_init(void) {
	kbctx = malloc(sizeof(kbctx_t));
	memset(kbctx, 0, sizeof(kbctx_t));
	return;
}

void kb_shutdown(void) {
	free(kbctx);
	return;
}