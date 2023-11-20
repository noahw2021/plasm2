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

void cge_error(int Line, const char* Reason, ...) {
	if (!cgctx->Errors) {
		cgctx->Errors = malloc(sizeof(cgctx->Errors[0]));
		memset(cgctx->Errors, 0, sizeof(cgctx->Errors[0]));
		cgctx->ErrorCount++;
	} else {
		cgctx->Errors = realloc(cgctx->Errors, (sizeof(cgctx->Errors[0]) * (cgctx->ErrorCount + 1)));
		memset(&cgctx->Errors[cgctx->ErrorCount], 0, sizeof(cgctx->Errors[0]));
		cgctx->ErrorCount++;
	}

	va_list VaList;
	va_start(VaList, Reason);
	char* ReasonBuffer = malloc(768);
	vsprintf(ReasonBuffer, Reason, VaList);
	va_end(VaList);

	cgctx->Errors[cgctx->ErrorCount - 1].Line = Line;
	cgctx->Errors[cgctx->ErrorCount - 1].Reason = ReasonBuffer;

	return;
}
	
int  cge_errorcnt(void) {
	return cgctx->ErrorCount;
}

char* cge_geterr(int i) {
	for (int e = 0; e < cgctx->ErrorCount; e++) {
		if (e != i)
			continue;
		char* Return = malloc(512);

		if (cgctx->Errors[e].Line == 0xFFFF)
			sprintf(Return, "[LINK]: %s", cgctx->Errors[e].Reason);
		else
			sprintf(Return, "[%04i]: %s", cgctx->Errors[e].Line, cgctx->Errors[e].Reason);

		return Return;
	}

	return NULL;
}
