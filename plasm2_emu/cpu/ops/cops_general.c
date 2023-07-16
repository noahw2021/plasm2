#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_general.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void MOV(void) {
	union {
		byte Byte;
		struct {
			byte Dest : 4;
			byte Source : 4;
		};
	}Input;
	Input.Byte = mmu_read1(i->ip++);
	i->rs_gprs[Input.Dest] = i->rs_gprs[Input.Source];
	return;
}

void LDI(void) {
	byte Destination = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	i->rs_gprs[Destination] = Immediate;
	return;
}

void JMP(void) {
	byte Address = mmu_read1(i->ip++) & 0xF;
	u64 PhysicalAddress = mmu_translate(i->rs_gprs[Address], REASON_READ | REASON_EXEC);
	if (PhysicalAddress)
		cpui_inst_jmp(PhysicalAddress);
	else
		i->flags_s.XF = 1;
	return;
}

void NXC(void) {
	i->flags_s.NF = 0;
	return;
}

void NXE(void) {
	i->flags_s.NF = !i->flags_s.EF;
	return;
}

void NXZ(void) {
	i->flags_s.NF = !i->flags_s.ZF;
	return;
}

void NXG(void) {
	i->flags_s.NF = !i->flags_s.GF;
	return;
}

void NXL(void) {
	i->flags_s.NF = i->flags_s.GF;
	return;
}

void CLL(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	u64 Address = i->rs_gprs[Register];
	u64 PhysicalAddress = mmu_translate(Address, REASON_EXEC | REASON_READ);
	cpui_inst_cll(PhysicalAddress);
}

void RET(void) {
	cpui_inst_ret();
}

void IMR(void) {
	u16 ReturnValue = mmu_readx(i->ip, 2);
	i->ip += 2;
	i->r0 = ReturnValue;
	RET(); // reuse
	return;
}

void SHF(void) {
	i->flags_s.HF = 1;
	return;
}

void CMP(void) { // __CMP = 0x0C, // CMP 0C (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare
	union {
		byte Byte;
		struct {
			byte r1 : 4;
			byte r2 : 4;
		};
	}Input;
	Input.Byte = mmu_read1();

	if (i->rs_gprs[Input.r1] > i->rs_gprs[Input.r2])
		i->flags_s.GF = 1;
	if (i->rs_gprs[Input.r1] == i->rs_gprs[Input.r2])
		i->flags_s.EF = 1;
	
	return;
}