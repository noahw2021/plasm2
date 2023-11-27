//
//  symbols_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include "symbols.h"

PSYMBOL_CTX SymbolsCtx;

void SymbolsInit(void) {
    SymbolsCtx = malloc(sizeof(SYMBOL_CTX));
    memset(SymbolsCtx, 0, sizeof(SYMBOL_CTX));
    return;
}

void SymbolsShutdown(void) {
    if (!SymbolsCtx)
        return;
    
    if (SymbolsCtx->SymbolFile)
        fclose(SymbolCtx->SymbolFile);
    if (SymbolsCtx->ResolvedSymbols)
        free(SymbolsCtx->ResolvedSymbols);
    
    free(SymbolsCtx);
    return;
}

void SymbolsSetFile(FILE* OutFile) {
    if (!SymbolsCtx)
        return;
    
    SymbolsCtx->SymbolFile = OutFile;
    return;
}
