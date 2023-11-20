//
//  psin2_parse.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "psin2.h"

#pragma warning(disable: 6308 26451 28182)

int psin2_parse(const char* InstructionData) {
	if (InstructionData[0] == '/')
		return psin2ctx->InstructionCount;

	if (!psin2ctx->Instructions) {
		psin2ctx->Instructions = malloc(sizeof(psininstruction_t) * (psin2ctx->InstructionCount + 1));
		memset(psin2ctx->Instructions, 0, sizeof(psininstruction_t) * (psin2ctx->InstructionCount + 1));
	} else {
		psin2ctx->Instructions = realloc(psin2ctx->Instructions, (sizeof(psininstruction_t) * (psin2ctx->InstructionCount + 1)));
		memset(&psin2ctx->Instructions[psin2ctx->InstructionCount], 0, sizeof(psininstruction_t));
	}

	

	psininstruction_t* Target = &psin2ctx->Instructions[psin2ctx->InstructionCount];
	psin2ctx->InstructionCount++;

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
	Target->Opcode = (byte)strtoul(Temporary, NULL, 16);
	c += 3;

	if (InstructionData[c] == '(') { // Operand A
		Target->OperandCount++;
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
			return psin2ctx->InstructionCount;
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
				printf("[ERR]: Invalid operand type or syntax error. Check line %i.\n", psin2ctx->InstructionCount);
				if (psin2ctx->InstructionCount == 1)
					free(psin2ctx->Instructions);
				else
					psin2ctx->Instructions = realloc(psin2ctx->Instructions, sizeof(psininstruction_t) * (psin2ctx->InstructionCount - 1));
				psin2ctx->InstructionCount--;
				return psin2ctx->InstructionCount;
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
	return psin2ctx->InstructionCount;
}

int psin2_getcnt(void) {
	return psin2ctx->InstructionCount;
}
