//
//  link.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _link_h
#define _link_h

#include "../basetypes.h"

void LinkInit(void);
void LinkShutdown(void);

WORD64 LinkGetSymbol(const char* Name, WORD32 Offset);
void LinkResolve(const char* Name, WORD64 Pointer);
void LinkGo(void);

typedef struct _LINK_CTX_SYMBOL {
    WORD64 Resolution;
    char* SymbolName;
    BYTE Resolved;

    WORD64 LocationCount;
    WORD64* Locations;
}LINK_CTX_SYMBOL, *PLINK_CTX_SYMBOL;

typedef struct _LINK_CTX {
	WORD64 SymbolCount;
    PLINK_CTX_SYMBOL Symbols;
}LINK_CTX, *PLINK_CTX;

extern PLINK_CTX LinkCtx;

#endif
