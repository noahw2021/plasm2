#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "kb.h"
#include <string.h>
/*
video_native.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void kbi_setkeydownint(u64 Interrupt) {
	kbctx->KeyDown = (byte)Interrupt;
	return;
}

void kbi_setkeyupint(u64 Interrupt) {
	kbctx->KeyUp = (byte)Interrupt;
	return;
}

u64  kbi_getkeymapptr(void) {
	return mmu_translate(0x23F0, REASON_READ);
}

void kbii_markkey(byte Keycode, byte Status) {
	if (Status)
		KeysDown[Keycode / 4] |= 0x1LLU << (Keycode % 64);
	else
		KeysDown[Keycode / 4] &= ~(0x1LLU << (Keycode % 64));
	return;
}