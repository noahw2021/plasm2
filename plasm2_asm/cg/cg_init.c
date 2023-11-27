//
//  cg_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include <stdlib.h>
#include <string.h>

PCG_CTX CgCtx;

#pragma warning(disable: 6001 6011 6387)

void CgInit(void) {
	CgCtx = malloc(sizeof(CG_CTX));
	memset(CgCtx, 0, sizeof(CG_CTX));

	CgCtx->CurrentRadix = 10;
	CgCtx->Shutdown = 0;

	return;
}

void CgShutdown(void) {
	if (CgCtx->Errors) {
		for (int i = 0; i < CgCtx->ErrorCount; i++) {
			if (CgCtx->Errors[i].Reason)
				free(CgCtx->Errors[i].Reason);
		}
		free(CgCtx->Errors);
	}
	free(CgCtx);
	CgCtx = NULL;

	return;
}
