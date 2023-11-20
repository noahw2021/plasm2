//
//  cg_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include <stdlib.h>
#include <string.h>

cgctx_t* cgctx;

#pragma warning(disable: 6001 6011 6387)

void cg_init(void) {
	cgctx = malloc(sizeof(cgctx_t));
	memset(cgctx, 0, sizeof(cgctx_t));

	cgctx->CurrentRadix = 10;
	cgctx->Shutdown = 0;

	return;
}

void cg_shutdown(void) {
	if (cgctx->Errors) {
		for (int i = 0; i < cgctx->ErrorCount; i++) {
			if (cgctx->Errors[i].Reason)
				free(cgctx->Errors[i].Reason);
		}
		free(cgctx->Errors);
	}
	free(cgctx);
	cgctx = NULL;

	return;
}
