//
//  decoder_print.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include <stdarg.h>
#include "decoder.h"

void DecoderPrint(const char* Format, ...) {
    printf("%s", Format);
}
