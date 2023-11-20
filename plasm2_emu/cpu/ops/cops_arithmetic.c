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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] += i->rs_gprs[Inputs.Source];
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] -= i->rs_gprs[Inputs.Source];
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] *= i->rs_gprs[Inputs.Source];
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] /= i->rs_gprs[Inputs.Source];
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] %= i->rs_gprs[Inputs.Source];
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Source]++;
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
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Source]--;
	return;
}

void ADI(void) {
	BYTE Input = mmu_read1(i->ip++) & 0xF;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] += Immediate;
	return;
}

void SBI(void) {
	BYTE Input = mmu_read1(i->ip++) & 0xF;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] -= Immediate;
	return;
}

void MLI(void) {
	BYTE Input = mmu_read1(i->ip++) & 0xF;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] *= Immediate;
	return;
}

void DVI(void) {
	BYTE Input = mmu_read1(i->ip++) & 0xF;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] /= Immediate;
	return;
}

void MDI(void) {
	BYTE Input = mmu_read1(i->ip++) & 0xF;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] %= Immediate;
	return;
}
