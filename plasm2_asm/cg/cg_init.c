#include "cg.h"
#include <stdlib.h>
#include <string.h>
/*
cg_init.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

cgctx_t* cgctx;

#pragma warning(disable: 6387)

void cg_init(void) {
	cgctx = malloc(sizeof(cgctx_t));
	memset(cgctx, 0, sizeof(cgctx_t));

	cgctx->CurrentRadix = 10;

	return;
}

void cg_shutdown(void) {
	if (cgctx->Errors)
		free(cgctx->Errors);
	free(cgctx);

	return;
}