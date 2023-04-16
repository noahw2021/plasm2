#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_general.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void __MOV(void) {
	union {
		byte Byte;
		struct {
			byte Dest : 4;
			byte Source : 4;
		};
	}Input;
	Input.Byte = mmu_read1(++i->ip);
	i->rs_gprs[Input.Dest] = i->rs_gprs[Input.Source];
	return;
}

void __LDI(void) {
	byte Destination = mmu_read1(++i->ip) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	i->rs_gprs[Destination] = Immediate;
	return;
}

void __JMP(void) {
	byte Address = mmu_read1(++i->ip) & 0xF;
	u64 PhysicalAddress = mmu_translate(i->rs_gprs[Address], REASON_READ | REASON_EXEC);
	if (PhysicalAddress)
		i->ip = PhysicalAddress;
	else
		i->flags_s.XF = 1;
	return;
}

void __NXC(void) {
	i->flags_s.NF = 0;
	return;
}

void __NXE(void) {
	i->flags_s.NF = !i->flags_s.EF;
	return;
}

void __NXZ(void) {
	i->flags_s.NF = !i->flags_s.ZF;
	return;
}

void __NXG(void) {
	i->flags_s.NF = !i->flags_s.GF;
	return;
}

void __NXL(void) {
	i->flags_s.NF = i->flags_s.GF;
	return;
}

void __CLL(void) {
	byte Register = mmu_read1(++i->ip) & 0xF;
	u64 Address = i->rs_gprs[Register];
	u64 PhysicalAddress = mmu_translate(Address, REASON_EXEC | REASON_READ);
	if (!PhysicalAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->pti.ral = i->sp;
	mmu_push(i->ip);
	i->flags_s.SF = 1;
	i->ip = PhysicalAddress;
	return;
}

void __RET(void) {
	if (i->sp != i->pti.ral) {
		i->flags_s.XF = 1;
		if (i->flags_s.AF) {
			cpui_csm(CSM_IMPROPERSTACK, i->sp - i->pti.ral);
			return;
		}
	}
	i->sp = i->pti.ral;
	i->ip = mmu_pop();
	return;
}

void __IMR(void) {
	u16 ReturnValue = mmu_readx(i->ip, 2);
	i->ip += 2;
	i->r0 = ReturnValue;
	__RET(); // reuse
	return;
}

void __SHF(void) {
	i->flags_s.HF = 1;
	return;
}
