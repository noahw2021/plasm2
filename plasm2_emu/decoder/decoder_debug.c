//
//  decoder_debug.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "decoder.h"
#include "../cpu/cpu.h"
#include "../cpu/mmu/mmu.h"
#include "../psin2/psin2.h"
#include "../emu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 6011 6387) // no it couldnt

BYTE DecoderRead1(void) {
	BYTE Return = MmuRead1(DcCtx->SpeculativePointer);
	DcCtx->SpeculativePointer += 1;
	return Return;
}

WORD64  DecoderReadX(BYTE HowMuch) {
	WORD64 Returns = MmuReadX(DcCtx->SpeculativePointer, HowMuch);
	DcCtx->SpeculativePointer += HowMuch;
	return Returns;
}

void DecoderGo(BYTE Instruction) {
    if (EmuCtx->Flags & EMUFLAG_NODEBUG)
        return;
    
	int Psin2Id = Psin2iGetInstructionByOpcode(Instruction);
	/*
	if debugger is disabled, the cpu does no opcode checking by default (yet)
	as of 7/21/23. this may be implemented in the future, but for now there 
    will be no error checking within the debugger. this may seem bad, but it
    should encourage me to get on error checking within the cpu, as this
    debugger may be slow and shouldn't be functioning all the time. thus, we
    will not do any checking and pretend everything is all good, all the
    time. until the system burns. right now, the only real thing that can go
    wrong is an invalid opcode, or a malformed argument package. both of
    which should cause the cpu to go haywire, but not crash the emulator.
	*/

	DcCtx->SpeculativePointer = ECtx->ip;

	_bool TwoArgsOneByte = TRUE;
	_bool IsOperandRegister[2] = { FALSE, FALSE };
	WORD64  OperandValues[2] = { 0, 0 };
	WORD64  ActiveOperandSize;
	BYTE OperandCnt = 0;

	for (int i = 0; i < Psin2iGetOperandCount(Psin2Id); i++) {
		if (Psin2iGetPhysicalSize(Psin2Id, i) != 4)
			TwoArgsOneByte = FALSE;
	}
	if (Psin2iGetOperandCount(Psin2Id) == 1)
		TwoArgsOneByte = FALSE;

	if (TwoArgsOneByte) {
		union {
			BYTE Raw;
			struct {
				BYTE r1 : 4;
				BYTE r0 : 4;
			};
		}InputData;
		InputData.Raw = DecoderRead1();
		IsOperandRegister[0] = TRUE;
		IsOperandRegister[1] = TRUE;

		OperandValues[0] = InputData.r0;
		OperandValues[1] = InputData.r1;

		OperandCnt = 2;
	} else {
		for (int i = 0; i < Psin2iGetOperandCount(Psin2Id); i++) {
			BYTE StoredSize = Psin2iGetPhysicalSize(Psin2Id, i);
			if (StoredSize == 4)
				ActiveOperandSize = 1;
			else
				ActiveOperandSize = StoredSize / 8;

			IsOperandRegister[i] = Psin2iGetOperandType(Psin2Id, i) ^ 1; // psin2 stores these opposite
			OperandValues[i] = DecoderReadX((BYTE)ActiveOperandSize);
			OperandCnt++;
		}
	}

	char* DebugStr = malloc(256);

	switch (OperandCnt) {
	case 0: // OPC
		sprintf(DebugStr, "%s", Psin2iGetInstructionName(Psin2Id));
		break;
	case 1: // OPC ARG
		sprintf(DebugStr, "%s %s=%c%llu", Psin2iGetInstructionName(Psin2Id), 
            Psin2iGetOperandName(Psin2Id, 0),
			(IsOperandRegister[0] == 1 ? 'r' : 0xEE), OperandValues[0]);
		break;
	case 2: // OPC ARG, RG2
		sprintf(DebugStr, "%s %s=%c%llu, %s=%c%llu", 
            Psin2iGetInstructionName(Psin2Id),
            Psin2iGetOperandName(Psin2Id, 0),
			(IsOperandRegister[0] == 1 ? 'r' : 0xEE), OperandValues[0], 
            Psin2iGetOperandName(Psin2Id, 1),
			(IsOperandRegister[1] == 1 ? 'r' : 0xEE), OperandValues[1]);
		break;
	}

	DecoderPrint(DebugStr);
	
	char* Ctx = malloc(768);
	Ctx[0] = '\t';
	Ctx[1] = '\0';

	char* CPart = malloc(256);
	int Written = 0;
	for (int c = 0; c < 2; c++) {
		if (IsOperandRegister[c]) {

			CPart[0] = '\0';
			if (Written)
				strcat(CPart, ", ");
			sprintf(CPart, "%sr%llu=0x%016llX", CPart, OperandValues[c], 
                ECtx->GPRs[OperandValues[c]]);

			strcat(Ctx, CPart);
			Written++;
		}
	}

	if (Written)
		strcat(Ctx, ", ");

	sprintf(CPart, "ip=0x%llX, sp=0x%llX", ECtx->ip, ECtx->sp);
	strcat(Ctx, CPart);

	DecoderPrint(Ctx);

	free(DebugStr);
	free(Ctx);
	free(CPart);
	return ;
}
