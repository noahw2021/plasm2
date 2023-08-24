#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include "../vf/vf.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
cg_parse.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6308 6387 28182)

extern FILE* PrimaryOutput;

void cg_parse(const char* Line) {
	cgctx->CurrentLine++;

	char* CommentCorrect = (char*)Line;
	// incredibly basic and temporary comment system
	if (strstr(CommentCorrect, ";"))
		*(char*)strstr(CommentCorrect, ";") = 0x00;

	if (CommentCorrect[0] == 0x00 || CommentCorrect[0] == '\n')
		return;

	// MOV r0, r1
	int c = 0, t = 0;
	byte OperandAPresent = 0, OperandBPresent = 0;
	byte** OperandPtrs = malloc(sizeof(byte*) * 2);
	OperandPtrs[0] = &OperandAPresent;
	OperandPtrs[1] = &OperandBPresent;

	char* OperandA, * OperandB;
	char* Temporary = malloc(256);
	memset(Temporary, 0, 256);
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

	int Usage = 0;
	char* FollowString = (char*)Line + 3;

	if (Temporary[0] == '-') { // pragmas
		switch (Temporary[1]) {
		case 'a': // address set (-a 0000)
			cgctx->DataPosition = strtoull(Line + 3, NULL, cgctx->CurrentRadix);
			fseek(PrimaryOutput, (u32)cgctx->DataPosition, SEEK_SET);
			break;
		case 'b': // base set
			cgctx->CurrentRadix = atoi(Line  + 3);
			break;
		case 'c': // compile
			cgctx->DataPosition = cgctx->HighestPosition;
			cg_compile();
			break;
		case 'f': // fill
			vfg_write(Line + 3, cgctx->DataPosition);
			break;
		case 'i': // include
			vf_register(FollowString);
			break;
		case 'p': // pad with zero until x size
			Usage = (int)strtoull(Line + 3, NULL, cgctx->CurrentRadix);
			cgctx->DataPosition = cgctx->HighestPosition;
			while (cgctx->DataPosition < Usage)
				cgp_put1('\0');
			break;
		case 'r': // set the current reference (location in memory)
			cgctx->ReferencePtr = strtoull(Line + 3, NULL, cgctx->CurrentRadix);
			break;
		case 's': // string
			Usage = 1;
			while (FollowString[Usage] != '"')
				cgp_put1(FollowString[Usage++]);
			cgp_put1('\0');
			break;
		case 'z': // fill with zero
			Usage = (int)strtoull(Line + 3, NULL, cgctx->CurrentRadix);
			for (int i = 0; i < Usage; i++)
				cgp_put1('\0');
			break;
		}
		goto ExitThis;
	}

	char* _Line = (char*)Line;
	int __x = strlen(Line);
	int Count = 0;
	while (_Line[__x - Count] != ':') {
		Count++;
		if (Count == __x)
			break;
	}
	if (Count != __x) {
		if (strstr(_Line, ":"))
			*(char*)strstr(_Line, ":") = 0x00;
		link_resolve(Line, cgctx->DataPosition);
		goto ExitThis;
	}

	int Psin = psin2i_getinstructionname(Temporary);

	if (Psin == -1) {
		cge_error(cgctx->CurrentLine, "[E1002]: Invalid operand");
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

		if (!InRange(OperandA[0], '0', '9')) {
			if (OperandA[0] != 'r') {
				byte Found = 0;
				for (int i = 0; i < linkctx->SymbolCount; i++) {
					if (!strcmp(OperandA, linkctx->Symbols[i].SymbolName)) {
						Found = 1;
						if (linkctx->Symbols[i].Resolved) {
							char* ToString = malloc(64);
							if (cgctx->CurrentRadix == 16)
								sprintf(ToString, "%llX", linkctx->Symbols[i].Resolution);
							else
								sprintf(ToString, "%llu", linkctx->Symbols[i].Resolution);
							strcpy(OperandA, ToString);
							free(ToString);
						}
						else {
							linkctx->Symbols[i].Locations = realloc(linkctx->Symbols[i].Locations, (sizeof(linkctx->Symbols[i].Locations[0]) * (linkctx->Symbols[i].LocationCount)));
							linkctx->Symbols[i].Locations[linkctx->Symbols[i].LocationCount].Location = cgctx->DataPosition + 1;
							linkctx->Symbols[i].LocationCount++;
							strcpy(OperandA, "0");
						}
					}
				}
				if (!Found) {
					char* ToString = malloc(64);
					sprintf(ToString, "%llu", link_getsymbol(OperandA, 1)); // base doens't matter, will always be 0
					strcpy(OperandA, ToString);
					free(ToString);
				}
			}
		}
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

		if (!InRange(OperandB[0], '0', '9')) {
			if (OperandB[0] != 'r') {
				byte Found = 0;
				for (int i = 0; i < linkctx->SymbolCount; i++) {
					if (!strcmp(OperandB, linkctx->Symbols[i].SymbolName)) {
						Found = 1;
						if (linkctx->Symbols[i].Resolved) {
							char* ToString = malloc(64);
							if (cgctx->CurrentRadix == 16)
								sprintf(ToString, "%llX", linkctx->Symbols[i].Resolution);
							else
								sprintf(ToString, "%llu", linkctx->Symbols[i].Resolution);
							strcpy(OperandB, ToString);
							free(ToString);
						}
						else {
							linkctx->Symbols[i].Locations = realloc(linkctx->Symbols[i].Locations, (sizeof(linkctx->Symbols[i].Locations[0]) * (linkctx->Symbols[i].LocationCount)));
							linkctx->Symbols[i].Locations[linkctx->Symbols[i].LocationCount].Location = cgctx->DataPosition + (psin2i_getphyssize(Psin, 0) / 8) + 1;
							linkctx->Symbols[i].LocationCount++;
							strcpy(OperandB, "0");
						}
					}
				}
				if (!Found) {
					char* ToString = malloc(64);
					sprintf(ToString, "%llu", link_getsymbol(OperandB, (psin2i_getphyssize(Psin, 0) / 8) + 1)); // base doens't matter, will always be 0
					strcpy(OperandB, ToString);
					free(ToString);
				}
			}
		}
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
			OperandPhysPtrs[o][0] = atoi(OperandNamePtrs[o][0] + 1);
			if (psin2i_getphyssize(Psin, o) == 4) {
				OperandSingleByte |= *OperandPhysPtrs[o] << (4 * (1 - o));
				OperandPtrSizes[o] = 4;
			} else {
				OperandPtrSizes[o] = psin2i_getphyssize(Psin, o);
			}
		} else {
			OperandPtrSizes[o] = psin2i_getphyssize(Psin, o);
			*OperandPhysPtrs[o] = strtoull(*OperandNamePtrs[o], NULL, cgctx->CurrentRadix);
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

		SingleDual.DualA = (byte)*OperandPhysPtrs[0];
		SingleDual.DualB = (byte)*OperandPhysPtrs[1];

		DualOutput = SingleDual.Single;	
	} else {
		for (int i = 0; i < 2; i++) {
			if (OperandPtrSizes[i] == 4)
				OperandPtrSizes[i] = 8;
		}
	}

 	cgp_put1(psin2i_getopcode(Psin));
	if (OperandSingleByte && OperandPtrSizes[0] == 4) {
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