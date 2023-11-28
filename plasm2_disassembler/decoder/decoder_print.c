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
    va_list List;
    va_start(List, Format);
    vfprintf(stdout, Format, List);
    va_end(List);
    return;
}
