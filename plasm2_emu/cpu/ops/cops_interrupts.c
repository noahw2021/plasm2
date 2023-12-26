//
//  cops_interrupts.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void INT(void) {
	BYTE Interrupt = ECtx->GPRs[MmuRead1(ECtx->ip++) & 0xF] & 0xFF;
	CpuInstructionINT(Interrupt);
	return;
}

void HND(void) {
	union {
		BYTE Byte;
		struct {
			BYTE Handler : 4;
			BYTE Interrupt : 4;
		};
	}Input;
	Input.Byte = MmuRead1(ECtx->ip++);
	BYTE SecurityLevel = (BYTE)MmuPop();
	if (!ECtx->FlagsS.TF) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	if (SecurityLevel < ECtx->Security.SecurityLevel) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	WORD64 VirtualAddress = ECtx->GPRs[Input.Handler];
	VirtualAddress &= 0x00FFFFFFFFFFFFFF;
	VirtualAddress |= ((WORD64)SecurityLevel) << 56;
	MmuPut8(ECtx->ControlRegisters.InterruptTable + ((WORD64)Input.Interrupt * 8), VirtualAddress);
	return;
}

void IRT(void) {
    CpuInstructionIRT();
    return;
}

void ENI(void) {
	ECtx->FlagsS.IF = 1;
	return;
}

void DSI(void) {
	ECtx->FlagsS.IF = 0;
	return;
}

void SMH(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (ECtx->Security.SecurityLevel == 0)
		CpuCsmSetHandler(ECtx->GPRs[Register]);
	else
		ECtx->FlagsS.XF = 1;
	return;
}

void SIT(void) {
	WORD64 PhysicalAddress = ECtx->GPRs[MmuRead1(ECtx->ip++) & 0xF];
	if (ECtx->Security.SecurityLevel < 2)
		ECtx->ControlRegisters.InterruptTable = PhysicalAddress;
	return;
}
