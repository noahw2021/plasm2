//
//  cpu_inst.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>

void CpuInstructionJMP(WORD64 Address) {
	i->ip = Address;
	return;
}

void CpuInstructionCLL(WORD64 Address) {
	if (!Address) {
		i->flags_s.XF = 1;
		return;
	}

	i->ControlRegisters.ReturnAddressLocation = i->sp + 16;
    MmuPush(i->ip);
    
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = i->flags_s.CF;
    i->flags_s.HF = 0;
	SecurityPacket.Flags = (WORD32)i->Flags;
	SecurityPacket.SecurityLevel = i->Security.SecurityLevel;
	MmuPush(SecurityPacket.Raw);
    i->flags_s.SF = 1;
	i->flags_s.CF = 1;
    
    WORD64 PhysAdr = MmuTranslate(Address, REASON_READ | REASON_EXEC,
        SIZE_WATCHDOG);
    
    i->ControlRegisters.NextCallAddress = PhysAdr;
	return;
}

void CpuInstructionRET(void) {
	if (!i->flags_s.CF)
		return;
	
	//i->sp = i->pti.ral;
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket = { 0 };
	SecurityPacket.Raw = MmuPop();
	
    i->ip = MmuPop();
    i->Flags = SecurityPacket.Flags;
    i->Security.SecurityLevel = SecurityPacket.SecurityLevel;
    i->flags_s.CF = SecurityPacket.CallFlag;
    
    return;
}

void CpuInstructionINT(BYTE Interrupt) {
	WORD64* InterruptTable = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory + i->ControlRegisters.InterruptTable); // PM usage good (reason: pti.it is a secure register)
	WORD64 VirtualAddress = InterruptTable[Interrupt];
	BYTE SecurityLevel = (BYTE)((VirtualAddress & 0xFF00000000000000LLU) >> 56LLU);
	i->Security.SecurityLevel = SecurityLevel;
	WORD64 PhysicalAddress = MmuTranslate(VirtualAddress & 0x00FFFFFFFFFFFFFF, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (!PhysicalAddress) {
		i->flags_s.XF = 1;
		return;
	}
    
	i->ControlRegisters.ReturnAddressLocation = i->ip;
	MmuPush(i->ip);
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = i->flags_s.CF;
	SecurityPacket.Flags = (WORD32)i->Flags;
	SecurityPacket.SecurityLevel = i->Security.SecurityLevel;
	MmuPush(SecurityPacket.Raw);
	i->flags_s.SF = 1;
	i->ip = PhysicalAddress;
	return;
}

void CpuInstructionCLR(void) {
    WORD64 StackPointerBackup = i->sp;
    i->sp = i->ControlRegisters.ReturnAddressLocation;
    WORD64 SecurityPacket = MmuPop();
    WORD64 ReturnAddress = MmuPop();
    ReturnAddress = i->ip;
    MmuPush(ReturnAddress);
    MmuPush(SecurityPacket);
    i->sp = StackPointerBackup;
    if (i->flags_s.CF)
        i->ip = i->ControlRegisters.NextCallAddress;
}
