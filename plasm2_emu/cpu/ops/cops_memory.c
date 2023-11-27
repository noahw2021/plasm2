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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_READ, 8);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}
	ECtx->rs_gprs[Inputs.Destination] = MmuRead8(VirtualAddress);
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_READ, 1);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}
	ECtx->rs_gprs[Inputs.Destination] = MmuRead1(VirtualAddress);
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}
	MmuPut8(VirtualAddress, ECtx->rs_gprs[Inputs.Register]);
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_WRTE, 1);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}
	MmuPut1(VirtualAddress, (BYTE)ECtx->rs_gprs[Inputs.Register]);
	return;
}

void PSH(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	MmuPush(ECtx->rs_gprs[Register]);
	return;
}

void POP(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->rs_gprs[Register] = MmuPop();
	return;
}

void PSR(void) {
	for (int c = 0; c < 16; c++)
		MmuPush(ECtx->rs_gprs[c]);
	return;
}

void POR(void) {
	for (int c = 0; c < 16; c++)
		ECtx->rs_gprs[c] = MmuPop();
	return;
}

void VME(void) {
	ECtx->flags_s.VF = 1;
	return;
}

void VMD(void) {
	ECtx->flags_s.VF = 0;
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	if (!ECtx->flags_s.VF)
		return;
	if (ECtx->flags_s.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	WORD64 VirtualAddr = MmuCreatePage(ECtx->rs_gprs[Inputs.PhysicalAddress], ECtx->rs_gprs[Inputs.Size], ECtx->r0 & (REASON_EXEC | REASON_READ | REASON_WRTE));
	MmuPush(VirtualAddr);
	return;
}

void VPD(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (!ECtx->flags_s.VF)
		return;
	if (ECtx->flags_s.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuDeletePage(ECtx->rs_gprs[Register]);
	return;
}

void VSI(void) {
	ECtx->Security.SecurityLevel++;
	return;
}

void VSD(void) {
	if (!ECtx->flags_s.AF)
		ECtx->Security.SecurityLevel--;
	return;
}

void SPS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->sp = ECtx->rs_gprs[Register];
	return;
}

void SPG(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->rs_gprs[Register] = ECtx->sp;
	return;
}

void VSS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (ECtx->flags_s.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableStart(ECtx->rs_gprs[Register]);
	return;
}

void VES(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (ECtx->flags_s.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableEnd(ECtx->rs_gprs[Register]);
	return;
}

void LWS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	ECtx->rs_gprs[Register] = MmuRead8(Virtual);
	return;
}

void SWS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	MmuPut8(Virtual, ECtx->rs_gprs[Register]);

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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_READ, 4);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}
	ECtx->rs_gprs[Inputs.Destination] = MmuReadX(VirtualAddress, 4);
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
	Inputs.Input = MmuRead1(ECtx->ip++);
	WORD64 VirtualAddress = MmuTranslate(ECtx->rs_gprs[Inputs.Address], REASON_WRTE, 8);
	if (!VirtualAddress) {
		ECtx->flags_s.XF = 1;
		return;
	}

	MmuPut4(VirtualAddress, (WORD32)ECtx->rs_gprs[Inputs.Register]);
	return;
}

void PSI(void) {
    WORD64 Immediate = MmuRead8(ECtx->ip);
    ECtx->ip += 8;
    MmuPush(Immediate);
    return;
}

void PPW(void) {
    MmuPop();
    return;
}
