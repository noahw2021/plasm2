//
//  cops_bitwise.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void AND(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] &= i->GPRs[Inputs.Source];
	return;
}

void BOR(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] |= i->GPRs[Inputs.Source];
	return;
}

void XOR(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] ^= i->GPRs[Inputs.Source];
	return;
}

void BSL(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] <<= i->GPRs[Inputs.Source];
}

void BSR(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] >>= i->GPRs[Inputs.Source];
}

void NOT(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] ^= 0xFFFFFFFFFFFFFFFF;
}

void ANI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] &= Immediate;
	return;
}

void XOI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] ^= Immediate;
	return;
}

void ORI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] |= Immediate;
	return;
}

void BLI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	BYTE Immediate = MmuRead1(i->ip++);

	i->GPRs[Input] <<= Immediate;
	return;
}

void BRI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	BYTE Immediate = MmuRead1(i->ip++);


	i->GPRs[Input] >>= Immediate;
	return;
}
