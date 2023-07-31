#include "token.h"
#include <stdlib.h>
#include <string.h>
/*
token_init.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6387)

tokenctx_t* tokenctx;

void token_init(void) {
	tokenctx = malloc(sizeof(tokenctx_t));
	memset(tokenctx, 0, sizeof(tokenctx_t));

	tokenctx->Started = 1;

	return;
}

void token_shutdown(void) {
	free(tokenctx);
}