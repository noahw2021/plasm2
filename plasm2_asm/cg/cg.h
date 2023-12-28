//
//  cg.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#pragma once
#include "../basetypes.h"

void CgInit(void);
void CgShutdown(void);

void CgParse(const char* Line);
void CgCompile(void);

void CgeError(int Line, const char* Reason, ...);
int  CgeErrorCount(void);
char* CgeGetErrorString(int i);

void CgpPut1(BYTE Data);
void CgpPut8(WORD64 Data);
void CgpPutX(WORD64 Data, BYTE ByteSize);

typedef struct _CG_CTX {
	int LineCount;
	int CurrentLine;
	int CurrentRadix;

	WORD64 DataPosition;
	WORD64 HighestPosition;
	WORD64 ReferencePtr;
	BYTE InSub;

	int ErrorCount;
	struct {
		int Line;
		char* Reason;
	}*Errors;

	BYTE Shutdown;
}CG_CTX, *PCG_CTX;
extern PCG_CTX CgCtx;
