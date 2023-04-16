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

byte mmu_read1(u64 Address) {
	byte* PhysicalMemory = (byte*)(cpuctx->PhysicalMemory);
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
		return PhysicalMemory[Address];
	return 0;
}
u64 mmu_read8(u64 Address) {
	return mmu_readx(Address, 8);
}
u64  mmu_readx(u64 Address, byte BytesToRead) {
	union {
		u64 b;
		byte s[8];
	}a = { 0 };
	for (int i = 0; i < BytesToRead; i++) {
		a.s[i] = mmu_read1(Address + i);
	}
	return a.b;
}

void mmu_put8(u64 Address, u64 Value) {
	byte* PhysicalBytes = (byte*)((byte*)cpuctx->PhysicalMemory + Address);
	u64* PhysicalWords = (u64*)PhysicalBytes;
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
	PhysicalWords[0] = Value;
	return;
}
void mmu_put1(u64 Address, byte Value) {
	byte* PhysicalBytes = (byte*)(cpuctx->PhysicalMemory + Address);
	if (Address > PHYS_MEMSZ)
		i->flags_s.XF = 1;
	else
		PhysicalBytes[0] = Value;
	return;
}