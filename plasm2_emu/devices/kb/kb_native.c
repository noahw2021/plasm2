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
	kbctx->KeyDown = Interrupt;
	return;
}

void kbi_setkeyupint(u64 Interrupt) {
	kbctx->KeyUp = Interrupt;
	return;
}

u64  kbi_getkeymapptr(void) {
	if (i->flags_s.VF) {
		for (int c = 0; c < mmuctx->PageCount; c++) {
			if (mmuctx->Pages[c].Physical == 0x23F0)
				return mmuctx->Pages[c].Virtual;
		}
	} else {
		return 0x23F0;
	}
}

void kbii_markkey(byte Keycode, byte Status) {
	if (Status)
		KeysDown[Keycode / 4] |= 0x1LLU << (Keycode % 64);
	else
		KeysDown[Keycode / 4] &= ~(0x1LLU << (Keycode % 64));
	return;
}