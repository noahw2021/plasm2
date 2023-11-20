//
//  decoder_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "decoder.h"
#include <stdlib.h>
#include <string.h>

dcctx_t* dcctx;

void decoder_init(void) {
	dcctx = malloc(sizeof(dcctx_t));
	memset(dcctx, 0, sizeof(dcctx_t));
	return;
}

void decoder_shutdown(void) {
	free(dcctx);
}
