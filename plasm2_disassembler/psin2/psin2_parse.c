//
//  psin2_parse.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "psin2.h"

#pragma warning(disable: 6308 26451 28182)

int Psin2Parse(const char* InstructionData) {
	if (InstructionData[0] == '/')
		return 0;

	if (!Psin2Ctx->Instructions) {
		Psin2Ctx->Instructions = malloc(sizeof(PSIN_INSTRUCTION) * (Psin2Ctx->InstructionCount + 1));
		memset(Psin2Ctx->Instructions, 0, sizeof(PSIN_INSTRUCTION) * (Psin2Ctx->InstructionCount + 1));
	} else {
		Psin2Ctx->Instructions = realloc(Psin2Ctx->Instructions, (sizeof(PSIN_INSTRUCTION) * (Psin2Ctx->InstructionCount + 1)));
		memset(&Psin2Ctx->Instructions[Psin2Ctx->InstructionCount], 0, sizeof(PSIN_INSTRUCTION));
	}

	

	PPSIN_INSTRUCTION Target = &Psin2Ctx->Instructions[Psin2Ctx->InstructionCount];
	Psin2Ctx->InstructionCount++;

	char* Temporary = malloc(256);

	// __MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers

	int c = 0;
	while (InstructionData[c++] != '/')
		c++;
	c += 1; // first char = 'M' in example

	if (InstructionData[c] == ' ')
		c++;

	memcpy(Target->Instruction, InstructionData + c, 3); // MOV
	Target->Instruction[3] = 0x00;
	c += 4;

	memcpy(Temporary, InstructionData + c, 2);
	Temporary[2] = 0x00;
	Target->Opcode = (BYTE)strtoul(Temporary, NULL, 16);
	c += 3;

	if (InstructionData[c] == '(') { // Operand A
		Target->OperandCount++;
		c++;
		if (InstructionData[c] == 'R') {
			Target->Operands[0].Type = 0;
		} else if (InstructionData[c] == 'I') {
			Target->Operands[0].Type = 1;
		} else {
			printf("[ERR]: Invalid operand type or syntax error. Check line %i.\n", Psin2Ctx->InstructionCount);
			if (Psin2Ctx->InstructionCount == 1)
				free(Psin2Ctx->Instructions);
			else
				Psin2Ctx->Instructions = realloc(Psin2Ctx->Instructions, sizeof(PSIN_INSTRUCTION) * (Psin2Ctx->InstructionCount - 1));
			Psin2Ctx->InstructionCount--;
			return 0;
		}
		c += 2;
		
		memcpy(Temporary, InstructionData + c, 2);
		Temporary[2] = 0x00;
		Target->Operands[0].PhysicalSize = atoi(Temporary);
		c += 3;
		memcpy(Temporary, InstructionData + c, 2);
		Target->Operands[0].AvailableSize = atoi(Temporary);
		c += 3;

		memcpy(Temporary, InstructionData + c, 6);
		Temporary[6] = 0x00;
		for (int i = 0; i < 6; i++) {
			if (Temporary[i] == '_')
				Temporary[i] = ' ';
		}
		strcpy(Target->Operands[0].OperandName, Temporary);
		c += 8;

		if (InstructionData[c] == '(') { // operand b
			Target->OperandCount++;
			c++;
			if (InstructionData[c] == 'R') {
				Target->Operands[1].Type = 0;
			}
			else if (InstructionData[c] == 'I') {
				Target->Operands[1].Type = 1;
			}
			else {
				printf("[ERR]: Invalid operand type or syntax error. Check line %i.\n", Psin2Ctx->InstructionCount);
				if (Psin2Ctx->InstructionCount == 1)
					free(Psin2Ctx->Instructions);
				else
					Psin2Ctx->Instructions = realloc(Psin2Ctx->Instructions, sizeof(PSIN_INSTRUCTION) * (Psin2Ctx->InstructionCount - 1));
				Psin2Ctx->InstructionCount--;
				return 0;
			}
			c += 2;

			memcpy(Temporary, InstructionData + c, 2);
			Temporary[2] = 0x00;
			Target->Operands[1].PhysicalSize = atoi(Temporary);
			c += 3;
			memcpy(Temporary, InstructionData + c, 2);
			Target->Operands[1].AvailableSize = atoi(Temporary);
			c += 3;

			memcpy(Temporary, InstructionData + c, 6);
			Temporary[6] = 0x00;
			for (int i = 0; i < 6; i++) {
				if (Temporary[i] == '_')
					Temporary[i] = ' ';
			}
			strcpy(Target->Operands[1].OperandName, Temporary);
			c += 8;
		} else {
			c += 17;
		}
	} else { // account for no register being there
		c += 34;
	}

	memcpy(Temporary, InstructionData + c, 2);
	Temporary[2] = 0x00;
	Target->TotalInstructionSize = atoi(Temporary);
	c += 5;

	int DescriptionStringLength = 0;
	while (InstructionData[c + DescriptionStringLength])
		DescriptionStringLength++;
	Target->InstructionDescription = malloc(DescriptionStringLength + 1);
	strcpy(Target->InstructionDescription, InstructionData + c);

	// :)
	free(Temporary);
	return 0;
}

int Psin2InstructionCount(void) {
	return Psin2Ctx->InstructionCount;
}
