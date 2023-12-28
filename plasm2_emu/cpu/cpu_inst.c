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
	ECtx->ip = Address;
	return;
}

void CpuInstructionCLL(WORD64 Address) {
	if (!Address) {
		ECtx->Flags.XF = 1;
		return;
	}

	ECtx->ControlRegisters.ReturnAddressLocation = ECtx->sp + 16;
    MmuPush(ECtx->ip);
    
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = ECtx->Flags.CF;
    ECtx->Flags.HF = 0;
	SecurityPacket.Flags = (WORD32)ECtx->Flags.FlagsR;
	SecurityPacket.SecurityLevel = ECtx->Security.SecurityLevel;
	MmuPush(SecurityPacket.Raw);
    ECtx->Flags.SF = 1;
	ECtx->Flags.CF = 1;
    
    WORD64 PhysAdr = MmuTranslate(Address, REASON_READ | REASON_EXEC,
        SIZE_WATCHDOG);
    
    ECtx->ControlRegisters.NextCallAddress = PhysAdr;
	return;
}

void CpuInstructionRET(void) {
	if (!ECtx->Flags.CF)
		return;
	
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
	
    ECtx->ip = MmuPop();
    ECtx->Flags.FlagsR = SecurityPacket.Flags;
    ECtx->Security.SecurityLevel = SecurityPacket.SecurityLevel;
    ECtx->Flags.CF = SecurityPacket.CallFlag;
    
    return;
}

void CpuInstructionINT(BYTE Interrupt) {
	WORD64* InterruptTable = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory +
        ECtx->ControlRegisters.InterruptTable);
    // PM usage good (reason: ControlRegisters->InterruptTable
    // is a secure register)
	WORD64 VirtualAddress = InterruptTable[Interrupt];
	BYTE SecurityLevel = (BYTE)((VirtualAddress & 0xFF00000000000000LLU) 
        >> 56LLU);
	ECtx->Security.SecurityLevel = SecurityLevel;
	WORD64 PhysicalAddress = MmuTranslate(VirtualAddress 
        & 0x00FFFFFFFFFFFFFF, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
    // PM usage good (reason: watchdog & trusted register)
	if (!PhysicalAddress) {
		ECtx->Flags.XF = 1;
		return;
	}
    
	ECtx->ControlRegisters.ReturnAddressLocation = ECtx->ip;
	MmuPush(ECtx->ip);
	union {
		WORD64 Raw;
		struct {
			WORD32 Flags;
			BYTE SecurityLevel;
			BYTE CallFlag;
			WORD16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = ECtx->Flags.CF;
	SecurityPacket.Flags = (WORD32)ECtx->Flags.FlagsR;
	SecurityPacket.SecurityLevel = ECtx->Security.SecurityLevel;
	MmuPush(SecurityPacket.Raw);
	ECtx->Flags.SF = 1;
	ECtx->ip = PhysicalAddress;
    
    // push registers
    for (int i = 0; i < 19; i++)
        MmuPush(ECtx->Registers64[i]);
    
	return;
}

void CpuInstructionCLR(void) {
    WORD64 StackPointerBackup = ECtx->sp;
    ECtx->sp = ECtx->ControlRegisters.ReturnAddressLocation;
    
    WORD64 SecurityPacket = MmuPop();
    WORD64 ReturnAddress = MmuPop();
    
    ReturnAddress = ECtx->ip;
    
    MmuPush(ReturnAddress);
    MmuPush(SecurityPacket);
    
    ECtx->sp = StackPointerBackup;
    
    if (ECtx->Flags.CF)
        ECtx->ip = ECtx->ControlRegisters.NextCallAddress;
    
    return;
}

void CpuInstructionIRT(void) {
    if (!ECtx->Flags.IF)
        return;
    
    for (int i = 0; i < 19; i++)
        ECtx->Registers64[i] = MmuPop();
    
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
    
    ECtx->ip = MmuPop();
    ECtx->Security.SecurityLevel = SecurityPacket.SecurityLevel;
    
    return;
}
