#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
/*
mmu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

BYTE mmu_read1(WORD64 Address) {
	BYTE* PhysicalMemory = (BYTE*)(cpuctx->PhysicalMemory); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
		return PhysicalMemory[Address];
	return 0;
}
WORD64 mmu_read8(WORD64 Address) {
	return mmu_readx(Address, 8);
}
WORD64  mmu_readx(WORD64 Address, BYTE BytesToRead) {
	union {
		WORD64 b;
		BYTE s[8];
	}a = { 0 };
	for (int i = 0; i < BytesToRead; i++) {
		a.s[i] = mmu_read1(Address + i);
	}
	return a.b;
}

void mmu_put8(WORD64 Address, WORD64 Value) {
	BYTE* PhysicalBytes = (BYTE*)((BYTE*)cpuctx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	WORD64* PhysicalWords = (WORD64*)PhysicalBytes;
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
	PhysicalWords[0] = Value;
	return;
}

void mmu_put1(WORD64 Address, BYTE Value) {
	BYTE* PhysicalBytes = (BYTE*)((BYTE*)cpuctx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
		PhysicalBytes[0] = Value;
	return;
}

void mmu_put4(WORD64 Address, WORD32 Value) {
	WORD32* PhysicalBytes = (WORD32*)((BYTE*)cpuctx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
		PhysicalBytes[0] = Value;
	return;
}
