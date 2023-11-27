//
//  decoder_print.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "decoder.h"
#include "../emu.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DecoderPrint(const char* Format) {
    fprintf(stdout, "[DBG]: %s\n", Format);
	return;
}
