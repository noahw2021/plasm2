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
    printf("[DBG]: ");
    
    int i = 0;
    while (Format[i]) {
        if (Format[i] == -18) {
            i++;
            continue;
        }

        fputc(Format[i], stdout);
        i++;
    }

    fputc('\n', stdout);
}
