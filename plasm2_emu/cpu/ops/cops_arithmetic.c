#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_arithmetic.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void ADD(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] += i->rs_gprs[Inputs.Source];
	return;
}

void SUB(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] -= i->rs_gprs[Inputs.Source];
	return;
}

void MUL(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] *= i->rs_gprs[Inputs.Source];
	return;
}

void DIV(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] /= i->rs_gprs[Inputs.Source];
	return;
}

void MOD(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] %= i->rs_gprs[Inputs.Source];
	return;
}

void INC(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Source]++;
	return;
}

void DEC(void) {
	union {
		byte Input;
		struct {
			byte Source : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Source]--;
	return;
}

void ADI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] += Immediate;
	return;
}

void SBI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] -= Immediate;
	return;
}

void MLI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] *= Immediate;
	return;
}

void DVI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] /= Immediate;
	return;
}

void MDI(void) {
	byte Input = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	i->rs_gprs[Input] %= Immediate;
	return;
}