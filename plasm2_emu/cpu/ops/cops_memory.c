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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_READ, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = MmuRead8(VirtualAddress);
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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_READ, 1);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = MmuRead1(VirtualAddress);
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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	MmuPut8(VirtualAddress, i->rs_gprs[Inputs.Register]);
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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_WRTE, 1);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	MmuPut1(VirtualAddress, (BYTE)i->rs_gprs[Inputs.Register]);
	return;
}

void PSH(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	MmuPush(i->rs_gprs[Register]);
	return;
}

void POP(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	i->rs_gprs[Register] = MmuPop();
	return;
}

void PSR(void) {
	for (int c = 0; c < 16; c++)
		MmuPush(i->rs_gprs[c]);
	return;
}

void POR(void) {
	for (int c = 0; c < 16; c++)
		i->rs_gprs[c] = MmuPop();
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
	Inputs.Input = MmuRead1(i->ip++);
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->Security.SecurityLevel > 1)
			return;
	}
	WORD64 VirtualAddr = MmuCreatePage(i->rs_gprs[Inputs.PhysicalAddress], i->rs_gprs[Inputs.Size], i->r0 & (REASON_EXEC | REASON_READ | REASON_WRTE));
	MmuPush(VirtualAddr);
	return;
}

void VPD(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->Security.SecurityLevel > 1)
			return;
	}
	MmuDeletePage(i->rs_gprs[Register]);
	return;
}

void VSI(void) {
	i->Security.SecurityLevel++;
	return;
}

void VSD(void) {
	if (!i->flags_s.AF)
		i->Security.SecurityLevel--;
	return;
}

void SPS(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	i->sp = i->rs_gprs[Register];
	return;
}

void SPG(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	i->rs_gprs[Register] = i->sp;
	return;
}

void VSS(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableStart(i->rs_gprs[Register]);
	return;
}

void VES(void) {
	BYTE Register = MmuRead1(i->ip++) & 0xF;
	if (i->flags_s.AF) {
		if (i->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableEnd(i->rs_gprs[Register]);
	return;
}

void LWS(void) {
	BYTE Register = MmuRead1(i->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	i->rs_gprs[Register] = MmuRead8(Virtual);
	return;
}

void SWS(void) {
	BYTE Register = MmuRead1(i->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(i->ip);
	i->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	MmuPut8(Virtual, i->rs_gprs[Register]);

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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_READ, 4);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = MmuReadX(VirtualAddress, 4);
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
	Inputs.Input = MmuRead1(i->ip++);
	WORD64 VirtualAddress = MmuTranslate(i->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}

	MmuPut4(VirtualAddress, (WORD32)i->rs_gprs[Inputs.Register]);
	return;
}

void PSI(void) {
    WORD64 Immediate = MmuRead8(i->ip);
    i->ip += 8;
    MmuPush(Immediate);
    return;
}

void PPW(void) {
    MmuPop();
    return;
}
