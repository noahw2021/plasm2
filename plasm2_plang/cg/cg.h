#pragma once
#include "../basetypes.h"
/*
cg.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

void cg_init(void);
void cg_shutdown(void);

#define CG_TYPE_U64   0x01
#define CG_TYPE_U32   0x02
#define CG_TYPE_U16   0x03
#define CG_TYPE_BYTE  0x04 
#define CG_TYPE_STR   0x05
#define CG_TYPE_VOID  0x06

#define CG_TYPE_PU64  0x11
#define CG_TYPE_PU32  0x12
#define CG_TYPE_PU16  0x13
#define CG_TYPE_PBYTE 0x14 
#define CG_TYPE_PSTR  0x15
#define CG_TYPE_PVOID 0x16

#define CG_CTX_GLOBAL 0xA0044000

void cg_decvar(byte Type, u32 Ctx, char* Name);
u32 cg_decfunc(byte Type, char* Name, int ArgCnt, byte* ArgType);

void cg_callfunc(char* Name, int Argc, u64* Args);
void cg_manipvar(char* Name, u32 Ctx, int Method);
u32 cg_getfuncctx(char* Name);