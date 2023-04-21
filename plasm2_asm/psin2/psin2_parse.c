#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "psin2.h"
/*
psin2_data.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 26451)

int psin2_parse(const char* InstructionData) {
	if (!psin2ctx->Instructions) {
		psin2ctx->Instructions = malloc(sizeof(psininstruction_t) * (psin2ctx->InstructionCount + 1));
		memset(psin2ctx->Instructions, 0, sizeof(psininstruction_t * (psin2ctx->InstructionCount + 1)));
	} else {
		psin2ctx->Instructions = realloc(psin2ctx->Instructions, (sizeof(psininstruction_t * (psin2ctx->InstructionCount + 1)));
		memset(&psin2ctx->Instructions[psin2ctx->InstructionCount], 0, sizeof(psininstruction_t));
	}

	psininstruction_t* Target = &psin2ctx->Instructions[psin2ctx->InstructionCount];
	psin2ctx->InstructionCount++;

	char Temporary = malloc(64);

	// __MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers

	int c = 0;
	while (InstructionData[c++] != '/')
		c++;
	c += 2; // first char = 'M' in example

	memcpy(Target->Instruction, InstructionData[c], 4); // MOV
	c += 5;

	memcpy(Temporary, InstructionData[c], 2);
	InstructionData[c + 2] = 0x00;
	Target->Opcode = strtoul(Temporary, NULL, 16);
	c += 3;

	if (InstructionData[c] == '(') { // Operand A
		c++;
		if (InstructionData[c] == 'R') {
			Target->Operands[0].Type = 0;
		} else if (InstructionData[c] == 'I') {
			Target->Operands[0].Type = 1;
		} else {
			printf("[ERR]: Invalid operand type or syntax error. Check line %i.\n", psin2ctx->InstructionCount);
			if (psin2ctx->InstructionCount == 1)
				free(psin2ctx->Instructions);
			else
				psin2ctx->Instructions = realloc(psin2ctx->Instructions, sizeof(psininstruction_t) * (psin2ctx->InstructionCount - 1));
			psin2ctx->InstructionCount--;
			return;
		}
		c += 2;
		
		memcpy(Temporary, InstructionData[c], 2);
		Temporary[2] = 0x00;
		Target->Operands[0].PhysicalSize = atoi(Temporary);
		c += 3;
		memcpy(Temporary, InstructionData[c], 2);
		Target->Operands[0].AvailableSize = atoi(Temporary);
		c += 3;

		memcpy(Temporary, InstructionData[c], 6);
		Temporary[6] = 0x00;
		for (int i = 0; i < 6; i++) {
			if (Temporary[i] == '_')
				Temporary[i] = ' ';
		}
		strcpy(Target->Operands[0].OperandName, Temporary);
		c += 2;

		if (InstructionData[c] == '(') { // operand b
			c++;
			if (InstructionData[c] == 'R') {
				Target->Operands[1].Type = 0;
			}
			else if (InstructionData[c] == 'I') {
				Target->Operands[1].Type = 1;
			}
			else {
				printf("[ERR]: Invalid operand type or syntax error. Check line %i.\n", psin2ctx->InstructionCount);
				if (psin2ctx->InstructionCount == 1)
					free(psin2ctx->Instructions);
				else
					psin2ctx->Instructions = realloc(psin2ctx->Instructions, sizeof(psininstruction_t) * (psin2ctx->InstructionCount - 1));
				psin2ctx->InstructionCount--;
				return;
			}
			c += 2;

			memcpy(Temporary, InstructionData[c], 2);
			Temporary[2] = 0x00;
			Target->Operands[1].PhysicalSize = atoi(Temporary);
			c += 3;
			memcpy(Temporary, InstructionData[c], 2);
			Target->Operands[1].AvailableSize = atoi(Temporary);
			c += 3;

			memcpy(Temporary, InstructionData[c], 6);
			Temporary[6] = 0x00;
			for (int i = 0; i < 6; i++) {
				if (Temporary[i] == '_')
					Temporary[i] = ' ';
			}
			strcpy(Target->Operands[1].OperandName, Temporary);
			c += 2;
		} else {
			c += 18;
		}
	} else { // account for no register being there
		c += 34;
	}

	memcpy(Temporary, InstructionData[c], 2);
	Temporary[2] = 0x00;
	Target->TotalInstructionSize = atoi(Temporary);
	c += 5;

	int DescriptionStringLength = 0;
	while (InstructionData[c + DescriptionStringLength])
		DescriptionStringLength++;
	Target->InstructionDescription = malloc(DescriptionStringLength + 1);
	strcpy(Target->InstructionDescription, InstructionData[c]);

	// :)
	return;
}

int psin2_getcnt(void) {
	return psin2ctx->InstructionCount;
}