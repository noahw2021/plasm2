//
//  symbols.h
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#ifndef symbols_h
#define symbols_h

#include <stdio.h>

typedef struct _SYMBOL {
    char SymbolName[256];
    WORD64 SymbolType;
    WORD64 VirtualIp;
    WORD64 AssignedValue;
}SYMBOL, *PSYMBOL;

typedef struct _SYMBOL_CTX {
    FILE* SymbolFile;
    
    PSYMBOL ResolvedSymbols;
    WORD32 SymbolCount;
}SYMBOL_CTX, *PSYMBOL_CTX;
extern PSYMBOL_CTX MainCtx;

void SymbolsInit(void);
void SymbolsShutdown(void);
void SymbolsResolve(void);

WORD32 SymbolsGetCount(void);
PSYMBOL SymbolGetById(int Id);
PSYMBOL SymbolGetByIp(WORD64 VirtualIp);
PSYMBOL SymbolGetByArgument(WORD64 VirtualIp);

#endif /* symbols_h */
