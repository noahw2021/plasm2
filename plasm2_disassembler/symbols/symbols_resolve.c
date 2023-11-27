//
//  symbols_resolve.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/21/23.
//

#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

void SymbolsResolve(void) {
    SYMBOL_FILE FileHeader;
    fseek(MainCtx->SymbolFile, 0, SEEK_SET);
    fread(&FileHeader, sizeof(SYMBOL_FILE), 1, MainCtx->SymbolFile);
    
    PSYMBOL Files = malloc(sizeof(SYMBOL) * FileHeader.SymbolCount);
    fseek(MainCtx->SymbolFile, FileHeader.SymbolVirtualAddress, SEEK_SET);
    fread(Files, sizeof(SYMBOL), FileHeader.SymbolCount, MainCtx->SymbolFile);
    
    MainCtx->ResolvedSymbols = Files;
    MainCtx->SymbolCount = FileHeader.SymbolCount;
    return;
}

WORD32 SymbolsGetCount(void) {
    return MainCtx->SymbolCount;
}

PSYMBOL SymbolGetById(int Id) {
    return &MainCtx->ResolvedSymbols[Id];
}

PSYMBOL SymbolGetByIp(WORD64 VirtualIp) {
    for (int i = 0; i < MainCtx->SymbolCount; i++) {
        PSYMBOL ThisSymbol = &MainCtx->ResolvedSymbols[i];
        if (ThisSymbol->SymbolType != SYMBOL_TYPE_FUNCTION)
            continue;
        if (InRange(VirtualIp, ThisSymbol->VirtualIp, ThisSymbol->VirtualSize))
            return ThisSymbol;
    }
}

PSYMBOL SymbolGetByArgument(WORD64 VirtualIp) {
    for (int i = 0; i < MainCtx->SymbolCount; i++) {
        PSYMBOL ThisSymbol = &MainCtx->ResolvedSymbols[i];
        if (ThisSymbol->SymbolType != SYMBOL_TYPE_VARIABLE)
            continue;
        if (InRange(VirtualIp, ThisSymbol->AssignedValue, ThisSymbol->VirtualSize))
            return ThisSymbol;
    }
}
