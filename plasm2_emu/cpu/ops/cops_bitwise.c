#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_bitwise.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void AND(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] &= i->rs_gprs[Inputs.Source];
	return;
}

void BOR(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] |= i->rs_gprs[Inputs.Source];
	return;
}

void XOR(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] ^= i->rs_gprs[Inputs.Source];
	return;
}

void BSL(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] <<= i->rs_gprs[Inputs.Source];
}

void BSR(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] >>= i->rs_gprs[Inputs.Source];
}

void NOT(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] ^= 0xFFFFFFFFFFFFFFFF;
}

void ANI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] &= Immediate;
	return;
}

void XOI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] ^= Immediate;
	return;
}

void ORI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] |= Immediate;
	return;
}

void BLI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	byte Immediate = mmu_read1(i->ip++);

	i->rs_gprs[Input] <<= Immediate;
	return;
}

void BRI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	byte Immediate = mmu_read1(i->ip++);


	i->rs_gprs[Input] >>= Immediate;
	return;
}