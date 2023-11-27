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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] += ECtx->GPRs[Inputs.Source];
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] -= ECtx->GPRs[Inputs.Source];
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] *= ECtx->GPRs[Inputs.Source];
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] /= ECtx->GPRs[Inputs.Source];
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] %= ECtx->GPRs[Inputs.Source];
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Source]++;
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Source]--;
	return;
}

void ADI(void) {
	BYTE Input = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	ECtx->GPRs[Input] += Immediate;
	return;
}

void SBI(void) {
	BYTE Input = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	ECtx->GPRs[Input] -= Immediate;
	return;
}

void MLI(void) {
	BYTE Input = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	ECtx->GPRs[Input] *= Immediate;
	return;
}

void DVI(void) {
	BYTE Input = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	ECtx->GPRs[Input] /= Immediate;
	return;
}

void MDI(void) {
	BYTE Input = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	ECtx->GPRs[Input] %= Immediate;
	return;
}
