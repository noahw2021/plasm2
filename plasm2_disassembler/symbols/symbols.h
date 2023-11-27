//
//  symbols.h
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#ifndef symbols_h
#define symbols_h

#include "../basetypes.h"
#include <stdio.h>

#define SYMBOL_TYPE_FUNCTION 0x01
#define SYMBOL_TYPE_VARIABLE 0x02
#define SYMBOL_TYPE_UNUSED   0x03

typedef struct _SYMBOL {
    char SymbolName[256];
    WORD64 SymbolType;
    WORD64 VirtualIp;
    WORD64 AssignedValue;
    WORD64 VirtualSize;
}SYMBOL, *PSYMBOL;

typedef struct _SYMBOL_FILE {
    WORD64 CompileDate;
    WORD32 SymbolCount;
    WORD32 SymbolVirtualAddress;
}SYMBOL_FILE, *PSYMBOL_FILE;

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
