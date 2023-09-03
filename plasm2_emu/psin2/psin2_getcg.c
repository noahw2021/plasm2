#include "psin2.h"
#include <stdlib.h>
#include <string.h>
/*
psin2_getcg.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

char* psin2i_getname(int Id) {
	return psin2ctx->Instructions[Id].Instruction;
}

byte  psin2i_getopcode(int Id) {
	return psin2ctx->Instructions[Id].Opcode;
}

byte  psin2i_getoperandtype(int Id, byte OperandId) {
	return psin2ctx->Instructions[Id].Operands[OperandId].Type;
}

byte  psin2i_getphyssize(int Id, byte OperandId) {
	return psin2ctx->Instructions[Id].Operands[OperandId].PhysicalSize;
}

byte  psin2i_getavailsize(int Id, byte OperandId) {
	return psin2ctx->Instructions[Id].Operands[OperandId].AvailableSize;
}

char* psin2i_getoperandname(int Id, byte OperandId) {
	char* ret = psin2ctx->Instructions[Id].Operands[OperandId].OperandName;
	int r = 0;
	while (ret[r] == ' ')
		r++;
	return ret + r;
}

byte  psin2i_totalsize(int Id) {
	return psin2ctx->Instructions[Id].TotalInstructionSize;
}

char* psin2i_description(int Id) {
	return psin2ctx->Instructions[Id].InstructionDescription;
}

int	  psin2i_getoperandcnt(int Id) {
	return psin2ctx->Instructions[Id].OperandCount;
}

int  psin2i_getinstructionname(const char* Operand) {
	for (int i = 0; i < psin2ctx->InstructionCount; i++) {
		if (!strcmp(psin2ctx->Instructions[i].Instruction, Operand))
			return i;
	}
	return -1;
}

int   psin2i_getinstructionbycd(byte Opcode) {
	for (int i = 0; i < psin2ctx->InstructionCount; i++) {
		if (psin2ctx->Instructions[i].Opcode == Opcode)
			return i;
	}
	return 0xFFFF;
}