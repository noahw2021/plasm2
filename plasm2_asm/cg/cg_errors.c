//
//  cg_errors.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#pragma warning(disable: 6308 6387 26451 28182)

void CgeError(int Line, const char* Reason, ...) {
	if (!CgCtx->Errors) {
		CgCtx->Errors = malloc(sizeof(CgCtx->Errors[0]));
		memset(CgCtx->Errors, 0, sizeof(CgCtx->Errors[0]));
		CgCtx->ErrorCount++;
	} else {
		CgCtx->Errors = realloc(CgCtx->Errors, (sizeof(CgCtx->Errors[0]) * (CgCtx->ErrorCount + 1)));
		memset(&CgCtx->Errors[CgCtx->ErrorCount], 0, sizeof(CgCtx->Errors[0]));
		CgCtx->ErrorCount++;
	}

	va_list VaList;
	va_start(VaList, Reason);
	char* ReasonBuffer = malloc(768);
	vsprintf(ReasonBuffer, Reason, VaList);
	va_end(VaList);

	CgCtx->Errors[CgCtx->ErrorCount - 1].Line = Line;
	CgCtx->Errors[CgCtx->ErrorCount - 1].Reason = ReasonBuffer;

	return;
}
	
int  CgeErrorCount(void) {
	return CgCtx->ErrorCount;
}

char* CgeGetErrorString(int i) {
	for (int e = 0; e < CgCtx->ErrorCount; e++) {
		if (e != i)
			continue;
		char* Return = malloc(512);

		if (CgCtx->Errors[e].Line == 0xFFFF)
			sprintf(Return, "[LINK]: %s", CgCtx->Errors[e].Reason);
		else
			sprintf(Return, "[%04i]: %s", CgCtx->Errors[e].Line, CgCtx->Errors[e].Reason);

		return Return;
	}

	return NULL;
}
