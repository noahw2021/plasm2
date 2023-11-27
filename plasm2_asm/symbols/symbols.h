//
//  symbols.h
//  plasm2_asm
//
//  Created by Noah Wooten on 11/21/23.
//

#ifndef symbols_h
#define symbols_h

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
extern PSYMBOL_CTX SymbolsCtx;

void SymbolsInit(void);
void SymbolsShutdown(void);
void SymbolsSetFile(FILE* OutFile);
void SymbolsGenerate(void);

void SymbolsCreateFunction(WORD64 VirtualIp, WORD64 VirtualSize, char* Name);
void SymbolsCreateVariable(WORD64 AssignedValue, WORD64 Size, char* Name);
void SymbolsCreateUnused(WORD64 VirtualIp, WORD64 VirtualSize, char* Name);

#endif /* symbols_h */
