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
    WORD32 EmptyStruct;
}SYMBOL, *PSYMBOL;

typedef struct _SYMBOL_CTX {
    FILE* SymbolFile;
    
    PSYMBOL ResolvedSymbols;
    WORD32 SymbolCount;
}SYMBOL_CTX, *PSYMBOL_CTX;

#endif /* symbols_h */
