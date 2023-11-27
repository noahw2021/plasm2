//
//  cops_interrupts.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void INT(void) {
	BYTE Interrupt = i->GPRs[MmuRead1(i->ip++) & 0xF] & 0xFF;
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
	Input.Byte = MmuRead1(i->ip++);
	BYTE SecurityLevel = (BYTE)MmuPop();
	if (!i->flags_s.TF) {
		i->flags_s.XF = 1;
		return;
	}
	if (SecurityLevel < i->Security.SecurityLevel) {
		i->flags_s.XF = 1;
		return;
	}
	WORD64 VirtualAddress = i->GPRs[Input.Handler];
	VirtualAddress &= 0x00FFFFFFFFFFFFFF;
	VirtualAddress |= ((WORD64)SecurityLevel) << 56;
	MmuPut8(i->ControlRegisters.InterruptTable + ((WORD64)Input.Interrupt * 8), VirtualAddress);
	return;
}

void IRT(void) {
	if (i->sp != i->ControlRegisters.ReturnAddressLocation) {
		i->flags_s.XF = 1;
		if (i->flags_s.AF) {
			CpuCsmSendMessage(CSM_IMPROPERSTACK, i->sp - i->ControlRegisters.ReturnAddressLocation);
			return;
		}
	}
	i->sp = i->ControlRegisters.ReturnAddressLocation;
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.Raw = MmuPop();
	i->Flags = SecurityPacket.Flags;
	i->Security.SecurityLevel = SecurityPacket.SecurityLevel;
	i->flags_s.CF = SecurityPacket.CallFlag;
	i->ip = MmuPop();
	return;
}

void ENI(void) {
	i->flags_s.IF = 1;
	return;
}

void DSI(void) {
	i->flags_s.IF = 0;
	return;
}

void SMH(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	if (i->Security.SecurityLevel == 0)
		CpuCsmSetHandler(i->GPRs[Register]);
	else
		i->flags_s.XF = 1;
	return;
}

void SIT(void) {
	WORD64 PhysicalAddress = i->GPRs[MmuRead1(i->ip++) & 0xF];
	if (i->Security.SecurityLevel < 2)
		i->ControlRegisters.InterruptTable = PhysicalAddress;
	return;
}
