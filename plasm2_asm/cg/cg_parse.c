//
//  cg_parse.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include "../vf/vf.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#pragma warning(disable: 6011 6308 6387 28182)

extern FILE* PrimaryOutput;

void CgParse(const char* Line) {
	CgCtx->CurrentLine++;

	char* CommentCorrect = (char*)Line;
	// incredibly basic and temporary comment system
	if (strstr(CommentCorrect, ";"))
		*(char*)strstr(CommentCorrect, ";") = 0x00;

	if (CommentCorrect[0] == 0x00 || CommentCorrect[0] == '\n')
		return;

	// MOV r0, r1
	int c = 0, t = 0;
	BYTE OperandAPresent = 0, OperandBPresent = 0;
	BYTE** OperandPtrs = malloc(sizeof(BYTE*) * 2);
	OperandPtrs[0] = &OperandAPresent;
	OperandPtrs[1] = &OperandBPresent;

	char* OperandA, * OperandB;
	char* Temporary = malloc(256);
	memset(Temporary, 0, 256);
	OperandA = malloc(64);
	OperandB = malloc(64);
	WORD64 OperandAPhys = 0, OperandBPhys = 0;
	WORD64** OperandPhysPtrs;
	OperandPhysPtrs = malloc(sizeof(WORD64*) * 2);
	OperandPhysPtrs[0] = &OperandAPhys;
	OperandPhysPtrs[1] = &OperandBPhys;
	char** OperandNamePtrs[2];
	OperandNamePtrs[0] = &OperandA;
	OperandNamePtrs[1] = &OperandB;
	BYTE OperandPtrSizes[2] = { 0, 0 };
	BYTE OperandSingleByte = 0x00;


	if (strstr(Line, "\n"))
		*(char*)(strstr(Line, "\n")) = 0x00;

	// Grab operand name
    int c2 = c;
	if (Line[0] != '-') {
		while (Line[c] && Line[c] != ' ')
			Temporary[c2++] = Line[c++] & ~0x20;
	} else {
		while (Line[c] && Line[c] != ' ')
			Temporary[c2++] = Line[c++];
	}
	c++;
	Temporary[c] = 0x00;

	int Usage = 0;
	char* FollowString = (char*)Line + 3;

    
	if (Temporary[0] == '-') { // pragmas
		switch (Temporary[1]) {
		case 'a': // address set (-a 0000)
			CgCtx->InSub = 0;
			CgCtx->DataPosition = strtoull(Line + 3, NULL, CgCtx->CurrentRadix);
			fseek(PrimaryOutput, (WORD32)CgCtx->DataPosition, SEEK_SET);
			break;
		case 'b': // base set
			CgCtx->CurrentRadix = atoi(Line  + 3);
			break;
		case 'c': // compile
			CgCtx->DataPosition = CgCtx->HighestPosition;
			CgCompile();
			break;
		case 'f': // fill
			VfgWrite(Line + 3, CgCtx->DataPosition);
			break;
		case 'i': // include
			VfRegister(FollowString);
			break;
		case 'p': // pad with zero until x size
			Usage = (int)strtoull(Line + 3, NULL, CgCtx->CurrentRadix);
			CgCtx->DataPosition = CgCtx->HighestPosition;
			while (CgCtx->DataPosition < Usage)
				CgpPut1('\0');
			break;
		case 'r': // set the current reference (location in memory)
			CgCtx->ReferencePtr = strtoull(Line + 3, NULL, CgCtx->CurrentRadix);
			break;
		case 's': // string
			Usage = 1;
			while (FollowString[Usage] != '"')
				CgpPut1(FollowString[Usage++]);
			CgpPut1('\0');
			break;
		case 'z': // fill with zero
			Usage = (int)strtoull(Line + 3, NULL, CgCtx->CurrentRadix);
			for (int i = 0; i < Usage; i++)
				CgpPut1('\0');
			break;
		}
		goto ExitThis;
	}

	char* _Line = (char*)Line;
	unsigned long __x = strlen(Line);
	int Count = 0;
	while (_Line[__x - Count] != ':') {
		Count++;
		if (Count == __x)
			break;
	}
	if (Count != __x) {
		if (strstr(_Line, ":"))
			*(char*)strstr(_Line, ":") = 0x00;
		LinkResolve(Line, CgCtx->DataPosition);
		CgCtx->InSub = 1;
		goto ExitThis;
	}

	int Psin = Psin2iGetInstructionByName(Temporary);

	if (Psin == -1) {
		CgeError(CgCtx->CurrentLine, "[E1002]: Invalid operand");
		goto ExitThis;
	}

	// Grab operand one
	t = 0;
	while (Line[c] && Line[c] != ',') {
		Temporary[t++] = Line[c++];
		OperandAPresent = 1;
	}
	if (OperandAPresent) {
		if (Line[c + 1]) {
			c++;
			if (Line[c + 1])
				c++;
		}
		Temporary[t] = 0x00;
		strcpy(OperandA, Temporary);

		if (OperandA[0] != 'r') {
			if (!(InRange(OperandA[0], '0', '9') || ((CgCtx->CurrentRadix == 16) && InRange(OperandA[0], 'A', 'F')))) {
				WORD64 Resolved = LinkGetSymbol(OperandA, 1);
				char* ToString = malloc(64);
				if (CgCtx->CurrentRadix == 16)
					sprintf(ToString, "%llX", Resolved);
				else
					sprintf(ToString, "%llu", Resolved);
				strcpy(OperandA, ToString);
				free(ToString);
			}
		}
	}
OutA:

	// Grab operand two
	t = 0;
	while (Line[c] && Line[c] != ',') {
		Temporary[t++] = Line[c++];
		OperandBPresent = 1;

	}
	if (OperandBPresent) {
		Temporary[t] = 0x00;
		strcpy(OperandB, Temporary);

		if (OperandB[0] != 'r') {
			if (!(InRange(OperandB[0], '0', '9') || ((CgCtx->CurrentRadix == 16) && InRange(OperandB[0], 'A', 'F')))) {
				WORD64 Resolved = LinkGetSymbol(OperandB, 1);
				char* ToString = malloc(64);
				if (CgCtx->CurrentRadix == 16)
					sprintf(ToString, "%llX", Resolved);
				else
					sprintf(ToString, "%llu", Resolved);
				strcpy(OperandB, ToString);
				free(ToString);
			}
		}
	}
OutB:

	for (int o = 0; o < Psin2iGetOperandCount(Psin); o++) {
		if (!OperandPtrs[o]) {
			CgeError(CgCtx->CurrentLine, "[E1000]: Missing Operand %c", (o == 0) ? 'A' : 'B');
			goto ExitThis;
		}

		if (Psin2iGetOperandType(Psin, o) == 0) {
			if (OperandNamePtrs[o][0][0] != 'r') {
				CgeError(CgCtx->CurrentLine, "[E1001]: Invalid syntax in Operand %c", (o == 0) ? 'A' : 'B');
				goto ExitThis;
			}
			OperandPhysPtrs[o][0] = atoi(OperandNamePtrs[o][0] + 1);
			if (Psin2iGetPhysicalSize(Psin, o) == 4) {
				OperandSingleByte |= *OperandPhysPtrs[o] << (4 * (1 - o));
				OperandPtrSizes[o] = 4;
			} else {
				OperandPtrSizes[o] = Psin2iGetPhysicalSize(Psin, o);
			}
		} else {
			OperandPtrSizes[o] = Psin2iGetPhysicalSize(Psin, o);
			*OperandPhysPtrs[o] = strtoull(*OperandNamePtrs[o], NULL, CgCtx->CurrentRadix);
		}
	}

    BYTE TwoOpsOneByte = 0, DualOutput = '\0';
	if (OperandPtrSizes[0] == 4 && OperandPtrSizes[1] == 4) {
		TwoOpsOneByte = 1;
		union {
			BYTE Single;
			struct {
				BYTE DualA : 4;
				BYTE DualB : 4;
			};
		}SingleDual;

		SingleDual.DualA = (BYTE)*OperandPhysPtrs[0];
		SingleDual.DualB = (BYTE)*OperandPhysPtrs[1];

		DualOutput = SingleDual.Single;	
	} else {
		for (int i = 0; i < 2; i++) {
			if (OperandPtrSizes[i] == 4)
				OperandPtrSizes[i] = 8;
		}
	}

 	CgpPut1(Psin2iGetInstructinOpcode(Psin));
	if (OperandSingleByte && OperandPtrSizes[0] == 4) {
		CgpPut1(OperandSingleByte);
	} else {
		if (TwoOpsOneByte) {
			CgpPut1(DualOutput);
		} else {
			if (OperandAPresent)
				CgpPutX(*OperandPhysPtrs[0], OperandPtrSizes[0] / 8);
			if (OperandBPresent)
				CgpPutX(*OperandPhysPtrs[1], OperandPtrSizes[1] / 8);
		}
	}

	goto ExitThis;
ExitThis:
	free(Temporary);
	free(OperandA);
	free(OperandB);
	free(OperandPhysPtrs);
	free(OperandPtrs);
	return;
}
