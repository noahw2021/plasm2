#pragma once
#include "../basetypes.h"
/*
psin2.h
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void psin2_init(void);
void psin2_shutdown(void);

typedef struct _psininstruction {
	// __MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers
	char Instruction[4];
	byte Opcode;
	struct {
		byte Type; // 0 = register, 1 = immediate
		byte PhysicalSize, AvailableSize;
		char OperandName[7];
	}Operands[2];
	byte OperandCount;
	byte TotalInstructionSize;
	char* InstructionDescription;
}psininstruction_t;

typedef struct _psin2ctx {
	psininstruction_t* Instructions;
	int InstructionCount;
}psin2ctx_t;
extern psin2ctx_t* psin2ctx;

void psin2_load(void);
int psin2_parse(const char* InstructionData);
int psin2_getcnt(void);

char* psin2i_getname(int Id);
byte  psin2i_getopcode(int Id);
byte  psin2i_getoperandtype(int Id, byte OperandId);
byte  psin2i_getphyssize(int Id, byte OperandId);
byte  psin2i_getavailsize(int Id, byte OperandId);
char* psin2i_getoperandname(int Id, byte OperandId);
byte  psin2i_totalsize(int Id);
char* psin2i_description(int Id);
int	  psin2i_getoperandcnt(int Id);
int   psin2i_getinstructionbyname(const char* Operand);
int   psin2i_getinstructionbycd(byte Opcode);
