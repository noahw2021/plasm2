//
//  symbols_io.c
//  plasm2_asm
//
//  Created by Noah Wooten on 11/21/23.
//

#include <string.h>
#include <stdlib.h>
#include "../basetypes.h"
#include "symbols.h"

void SymbolsCreateFunction(WORD64 VirtualIp, WORD64 VirtualSize, char* Name) {
    if (!SymbolsCtx)
        return;
    
    if (!SymbolsCtx->ResolvedSymbols) {
        SymbolsCtx->ResolvedSymbols = malloc(sizeof(SYMBOL));
    } else {
        SymbolsCtx->ResolvedSymbols = realloc(SymbolsCtx->ResolvedSymbols,
            sizeof(SYMBOL) * (SymbolsCtx->SymbolCount + 1));
    }
    
    PSYMBOL NewSym = &SymbolsCtx->ResolvedSymbols[SymbolsCtx->SymbolCount++];
    strcpy(NewSym->SymbolName, Name);
    NewSym->SymbolType = SYMBOL_TYPE_FUNCTION;
    NewSym->VirtualIp = VirtualIp;
    NewSym->VirtualSize = VirtualSize;
    return;
}

void SymbolsCreateVariable(WORD64 AssignedValue, WORD64 Size, char* Name) {
    if (!SymbolsCtx)
        return;
    
    if (!SymbolsCtx->ResolvedSymbols) {
        SymbolsCtx->ResolvedSymbols = malloc(sizeof(SYMBOL));
    } else {
        SymbolsCtx->ResolvedSymbols = realloc(SymbolsCtx->ResolvedSymbols,
            sizeof(SYMBOL) * (SymbolsCtx->SymbolCount + 1));
    }
    
    PSYMBOL NewSym = &SymbolsCtx->ResolvedSymbols[SymbolsCtx->SymbolCount++];
    strcpy(NewSym->SymbolName, Name);
    NewSym->SymbolType = SYMBOL_TYPE_VARIABLE;
    NewSym->AssignedValue = AssignedValue;
    NewSym->VirtualSize = Size;
    return;
}

void SymbolsCreateUnused(WORD64 AssignedValue, WORD64 Size, char* Name) {
    if (!SymbolsCtx)
        return;
    
    if (!SymbolsCtx->ResolvedSymbols) {
        SymbolsCtx->ResolvedSymbols = malloc(sizeof(SYMBOL));
    } else {
        SymbolsCtx->ResolvedSymbols = realloc(SymbolsCtx->ResolvedSymbols,
            sizeof(SYMBOL) * (SymbolsCtx->SymbolCount + 1));
    }
    
    PSYMBOL NewSym = &SymbolsCtx->ResolvedSymbols[SymbolsCtx->SymbolCount++];
    strcpy(NewSym->SymbolName, Name);
    NewSym->SymbolType = SYMBOL_TYPE_UNUSED;
    NewSym->AssignedValue = AssignedValue;
    NewSym->VirtualIp = AssignedValue;
    NewSym->VirtualSize = Size;
    return;
}

