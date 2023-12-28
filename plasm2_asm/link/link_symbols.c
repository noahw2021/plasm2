//
//  link_symbols.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#include "link.h"
#include "../cg/cg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#pragma warning(disable: 6308 26451)

WORD64 LinkGetSymbol(const char* Name, WORD32 Offset) {
	for (int i = 0; i < LinkCtx->SymbolCount; i++) {
        PLINK_CTX_SYMBOL ThisSymbol = &LinkCtx->Symbols[i];
        
		if (!strcmp(ThisSymbol->SymbolName, Name)) {
			if (ThisSymbol->Resolved)
				return ThisSymbol->Resolution;
			
            if (!ThisSymbol->Locations) {
				ThisSymbol->Locations = malloc(
                    sizeof(ThisSymbol->Locations[0]));
			} else {
				ThisSymbol->Locations = 
                    realloc(ThisSymbol->Locations,
                    (sizeof(WORD64) *
                    (ThisSymbol->LocationCount + 1)));
			}

			WORD64 CodePos = CgCtx->DataPosition + Offset;
			WORD32 Loc = ThisSymbol->LocationCount;
			ThisSymbol->Locations[Loc] = CodePos;
			ThisSymbol->LocationCount++;
            
			return ThisSymbol->Resolution;
		}
	}

    if (LinkCtx->Symbols) {
        LinkCtx->Symbols = realloc(LinkCtx->Symbols, 
            sizeof(*LinkCtx->Symbols) *
            (LinkCtx->SymbolCount + 1));
    } else {
        LinkCtx->Symbols = malloc(sizeof(*LinkCtx->Symbols));
    }
    
    PLINK_CTX_SYMBOL ThisSymbol =
        &LinkCtx->Symbols[LinkCtx->SymbolCount];
    LinkCtx->SymbolCount++;
    
	ThisSymbol->LocationCount = 1;
	ThisSymbol->Locations = malloc(sizeof(WORD64));
	ThisSymbol->Locations[0] = CgCtx->DataPosition + Offset;
	ThisSymbol->Resolution = 0;
	ThisSymbol->Resolved = 0;
	ThisSymbol->SymbolName = malloc(strlen(Name) + 1);
	strcpy(ThisSymbol->SymbolName, Name);

	return 0; 
}

void LinkResolve(const char* Name, WORD64 Pointer) {
	for (int i = 0; i < LinkCtx->SymbolCount; i++) {
        PLINK_CTX_SYMBOL ThisSymbol = &LinkCtx->Symbols[i];
        
        if (!strcmp(ThisSymbol->SymbolName, Name)) {
			ThisSymbol->Resolution = Pointer + 
                CgCtx->ReferencePtr;
			ThisSymbol->Resolved = 1;
			return;
		}
	}

    if (!LinkCtx->Symbols) {
        LinkCtx->Symbols = malloc(sizeof(*LinkCtx->Symbols));
    } else {
        LinkCtx->Symbols = realloc(LinkCtx->Symbols, 
            sizeof(*LinkCtx->Symbols) *
            (LinkCtx->SymbolCount + 1));
    }
    
    PLINK_CTX_SYMBOL ThisSymbol =
        &LinkCtx->Symbols[LinkCtx->SymbolCount];
    LinkCtx->SymbolCount++;
    
	ThisSymbol->LocationCount = 0;
	ThisSymbol->Locations = NULL;
	ThisSymbol->Resolution = Pointer + CgCtx->ReferencePtr;
	ThisSymbol->Resolved = 1;
	ThisSymbol->SymbolName = malloc(strlen(Name) + 1);
	strcpy(ThisSymbol->SymbolName, Name);
	return;
}
