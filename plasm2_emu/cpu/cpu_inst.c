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

	i->pti.ral = i->sp + 16;
    mmu_push(i->ip);
    
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
	SecurityPacket.Flags = (WORD32)i->flags;
	SecurityPacket.SecurityLevel = i->security_s.SecurityLevel;
	mmu_push(SecurityPacket.Raw);
    i->flags_s.SF = 1;
	i->flags_s.CF = 1;
    
    WORD64 PhysAdr = mmu_translate(Address, REASON_READ | REASON_EXEC,
        SIZE_WATCHDOG);
    
    i->pti.nca = PhysAdr;
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
	SecurityPacket.Raw = mmu_pop();
	
    i->ip = mmu_pop();
    i->flags = SecurityPacket.Flags;
    i->security_s.SecurityLevel = SecurityPacket.SecurityLevel;
    i->flags_s.CF = SecurityPacket.CallFlag;
    
    return;
}

void CpuInstructionINT(BYTE Interrupt) {
	WORD64* InterruptTable = (WORD64*)((BYTE*)cpuctx->PhysicalMemory + i->pti.it); // PM usage good (reason: pti.it is a secure register)
	WORD64 VirtualAddress = InterruptTable[Interrupt];
	BYTE SecurityLevel = (BYTE)((VirtualAddress & 0xFF00000000000000LLU) >> 56LLU);
	i->security_s.SecurityLevel = SecurityLevel;
	WORD64 PhysicalAddress = mmu_translate(VirtualAddress & 0x00FFFFFFFFFFFFFF, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (!PhysicalAddress) {
		i->flags_s.XF = 1;
		return;
	}
    
	i->pti.ral = i->ip;
	mmu_push(i->ip);
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
	SecurityPacket.Flags = (WORD32)i->flags;
	SecurityPacket.SecurityLevel = i->security_s.SecurityLevel;
	mmu_push(SecurityPacket.Raw);
	i->flags_s.SF = 1;
	i->ip = PhysicalAddress;
	return;
}

void CpuInstructionCLR(void) {
    WORD64 StackPointerBackup = i->sp;
    i->sp = i->pti.ral;
    WORD64 SecurityPacket = mmu_pop();
    WORD64 ReturnAddress = mmu_pop();
    ReturnAddress = i->ip;
    mmu_push(ReturnAddress);
    mmu_push(SecurityPacket);
    i->sp = StackPointerBackup;
    if (i->flags_s.CF)
        i->ip = i->pti.nca;
}
