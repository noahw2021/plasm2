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

void cpui_inst_jmp(WORD64 Address) {
	i->ip = Address;
	return;
}

void cpui_inst_cll(WORD64 Address) {
	if (!Address) {
		i->flags_s.XF = 1;
		return;
	}

	i->pti.ral = i->sp;
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
	i->ip = Address;
	i->flags_s.CF = 1;
	return;
}

void cpui_inst_ret(void) {
	if (!i->flags_s.CF)
		return;
	if (i->sp != i->pti.ral) {
		i->flags_s.XF = 1;
		if (i->flags_s.AF) {
			cpui_csm_msg(CSM_IMPROPERSTACK, i->sp - i->pti.ral);
			return;
		}
	}
	i->sp = i->pti.ral;
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
	i->flags = SecurityPacket.Flags;
	i->security_s.SecurityLevel = SecurityPacket.SecurityLevel;
	i->flags_s.CF = SecurityPacket.CallFlag;
	i->ip = mmu_pop();
	return;
}

void cpui_inst_int(BYTE Interrupt) {
	WORD64* InterruptTable = (WORD64*)((BYTE*)cpuctx->PhysicalMemory + i->pti.it); // PM usage good (reason: pti.it is a secure register)
	WORD64 VirtualAddress = InterruptTable[Interrupt];
	BYTE SecurityLevel = (BYTE)((VirtualAddress & 0xFF00000000000000LLU) >> 56LLU);
	i->security_s.SecurityLevel = SecurityLevel;
	WORD64 PhysicalAddress = mmu_translate(VirtualAddress & 0x00FFFFFFFFFFFFFF, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (!PhysicalAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->pti.ral = i->sp;
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

void cpui_inst_clr(void) {
    mmu_push(i->pti.ral);
    return;
}
