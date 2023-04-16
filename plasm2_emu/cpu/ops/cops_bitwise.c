#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_bitwise.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void __AND(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Source : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(++i->ip);
	i->rs_gprs[Inputs.Destination] &= i->rs_gprs[Inputs.Source];
	return;
}

void __BOR(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Source : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(++i->ip);
	i->rs_gprs[Inputs.Destination] |= i->rs_gprs[Inputs.Source];
	return;
}

void __XOR(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Source : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(++i->ip);
	i->rs_gprs[Inputs.Destination] ^= i->rs_gprs[Inputs.Source];
	return;
}