//
//  decoder_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "decoder.h"
#include <stdlib.h>
#include <string.h>

PDC_CTX DcCtx;

void DecoderInit(void) {
	DcCtx = malloc(sizeof(DC_CTX));
	memset(DcCtx, 0, sizeof(DC_CTX));
	return;
}

void DecoderShutdown(void) {
	free(DcCtx);
}
