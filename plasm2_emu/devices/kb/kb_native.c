//
//  kb_native.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "kb.h"
#include <string.h>

void KbiSetKeyDownInterrupt(WORD64 Interrupt) {
	KbCtx->KeyDown = (BYTE)Interrupt;
	return;
}

void KbiSetKeyUpInterrupt(WORD64 Interrupt) {
	KbCtx->KeyUp = (BYTE)Interrupt;
	return;
}

WORD64  KbiGetKeyMapPointer(void) {
	return MmuTranslate(0x23F0, REASON_READ, 256);
}

void KbiiMarkKeyState(BYTE Keycode, BYTE Status) {
	if (Status)
		KeysDown[Keycode / 4] |= 0x1LLU << (Keycode % 64);
	else
		KeysDown[Keycode / 4] &= ~(0x1LLU << (Keycode % 64));
	return;
}
