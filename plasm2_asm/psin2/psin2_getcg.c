#include "psin2.h"
#include <stdlib.h>
#include <string.h>
/*
psin2_getcg.c
plasm2
plasm2_asm
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