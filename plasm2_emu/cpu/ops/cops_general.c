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
	Input.Byte = mmu_read1(++i->ip);
	i->rs_gprs[Input.Dest] = i->rs_gprs[Input.Source];
	return;
}

void LDI(void) {
	byte Destination = mmu_read1(++i->ip) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	i->rs_gprs[Destination] = Immediate;
	return;
}

void JMP(void) {
	byte Address = mmu_read1(++i->ip) & 0xF;
	u64 PhysicalAddress = mmu_translate(i->rs_gprs[Address], REASON_READ | REASON_EXEC);
	if (PhysicalAddress)
		i->ip = PhysicalAddress;
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
	byte Register = mmu_read1(++i->ip) & 0xF;
	u64 Address = i->rs_gprs[Register];
	u64 PhysicalAddress = mmu_translate(Address, REASON_EXEC | REASON_READ);
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
	SecurityPacket.Flags = i->flags;
	SecurityPacket.SecurityLevel = i->security_s.SecurityLevel;
	mmu_push(SecurityPacket.Raw);
	i->flags_s.SF = 1;
	i->ip = PhysicalAddress;
	i->flags_s.CF = 1;
	return;
}

void RET(void) {
	if (!i->flags_s.CF)
		return;
	if (i->sp != i->pti.ral) {
		i->flags_s.XF = 1;
		if (i->flags_s.AF) {
			cpui_csm(CSM_IMPROPERSTACK, i->sp - i->pti.ral);
			return;
		}
	}
	i->sp = i->pti.ral;
	union {
		u64 Raw;
		struct {
			u32 Flags;
			byte SecurityLevel;
			byte CallFlag;
			u16 Reserved;
		};
	}SecurityPacket;
	SecurityPacket.Raw = mmu_pop();
	i->flags = SecurityPacket.Flags;
	i->security_s.SecurityLevel = SecurityPacket.SecurityLevel;
	i->flags_s.CF = SecurityPacket.CallFlag;
	i->ip = mmu_pop();
	return;
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
