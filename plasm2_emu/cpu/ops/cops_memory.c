#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_memory.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void LDW(void) {
	union {
		byte Input;
		struct {
			byte Address : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read8(VirtualAddress);
	return;
}

void LDB(void) {
	union {
		byte Input;
		struct {
			byte Address : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read1(VirtualAddress);
	return;
}

void STW(void) {
	union {
		byte Input;
		struct {
			byte Register : 4;
			byte Address : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put8(VirtualAddress, i->rs_gprs[Inputs.Register]);
	return;
}

void STB(void) {
	union {
		byte Input;
		struct {
			byte Register : 4;
			byte Address : 4;
		};
	}Inputs; 
	Inputs.Input = mmu_read1(i->ip++);
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put1(VirtualAddress, (byte)i->rs_gprs[Inputs.Register]);
	return;
}

void PSH(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	mmu_push(i->rs_gprs[Register]);
	return;
}

void POP(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	i->rs_gprs[Register] = mmu_pop();
	return;
}

void PSR(void) {
	for (int c = 0; c < 16; c++)
		mmu_push(i->rs_gprs[c]);
	return;
}

void POR(void) {
	for (int c = 0; c < 16; c++)
		i->rs_gprs[c] = mmu_pop();
	return;
}

void VME(void) {
	i->flags_s.VF = 1;
	return;
}

void VMD(void) {
	i->flags_s.VF = 0;
	return;
}

void VPC(void) {
	union {
		byte Input;
		struct {
			byte Size : 4;
			byte PhysicalAddress : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	u64 VirtualAddr = mmu_createpage(i->rs_gprs[Inputs.PhysicalAddress], i->rs_gprs[Inputs.Size], i->r0 & (REASON_EXEC | REASON_READ | REASON_WRTE));
	mmu_push(VirtualAddr);
	return;
}

void VPD(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	mmu_deletepage(i->rs_gprs[Register]);
	return;
}

void VSI(void) {
	i->security_s.SecurityLevel++;
	return;
}

void VSD(void) {
	if (!i->flags_s.AF)
		i->security_s.SecurityLevel--;
	return;
}

void SPS(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	i->sp = i->rs_gprs[Register];
	return;
}

void SPG(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	i->rs_gprs[Register] = i->sp;
	return;
}

void VSS(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	mmu_setptstart(i->rs_gprs[Register]);
	return;
}

void VES(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	mmu_setptend(i->rs_gprs[Register]);
	return;
}
