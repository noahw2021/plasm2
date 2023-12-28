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
	Input.Byte = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Input.Dest] = ECtx->GPRs[Input.Source];
	return;
}

void LDI(void) {
	BYTE Destination = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;
	ECtx->GPRs[Destination] = Immediate;
	return;
}

void JMP(void) {
	BYTE Address = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 PhysicalAddress = MmuTranslate(ECtx->GPRs[Address], 
        REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (PhysicalAddress)
		CpuInstructionJMP(PhysicalAddress);
	else
		ECtx->Flags.XF = 1;
	return;
}

void NXC(void) {
	ECtx->Flags.NF = 0;
	return;
}

void NXE(void) {
	ECtx->Flags.NF = !ECtx->Flags.EF;
	return;
}

void NXZ(void) {
	ECtx->Flags.NF = !ECtx->Flags.ZF;
	return;
}

void NXG(void) {
	ECtx->Flags.NF = !ECtx->Flags.GF;
	return;
}

void NXL(void) {
	ECtx->Flags.NF = !ECtx->Flags.LF;
	return;
}

void CLL(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Address = ECtx->GPRs[Register];
	WORD64 PhysicalAddress = MmuTranslate(Address, 
        REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (PhysicalAddress)
		CpuInstructionCLL(PhysicalAddress);
	else
		ECtx->Flags.XF = 1;
	return;
}

void RET(void) {
	CpuInstructionRET();
}

void IMR(void) {
	WORD16 ReturnValue = (WORD16)MmuReadX(ECtx->ip, 2);
	ECtx->ip += 2;
	ECtx->r0 = ReturnValue;
	RET(); // reuse
	return;
}

void SHF(void) {
	ECtx->Flags.HF = 1;
	return;
}

void CMP(void) { // __CMP = 0x0C, // CMP 0C (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare
	ECtx->Flags.EF = 0;
	ECtx->Flags.GF = 0;
	ECtx->Flags.LF = 0;

	union {
		BYTE Byte;
		struct {
			BYTE r2 : 4;
			BYTE r1 : 4;
		};
	}Input;
	Input.Byte = MmuRead1(ECtx->ip++);

	if (ECtx->GPRs[Input.r1] > ECtx->GPRs[Input.r2])
		ECtx->Flags.GF = 1;
	if (ECtx->GPRs[Input.r1] == ECtx->GPRs[Input.r2])
		ECtx->Flags.EF = 1;
	if (ECtx->GPRs[Input.r1] < ECtx->GPRs[Input.r2])
		ECtx->Flags.LF = 1;
	
	return;
}

void JMI(void) {
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;
	WORD64 Translated = MmuTranslate(Immediate, 
        REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (Translated)
		CpuInstructionJMP(Translated);
	else
		ECtx->Flags.XF = 1;
	return;
}

void CLI(void) {
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;
	WORD64 Translated = MmuTranslate(Immediate, 
        REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (Translated)
		CpuInstructionCLL(Translated);
	else
		ECtx->Flags.XF = 1;
	return;
}

void CMI(void) {
	ECtx->Flags.EF = 0;
	ECtx->Flags.GF = 0;
	ECtx->Flags.LF = 0;

	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	if (ECtx->GPRs[Register] > Immediate)
		ECtx->Flags.GF = 1;
	if (ECtx->GPRs[Register] < Immediate)
		ECtx->Flags.LF = 1;
	if (ECtx->GPRs[Register] == Immediate)
		ECtx->Flags.EF = 1;

	return;
}

void CLR(void) {
    return CpuInstructionCLR();
}
