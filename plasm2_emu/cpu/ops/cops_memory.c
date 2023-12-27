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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_READ, 8);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	ECtx->GPRs[Inputs.Destination] = MmuRead8(VirtualAddress);
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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_READ, 1);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	ECtx->GPRs[Inputs.Destination] = MmuRead1(VirtualAddress);
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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_WRTE, 8);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	MmuPut8(VirtualAddress, ECtx->GPRs[Inputs.Register]);
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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_WRTE, 1);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	MmuPut1(VirtualAddress, (BYTE)ECtx->GPRs[Inputs.Register]);
	return;
}

void PSH(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	MmuPush(ECtx->GPRs[Register]);
	return;
}

void POP(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->GPRs[Register] = MmuPop();
	return;
}

void PSR(void) {
	for (int c = 0; c < 16; c++)
		MmuPush(ECtx->GPRs[c]);
    if (ECtx->Security.SecurityLevel <= 1) {
        for (int c = 0; c < 16; c++)
            MmuPush(ECtx->SystemRs[c]);
    }
	return;
}

void POR(void) {
    if (ECtx->Security.SecurityLevel <= 1) {
        for (int c = 0; c < 16; c++)
            ECtx->SystemRs[c] = MmuPop();
    }
	for (int c = 0; c < 16; c++)
		ECtx->GPRs[c] = MmuPop();
	return;
}

void VME(void) {
	ECtx->FlagsS.VF = 1;
	return;
}

void VMD(void) {
	ECtx->FlagsS.VF = 0;
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
	if (!ECtx->FlagsS.VF)
		return;
	if (ECtx->FlagsS.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	WORD64 VirtualAddr = MmuCreatePage(ECtx->GPRs[Inputs.PhysicalAddress], 
        ECtx->GPRs[Inputs.Size], ECtx->r0 & (REASON_EXEC | REASON_READ |
        REASON_WRTE));
	MmuPush(VirtualAddr);
	return;
}

void VPD(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (!ECtx->FlagsS.VF)
		return;
	if (ECtx->FlagsS.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuDeletePage(ECtx->GPRs[Register]);
	return;
}

void VSI(void) {
	ECtx->Security.SecurityLevel++;
	return;
}

void VSD(void) {
	if (!ECtx->FlagsS.AF)
		ECtx->Security.SecurityLevel--;
	return;
}

void SPS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->sp = ECtx->GPRs[Register];
	return;
}

void SPG(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->GPRs[Register] = ECtx->sp;
	return;
}

void VSS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (ECtx->FlagsS.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableStart(ECtx->GPRs[Register]);
	return;
}

void VES(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	if (ECtx->FlagsS.AF) {
		if (ECtx->Security.SecurityLevel > 1)
			return;
	}
	MmuSetPageTableEnd(ECtx->GPRs[Register]);
	return;
}

void LWS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	ECtx->GPRs[Register] = MmuRead8(Virtual);
	return;
}

void SWS(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0x0F;
	WORD64 Immediate = MmuRead8(ECtx->ip);
	ECtx->ip += 8;

	WORD64 Virtual = MmuTranslate(Immediate, REASON_READ, 8);
	MmuPut8(Virtual, ECtx->GPRs[Register]);

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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_READ, 4);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}
	ECtx->GPRs[Inputs.Destination] = MmuReadX(VirtualAddress, 4);
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
	WORD64 VirtualAddress = MmuTranslate(ECtx->GPRs[Inputs.Address], 
        REASON_WRTE, 8);
	if (!VirtualAddress) {
		ECtx->FlagsS.XF = 1;
		return;
	}

	MmuPut4(VirtualAddress, (WORD32)ECtx->GPRs[Inputs.Register]);
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
