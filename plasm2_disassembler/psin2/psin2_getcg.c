//
//  psin2_getcg.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//
#include "psin2.h"
#include <stdlib.h>
#include <string.h>

char* Psin2iGetInstructionName(int Id) {
	return Psin2Ctx->Instructions[Id].Instruction;
}

BYTE  Psin2iGetInstructinOpcode(int Id) {
	return Psin2Ctx->Instructions[Id].Opcode;
}

BYTE  Psin2iGetOperandType(int Id, BYTE OperandId) {
	return Psin2Ctx->Instructions[Id].Operands[OperandId].Type;
}

BYTE  Psin2iGetPhysicalSize(int Id, BYTE OperandId) {
	return Psin2Ctx->Instructions[Id].Operands[OperandId].PhysicalSize;
}

BYTE  Psin2iGetAvailableSize(int Id, BYTE OperandId) {
	return Psin2Ctx->Instructions[Id].Operands[OperandId].AvailableSize;
}

char* Psin2iGetOperandName(int Id, BYTE OperandId) {
	char* ret = Psin2Ctx->Instructions[Id].Operands[OperandId].OperandName;
	int r = 0;
	while (ret[r] == ' ')
		r++;
	return ret + r;
}

BYTE  Psin2iGetTotalSize(int Id) {
	return Psin2Ctx->Instructions[Id].TotalInstructionSize;
}

char* Psin2iGetDescription(int Id) {
	return Psin2Ctx->Instructions[Id].InstructionDescription;
}

int	  Psin2iGetOperandCount(int Id) {
	return Psin2Ctx->Instructions[Id].OperandCount;
}

int  Psin2iGetInstructionByName(const char* Operand) {
	for (int i = 0; i < Psin2Ctx->InstructionCount; i++) {
		if (Psin2Ctx->Instructions[i].Instruction[0] == Operand[0] &&
            Psin2Ctx->Instructions[i].Instruction[1] == Operand[1] &&
            Psin2Ctx->Instructions[i].Instruction[2] == Operand[2]
        ) {
            return i;
        }
	}
	return -1;
}

int   Psin2iGetInstructionByOpcode(BYTE Opcode) {
	for (int i = 0; i < Psin2Ctx->InstructionCount; i++) {
		if (Psin2Ctx->Instructions[i].Opcode == Opcode)
			return i;
	}
	return 0;
}
