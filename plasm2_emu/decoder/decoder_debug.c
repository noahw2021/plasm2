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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 6011 6387) // no it couldnt

byte decoderi_g1(void) {
	byte Return = mmu_read1(dcctx->SpeculativePointer);
	dcctx->SpeculativePointer += 1;
	return Return;
}

u64  decoderi_gx(byte HowMuch) {
	u64 Returns = mmu_readx(dcctx->SpeculativePointer, HowMuch);
	dcctx->SpeculativePointer += HowMuch;
	return Returns;
}

void decoder_go(byte Instruction) {
	int Psin2Id = psin2i_getinstructionbycd(Instruction);	
	/*
	if debugger is disabled, the cpu does no opcode checking by default (yet)
	as of 7/21/23. this may be implemented in the future, but for now there will
	be no error checking within the debugger. this may seem bad, but it should 
	encourage me to get on error checking within the cpu, as this debugger may
	be slow and shouldn't be functioning all the time. thus, we will not do
	any checking and pretend everything is all good, all the time. until the system
	burns. right now, the only real thing that can go wrong is an invalid opcode,
	or a malformed argument package. both of which should cause the cpu to go haywire,
	but not crash the emulator.
	*/

	dcctx->SpeculativePointer = i->ip;

	_bool TwoArgsOneByte = TRUE;
	_bool IsOperandRegister[2] = { FALSE, FALSE };
	u64  OperandValues[2] = { 0, 0 };
	u64  ActiveOperandSize;
	byte OperandCnt = 0;

	for (int i = 0; i < psin2i_getoperandcnt(Psin2Id); i++) {
		if (psin2i_getphyssize(Psin2Id, i) != 4)
			TwoArgsOneByte = FALSE;
	}
	if (psin2i_getoperandcnt(Psin2Id) == 1)
		TwoArgsOneByte = FALSE;

	if (TwoArgsOneByte) {
		union {
			byte Raw;
			struct {
				byte r1 : 4;
				byte r0 : 4;
			};
		}InputData;
		InputData.Raw = decoderi_g1();
		IsOperandRegister[0] = TRUE;
		IsOperandRegister[1] = TRUE;

		OperandValues[0] = InputData.r0;
		OperandValues[1] = InputData.r1;

		OperandCnt = 2;
	} else {
		for (int i = 0; i < psin2i_getoperandcnt(Psin2Id); i++) {
			byte StoredSize = psin2i_getphyssize(Psin2Id, i);
			if (StoredSize == 4)
				ActiveOperandSize = 1;
			else
				ActiveOperandSize = StoredSize / 8;

			IsOperandRegister[i] = psin2i_getoperandtype(Psin2Id, i) ^ 1; // psin2 stores these opposite
			OperandValues[i] = decoderi_gx((byte)ActiveOperandSize);
			OperandCnt++;
		}
	}

	char* DebugStr = malloc(256);

	switch (OperandCnt) {
	case 0: // OPC
		sprintf(DebugStr, "%s", psin2i_getname(Psin2Id));
		break;
	case 1: // OPC ARG
		sprintf(DebugStr, "%s %s=%c%llu", psin2i_getname(Psin2Id), psin2i_getoperandname(Psin2Id, 0), 
			(IsOperandRegister[0] == 1 ? 'r' : 0xEE), OperandValues[0]);
		break;
	case 2: // OPC ARG, RG2
		sprintf(DebugStr, "%s %s=%c%llu, %s=%c%llu", psin2i_getname(Psin2Id), psin2i_getoperandname(Psin2Id, 0), 
			(IsOperandRegister[0] == 1 ? 'r' : 0xEE), OperandValues[0], psin2i_getoperandname(Psin2Id, 1), 
			(IsOperandRegister[1] == 1 ? 'r' : 0xEE), OperandValues[1]);
		break;
	}

	decoder_print(DebugStr);
	
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
			sprintf(CPart, "%sr%llu=0x%016llX", CPart, OperandValues[c], i->rs_gprs[OperandValues[c]]);

			strcat(Ctx, CPart);
			Written++;
		}
	}

	if (Written)
		strcat(Ctx, ", ");

	sprintf(CPart, "ip=0x%llX, sp=0x%llX", i->ip, i->sp);
	strcat(Ctx, CPart);

	decoder_print(Ctx);

	free(DebugStr);
	free(Ctx);
	free(CPart);
	return ;
}
