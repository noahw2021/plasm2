//
//  symbols_init.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include "symbols.h"
#include "../decoder.h"

PSYMBOL_CTX MainCtx;

void SymbolsInit(void) {
    MainCtx = DecoderCtx->SymbolInformation;
    return;
}

void SymbolsShutdown(void) {
    return;
}
