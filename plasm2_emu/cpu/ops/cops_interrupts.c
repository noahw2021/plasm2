//
//  cops_interrupts.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void INT(void) {
	byte Interrupt = i->rs_gprs[mmu_read1(i->ip++) & 0xF] & 0xFF;
	cpui_inst_int(Interrupt);
	return;
}

void HND(void) {
	union {
		byte Byte;
		struct {
			byte Handler : 4;
			byte Interrupt : 4;
		};
	}Input;
	Input.Byte = mmu_read1(i->ip++);
	byte SecurityLevel = (byte)mmu_pop();
	if (!i->flags_s.TF) {
		i->flags_s.XF = 1;
		return;
	}
	if (SecurityLevel < i->security_s.SecurityLevel) {
		i->flags_s.XF = 1;
		return;
	}
	u64 VirtualAddress = i->rs_gprs[Input.Handler];
	VirtualAddress &= 0x00FFFFFFFFFFFFFF;
	VirtualAddress |= ((u64)SecurityLevel) << 56;
	mmu_put8(i->pti.it + ((u64)Input.Interrupt * 8), VirtualAddress);
	return;
}

void IRT(void) {
	if (i->sp != i->pti.ral) {
		i->flags_s.XF = 1;
		if (i->flags_s.AF) {
			cpui_csm_msg(CSM_IMPROPERSTACK, i->sp - i->pti.ral);
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

void ENI(void) {
	i->flags_s.IF = 1;
	return;
}

void DSI(void) {
	i->flags_s.IF = 0;
	return;
}

void SMH(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	if (i->security_s.SecurityLevel == 0)
		cpui_csm_set(i->rs_gprs[Register]);
	else
		i->flags_s.XF = 1;
	return;
}

void SIT(void) {
	u64 PhysicalAddress = i->rs_gprs[mmu_read1(i->ip++) & 0xF];
	if (i->security_s.SecurityLevel < 2)
		i->pti.it = PhysicalAddress;
	return;
}
