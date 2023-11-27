//
//  decoder_init.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include "decoder.h"
#include "../symbols/symbols.h"
#include <stdlib.h>
#include <string.h>


PDECODER_CTX DecoderCtx;

void DecoderInit(void) {
    if (DecoderCtx)
        DecoderShutdown();
    
    DecoderCtx = malloc(sizeof(DECODER_CTX));
    memset(DecoderCtx, 0, sizeof(DECODER_CTX));
    return;
}

void DecoderShutdown(void) {
    if (!DecoderCtx)
        return;
    
    if (DecoderCtx->SymbolInformation)
        free(DecoderCtx->SymbolInformation);
    
    free(DecoderCtx);
    return;
}

void DecoderSetSymbolFile(FILE* SymbolFile) {
    if (!DecoderCtx->SymbolInformation) {        DecoderCtx->SymbolInformation =
            malloc(sizeof(DECODER_CTX));
        memset(DecoderCtx->SymbolInformation, 0,
               sizeof(DECODER_CTX));
    }
    
    DecoderCtx->SymbolInformation->SymbolFile =
        SymbolFile;
    
    return;
}

void DecoderSetData(BYTE* Data, WORD32 DataLength) {
    DecoderCtx->Data = Data;
    DecoderCtx->DataLength = DataLength;
    return;
}

void DecoderSetOutputFile(FILE* OutFile) {
    DecoderCtx->OutputFile = OutFile;
    return;
}
