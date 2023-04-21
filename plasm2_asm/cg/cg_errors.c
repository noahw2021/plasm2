#include "cg.h"
#include <stdlib.h>
#include <string.h>
/*
cg_errors.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6308 6387 26451 28182)

void cge_error(int Line, const char* Reason) {
	if (!cgctx->Errors) {
		cgctx->Errors = malloc(sizeof(cgctx->Errors[0]));
		memset(cgctx->Errors, 0, sizeof(cgctx->Errors[0]));
		cgctx->ErrorCount++;
	} else {
		cgctx->Errors = realloc(cgctx->Errors, (sizeof(cgctx->Errors[0]) * (cgctx->ErrorCount + 1)));
		memset(&cgctx->Errors[cgctx->ErrorCount], 0, sizeof(cgctx->Errors[0]));
		cgctx->ErrorCount++;
	}

	cgctx->Errors[cgctx->ErrorCount - 1].Line = Line;
	cgctx->Errors[cgctx->ErrorCount - 1].Reason = Reason;

	return;
}
	
int  cge_errorcnt(void) {
	return cgctx->ErrorCount;
}