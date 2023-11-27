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
		if (!strcmp(LinkCtx->Symbols[i].SymbolName, Name)) {
			if (LinkCtx->Symbols[i].Resolved)
				return LinkCtx->Symbols[i].Resolution;
			if (!LinkCtx->Symbols[i].Locations) {
				LinkCtx->Symbols[i].Locations = malloc(sizeof(LinkCtx->Symbols[i].Locations[0]));
			} else {
				LinkCtx->Symbols[i].Locations = realloc(LinkCtx->Symbols[i].Locations, (sizeof(WORD64) * (LinkCtx->Symbols[i].LocationCount + 1)));
			}

			WORD64 CodePos = CgCtx->DataPosition + Offset;
			int Loc = LinkCtx->Symbols[i].LocationCount;
			LinkCtx->Symbols[i].Locations[Loc] = CodePos;
			LinkCtx->Symbols[i].LocationCount++;
			return LinkCtx->Symbols[i].Resolution;
		}
	}

	if (LinkCtx->Symbols)
		LinkCtx->Symbols = realloc(LinkCtx->Symbols, sizeof(*LinkCtx->Symbols) * (LinkCtx->SymbolCount + 1));
	else
		LinkCtx->Symbols = malloc(sizeof(*LinkCtx->Symbols));

	LinkCtx->Symbols[LinkCtx->SymbolCount].LocationCount = 1;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Locations = malloc(sizeof(WORD64));
	LinkCtx->Symbols[LinkCtx->SymbolCount].Locations[0] = CgCtx->DataPosition + Offset;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Resolution = 0;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Resolved = 0;
	LinkCtx->Symbols[LinkCtx->SymbolCount].SymbolName = malloc(strlen(Name) + 1);
	strcpy(LinkCtx->Symbols[LinkCtx->SymbolCount].SymbolName, Name);
	LinkCtx->SymbolCount++;

	return 0; 
}

void LinkResolve(const char* Name, WORD64 Pointer) {
	for (int i = 0; i < LinkCtx->SymbolCount; i++) {
		if (!strcmp(LinkCtx->Symbols[i].SymbolName, Name)) {
			LinkCtx->Symbols[i].Resolution = Pointer + CgCtx->ReferencePtr;
			LinkCtx->Symbols[i].Resolved = 1;
			return;
		}
	}

	if (!LinkCtx->Symbols)
		LinkCtx->Symbols = malloc(sizeof(*LinkCtx->Symbols));
	else
		LinkCtx->Symbols = realloc(LinkCtx->Symbols, sizeof(*LinkCtx->Symbols) * (LinkCtx->SymbolCount + 1));
	
	LinkCtx->Symbols[LinkCtx->SymbolCount].LocationCount = 0;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Locations = NULL;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Resolution = Pointer + CgCtx->ReferencePtr;
	LinkCtx->Symbols[LinkCtx->SymbolCount].Resolved = 1;
	LinkCtx->Symbols[LinkCtx->SymbolCount].SymbolName = malloc(strlen(Name) + 1);
	strcpy(LinkCtx->Symbols[LinkCtx->SymbolCount].SymbolName, Name);
	LinkCtx->SymbolCount++;

	return;
}
