//
//  symbols_generate.c
//  plasm2_asm
//
//  Created by Noah Wooten on 11/21/23.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "symbols.h"

void SymbolsGenerate(void) {
    if (!SymbolsCtx)
        return;
    
    SYMBOL_FILE FileHeader;
    memset(&FileHeader, 0, sizeof(SYMBOL_FILE));
    
    time(&FileHeader.CompileDate);
    FileHeader.SymbolCount = SymbolsCtx->SymbolCount;
    FileHeader.SymbolVirtualAddress = sizeof(SYMBOL_FILE);
    
    fseek(SymbolsCtx->SymbolFile, 0, SEEK_SET);
    fwrite(&FileHeader, sizeof(SYMBOL_FILE), 1, SymbolsCtx->SymbolFile);
    
    fseek(SymbolsCtx->SymbolFile, FileHeader.SymbolVirtualAddress, SEEK_SET);
    fwrite(SymbolsCtx->ResolvedSymbols, sizeof(SYMBOL), SymbolsCtx->SymbolCount,
        SymbolsCtx->SymbolFile);
    
    return;
}
