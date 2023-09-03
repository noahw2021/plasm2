#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
/*
cpu_inst.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void cpui_inst_jmp(u64 Address) {
	i->ip = Address;
	return;
}
void cpui_inst_cll(u64 Address) {
	if (!Address) {
		i->flags_s.XF = 1;
		return;
	}

	i->pti.ral = i->sp;
	mmu_push(i->ip);
	union {
		u64 Raw;
		struct {
			u32 Flags;
			byte SecurityLevel;
			byte CallFlag;
			u16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = i->flags_s.CF;
	SecurityPacket.Flags = (u32)i->flags;
	SecurityPacket.SecurityLevel = i->security_s.SecurityLevel;
	mmu_push(SecurityPacket.Raw);
	i->flags_s.SF = 1;
	i->flags_s.CF = 1;
	//i->ip = Address;
	u64 PhysAdr = mmu_translate(Address, REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (!PhysAdr) {
		i->flags_s.XF = 1;
		i->flags_s.CF = 0;
		return;
	}
	i->pti.nca = PhysAdr;
	return;
}
void cpui_inst_ret(void) {
	if (!i->flags_s.CF)
		return;
	i->sp = i->pti.ral + 8;
	union {
		u64 Raw;
		struct {
			u32 Flags;
			byte SecurityLevel;
			byte CallFlag;
			u16 Reserved;
		};
	}SecurityPacket = { 0 };
	SecurityPacket.Raw = mmu_pop();
	i->flags = SecurityPacket.Flags;
	i->security_s.SecurityLevel = SecurityPacket.SecurityLevel;
	i->flags_s.CF = SecurityPacket.CallFlag;
	i->ip = mmu_pop();
	return;
}

void cpui_inst_int(byte Interrupt) {
	u64* InterruptTable = (u64*)((byte*)cpuctx->PhysicalMemory + i->pti.it); // PM usage good (reason: pti.it is a secure register)
	u64 VirtualAddress = InterruptTable[Interrupt];
	byte SecurityLevel = (byte)((VirtualAddress & 0xFF00000000000000LLU) >> 56LLU);
	byte BackupLevel = i->security_s.SecurityLevel;
	i->security_s.SecurityLevel = SecurityLevel;
	u64 PhysicalAddress = mmu_translate(VirtualAddress & 0x00FFFFFFFFFFFFFF, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (!PhysicalAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->pti.ral = i->sp;
	mmu_push(i->ip);
	union {
		u64 Raw;
		struct {
			u32 Flags;
			byte SecurityLevel;
			byte CallFlag;
			u16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.CallFlag = i->flags_s.CF;
	SecurityPacket.Flags = (u32)i->flags;
	SecurityPacket.SecurityLevel = i->security_s.SecurityLevel;
	mmu_push(SecurityPacket.Raw);
	i->flags_s.SF = 1;
	i->ip = PhysicalAddress;
	return;
}

void cpui_inst_clr(void) {
	u64 SecP = mmu_pop();
	u64 RetPtr = mmu_pop();
	RetPtr = i->ip;
	mmu_push(RetPtr);
	mmu_push(SecP);

	if (i->flags_s.CF)
		i->ip = i->pti.nca;
}