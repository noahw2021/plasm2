#pragma once
#include "../basetypes.h"
/*
cg.h
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void cg_init(void);
void cg_shutdown(void);

void cg_parse(const char* Line);
void cg_compile(void);

void cge_error(int Line, const char* Reason, ...);
int  cge_errorcnt(void);
char* cge_geterr(int i );

void cgp_put1(byte Data);
void cgp_put8(u64 Data);
void cgp_putx(u64 Data, byte ByteSize);

typedef struct _cgctx {
	int LineCount;
	int CurrentLine;
	int CurrentRadix;

	u64 DataPosition;
	u64 HighestPosition;
	u64 ReferencePtr;
	byte InSub;

	int ErrorCount;
	struct {
		int Line;
		char* Reason;
	}*Errors;

	byte Shutdown;
}cgctx_t;
extern cgctx_t* cgctx;