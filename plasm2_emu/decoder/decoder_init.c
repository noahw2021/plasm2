#include "decoder.h"
#include <stdlib.h>
#include <string.h>

/*
decoder_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

dcctx_t* dcctx;

void decoder_init(void) {
	dcctx = malloc(sizeof(dcctx_t));
	memset(dcctx, 0, sizeof(dcctx_t));
	return;
}

void decoder_shutdown(void) {
	free(dcctx);
}