//
//  mmu_io.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>

BYTE MmuRead1(WORD64 Address) {
	BYTE* PhysicalMemory = (BYTE*)(CpuCtx->PhysicalMemory); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		ECtx->FlagsS.XF = 1;
	else
		return PhysicalMemory[Address];
	return 0;
}
WORD64 MmuRead8(WORD64 Address) {
	return MmuReadX(Address, 8);
}
WORD64  MmuReadX(WORD64 Address, BYTE BytesToRead) {
	union {
		WORD64 b;
		BYTE s[8];
	}a = { 0 };
	for (int i = 0; i < BytesToRead; i++) {
		a.s[i] = MmuRead1(Address + i);
	}
	return a.b;
}

void MmuPut8(WORD64 Address, WORD64 Value) {
	BYTE* PhysicalBytes = (BYTE*)((BYTE*)CpuCtx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	WORD64* PhysicalWords = (WORD64*)PhysicalBytes;
	if (Address > PHYS_MEMSZ)
		ECtx->FlagsS.XF = 1;
	else
	PhysicalWords[0] = Value;
	return;
}

void MmuPut1(WORD64 Address, BYTE Value) {
	BYTE* PhysicalBytes = (BYTE*)((BYTE*)CpuCtx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		ECtx->FlagsS.XF = 1;
	else
		PhysicalBytes[0] = Value;
	return;
}

void MmuPut4(WORD64 Address, WORD32 Value) {
	WORD32* PhysicalBytes = (WORD32*)((BYTE*)CpuCtx->PhysicalMemory + Address); // PM usage good (reason: comes from trust)
	if (Address > PHYS_MEMSZ)
		ECtx->FlagsS.XF = 1;
	else
		PhysicalBytes[0] = Value;
	return;
}
