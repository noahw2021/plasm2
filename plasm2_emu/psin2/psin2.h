//
//  psin2.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../basetypes.h"

void Psin2Init(void);
void Psin2Shutdown(void);

typedef struct _PSIN_INSTRUCTION {
	// __MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers
	char Instruction[4];
	BYTE Opcode;
	struct {
		BYTE Type; // 0 = register, 1 = immediate
		BYTE PhysicalSize, AvailableSize;
		char OperandName[7];
	}Operands[2];
	BYTE OperandCount;
	BYTE TotalInstructionSize;
	char* InstructionDescription;
}PSIN_INSTRUCTION, *PPSIN_INSTRUCTION;

typedef struct PSIN2_CTX {
	PSIN_INSTRUCTION* Instructions;
	int InstructionCount;
}PSIN2_CTX, *PPSIN2_CTX;
extern PPSIN2_CTX Psin2Ctx;

void Psin2Load(void);
int Psin2Parse(const char* InstructionData);
int Psin2InstructionCount(void);

char* Psin2iGetInstructionName(int Id);
BYTE  Psin2iGetInstructinOpcode(int Id);
BYTE  Psin2iGetOperandType(int Id, BYTE OperandId);
BYTE  Psin2iGetPhysicalSize(int Id, BYTE OperandId);
BYTE  Psin2iGetAvailableSize(int Id, BYTE OperandId);
char* Psin2iGetOperandName(int Id, BYTE OperandId);
BYTE  Psin2iGetTotalSize(int Id);
char* Psin2iGetDescription(int Id);
int	  Psin2iGetOperandCount(int Id);
int   Psin2iGetInstructionByName(const char* Operand);
int   Psin2iGetInstructionByOpcode(BYTE Opcode);
