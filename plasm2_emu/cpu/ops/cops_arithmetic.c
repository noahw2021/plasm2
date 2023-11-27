//
//  cops_arithmetic.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void ADD(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] += i->GPRs[Inputs.Source];
	return;
}

void SUB(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] -= i->GPRs[Inputs.Source];
	return;
}

void MUL(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] *= i->GPRs[Inputs.Source];
	return;
}

void DIV(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] /= i->GPRs[Inputs.Source];
	return;
}

void MOD(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Destination] %= i->GPRs[Inputs.Source];
	return;
}

void INC(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Source]++;
	return;
}

void DEC(void) {
	union {
		BYTE Input;
		struct {
			BYTE Source : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(i->ip++);
	i->GPRs[Inputs.Source]--;
	return;
}

void ADI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] += Immediate;
	return;
}

void SBI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] -= Immediate;
	return;
}

void MLI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] *= Immediate;
	return;
}

void DVI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] /= Immediate;
	return;
}

void MDI(void) {
	BYTE Input = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	i->GPRs[Input] %= Immediate;
	return;
}
