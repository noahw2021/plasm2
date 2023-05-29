#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
cg_parse.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6387)

extern FILE* PrimaryOutput;

void cg_parse(const char* Line) {
	cgctx->CurrentLine++;

	// MOV r0, r1
	int c = 0, t = 0;
	byte OperandAPresent = 0, OperandBPresent = 0;
	byte** OperandPtrs = malloc(sizeof(byte*) * 2);
	OperandPtrs[0] = &OperandAPresent;
	OperandPtrs[1] = &OperandBPresent;

	char* OperandA, * OperandB;
	char* Temporary = malloc(64);
	memset(Temporary, 0, 64);
	OperandA = malloc(64);
	OperandB = malloc(64);
	u64 OperandAPhys = 0, OperandBPhys = 0;
	u64** OperandPhysPtrs;
	OperandPhysPtrs = malloc(sizeof(u64*) * 2);
	OperandPhysPtrs[0] = &OperandAPhys;
	OperandPhysPtrs[1] = &OperandBPhys;
	char** OperandNamePtrs[2];
	OperandNamePtrs[0] = &OperandA;
	OperandNamePtrs[1] = &OperandB;
	byte OperandPtrSizes[2] = { 0, 0 };
	byte OperandSingleByte = 0x00;

	if (strstr(Line, "\n"))
		*(char*)(strstr(Line, "\n")) = 0x00;

	// Grab operand name
	if (Line[0] != '-') {
		while (Line[c] && Line[c] != ' ')
			Temporary[c] = Line[c++] & ~0x20;
	} else {
		while (Line[c] && Line[c] != ' ')
			Temporary[c] = Line[c++];
	}
	c++;
	Temporary[c] = 0x00;

	if (Temporary[0] == '-') { // pragmas
		switch (Temporary[1]) {
		case 'a': // address set (-a 0000)
			cgctx->DataPosition = strtoull(Temporary + 3, NULL, cgctx->CurrentRadix);
			fseek(PrimaryOutput, cgctx->DataPosition, SEEK_SET);
			break;
		case 'b': // base set
			cgctx->CurrentRadix = atoi(Temporary + 3);
			break;
		case 'c': // compile
			cg_compile();
			break;
		}
		goto ExitThis;
	}
	if (Temporary[strlen(Temporary) - 1] == ':') { // symbol resolve
		Temporary[strlen(Temporary) - 1] = 0x00;
		link_resolve(Temporary, cgctx->DataPosition);
		goto ExitThis;
	}

	int Psin = psin2i_getinstructionname(Temporary);

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
	}

	// Grab operand two
	t = 0;
	while (Line[c] && Line[c] != ',') {
		Temporary[t++] = Line[c++];
		OperandBPresent = 1;
	}
	if (OperandBPresent) {
		Temporary[t] = 0x00;
		strcpy(OperandB, Temporary);
	}

	for (int o = 0; o < psin2i_getoperandcnt(Psin); o++) {
		if (!OperandPtrs[o]) {
			cge_error(cgctx->CurrentLine, "[E1000]: Missing Operand %c", (o == 0) ? 'A' : 'B');
			goto ExitThis;
		}

		if (psin2i_getoperandtype(Psin, o) == 0) {
			if (OperandNamePtrs[o][0][0] != 'r') {
				cge_error(cgctx->CurrentLine, "[E1001]: Invalid syntax in Operand %c", (o == 0) ? 'A' : 'B');
				goto ExitThis;
			}
			*OperandPhysPtrs[o] = atoi(OperandNamePtrs[o] + 1);
			if (psin2i_getphyssize(Psin, o) == 4) {
				OperandSingleByte |= *OperandPhysPtrs[o] << (4 * (1 - o));
				OperandPtrSizes[o] = 4;
			} else {
				OperandPtrSizes[o] = psin2i_getphyssize(Psin, o);
			}
		} else {
			OperandPtrSizes[o] = psin2i_getphyssize(Psin, o);
			if (!InRange(**OperandNamePtrs[o], '0', '9')) { // symbol
				*OperandPhysPtrs[o] = link_getsymbol(OperandNamePtrs[o]);
			} else { // phyiscal number
				*OperandPhysPtrs[o] = strtoull(*OperandNamePtrs[o], NULL, cgctx->CurrentRadix);
			}
		}
	}

	byte TwoOpsOneByte = 0, DualOutput;
	if (OperandPtrSizes[0] == 4 && OperandPtrSizes[1] == 4) {
		TwoOpsOneByte = 1;
		union {
			byte Single;
			struct {
				byte DualA : 4;
				byte DualB : 4;
			};
		}SingleDual;
		SingleDual.DualA = *OperandPhysPtrs[0];
		SingleDual.DualB = *OperandPhysPtrs[1];
		DualOutput = SingleDual.Single;	
	} else {
		for (int i = 0; i < 2; i++) {
			if (OperandPtrSizes[i] == 4)
				OperandPtrSizes[i] = 8;
		}
	}

	cgp_put1(psin2i_getopcode(Psin));
	if (OperandSingleByte) {
		cgp_put1(OperandSingleByte);
	} else {
		if (TwoOpsOneByte) {
			cgp_put1(DualOutput);
		} else {
			if (OperandAPresent)
				cgp_putx(*OperandPhysPtrs[0], OperandPtrSizes[0] / 8);
			if (OperandBPresent)
				cgp_putx(*OperandPhysPtrs[1], OperandPtrSizes[1] / 8);
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