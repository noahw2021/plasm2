//
//  cops_general.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void MOV(void) {
	union {
		BYTE Byte;
		struct {
			BYTE Source : 4;
			BYTE Dest : 4;
		};
	}Input;
	Input.Byte = MmuRead1(i->ip++);
	i->GPRs[Input.Dest] = i->GPRs[Input.Source];
	return;
}

void LDI(void) {
	BYTE Destination = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;
	i->GPRs[Destination] = Immediate;
	return;
}

void JMP(void) {
	BYTE Address = MmuRead1(i->ip++) & 0xF;
	WORD64 PhysicalAddress = MmuTranslate(i->GPRs[Address], REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (PhysicalAddress)
		CpuInstructionJMP(PhysicalAddress);
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
	i->flags_s.NF = !i->flags_s.LF;
	return;
}

void CLL(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	WORD64 Address = i->GPRs[Register];
	WORD64 PhysicalAddress = MmuTranslate(Address, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (PhysicalAddress)
		CpuInstructionCLL(PhysicalAddress);
	else
		i->flags_s.XF = 1;
	return;
}

void RET(void) {
	CpuInstructionRET();
}

void IMR(void) {
	WORD16 ReturnValue = (WORD16)MmuReadX(i->ip, 2);
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
	i->flags_s.EF = 0;
	i->flags_s.GF = 0;
	i->flags_s.LF = 0;

	union {
		BYTE Byte;
		struct {
			BYTE r2 : 4;
			BYTE r1 : 4;
		};
	}Input;
	Input.Byte = MmuRead1(i->ip++);

	if (i->GPRs[Input.r1] > i->GPRs[Input.r2])
		i->flags_s.GF = 1;
	if (i->GPRs[Input.r1] == i->GPRs[Input.r2])
		i->flags_s.EF = 1;
	if (i->GPRs[Input.r1] < i->GPRs[Input.r2])
		i->flags_s.LF = 1;
	
	return;
}

void JMI(void) {
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;
	WORD64 Translated = MmuTranslate(Immediate, REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (Translated)
		CpuInstructionJMP(Translated);
	else
		i->flags_s.XF = 1;
	return;
}

void CLI(void) {
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;
	WORD64 Translated = MmuTranslate(Immediate, REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (Translated)
		CpuInstructionCLL(Translated);
	else
		i->flags_s.XF = 1;
	return;
}

void CMI(void) {
	i->flags_s.EF = 0;
	i->flags_s.GF = 0;
	i->flags_s.LF = 0;

	BYTE Register = MmuRead1(i->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	if (i->GPRs[Register] > Immediate)
		i->flags_s.GF = 1;
	if (i->GPRs[Register] < Immediate)
		i->flags_s.LF = 1;
	if (i->GPRs[Register] == Immediate)
		i->flags_s.EF = 1;

	return;
}

void CLR(void) {
    return CpuInstructionCLR();
}
