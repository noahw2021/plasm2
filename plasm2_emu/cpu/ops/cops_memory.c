//
//  cops_memory.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"

void LDW(void) {
	union {
		BYTE Input;
		struct {
			BYTE Address : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read8(VirtualAddress);
	return;
}

void LDB(void) {
	union {
		BYTE Input;
		struct {
			BYTE Address : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ, 1);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read1(VirtualAddress);
	return;
}

void STW(void) {
	union {
		BYTE Input;
		struct {
			BYTE Register : 4;
			BYTE Address : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put8(VirtualAddress, i->rs_gprs[Inputs.Register]);
	return;
}

void STB(void) {
	union {
		BYTE Input;
		struct {
			BYTE Register : 4;
			BYTE Address : 4;
		};
	}Inputs; 
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE, 1);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put1(VirtualAddress, (BYTE)i->rs_gprs[Inputs.Register]);
	return;
}

void PSH(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
	mmu_push(i->rs_gprs[Register]);
	return;
}

void POP(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
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
		BYTE Input;
		struct {
			BYTE Size : 4;
			BYTE PhysicalAddress : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	WORD64 VirtualAddr = mmu_createpage(i->rs_gprs[Inputs.PhysicalAddress], i->rs_gprs[Inputs.Size], i->r0 & (REASON_EXEC | REASON_READ | REASON_WRTE));
	mmu_push(VirtualAddr);
	return;
}

void VPD(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
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
	BYTE Register = mmu_read1(i->ip++) & 0xF;
	i->sp = i->rs_gprs[Register];
	return;
}

void SPG(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
	i->rs_gprs[Register] = i->sp;
	return;
}

void VSS(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	mmu_setptstart(i->rs_gprs[Register]);
	return;
}

void VES(void) {
	BYTE Register = mmu_read1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}
	mmu_setptend(i->rs_gprs[Register]);
	return;
}

void LWS(void) {
	BYTE Register = mmu_read1(i->ip++) & 0x0F;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	WORD64 Virtual = mmu_translate(Immediate, REASON_READ, 8);
	i->rs_gprs[Register] = mmu_read8(Virtual);
	return;
}

void SWS(void) {
	BYTE Register = mmu_read1(i->ip++) & 0x0F;
	WORD64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	WORD64 Virtual = mmu_translate(Immediate, REASON_READ, 8);
	mmu_put8(Virtual, i->rs_gprs[Register]);

	return;
}

void LDH(void) {
	union {
		BYTE Input;
		struct {
			BYTE Address : 4;
			BYTE Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ, 4);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_readx(VirtualAddress, 4);
	return;
}

void STH(void) {
	union {
		BYTE Input;
		struct {
			BYTE Register : 4;
			BYTE Address : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	WORD64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}

	mmu_put4(VirtualAddress, (WORD32)i->rs_gprs[Inputs.Register]);
	return;
}

void PSI(void) {
    WORD64 Immediate = mmu_read8(i->ip);
    i->ip += 8;
    mmu_push(Immediate);
    return;
}
