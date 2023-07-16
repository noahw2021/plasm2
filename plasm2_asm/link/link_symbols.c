#include "link.h"
#include "../cg/cg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
link_symbols.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6308 26451)

u64 link_getsymbol(const char* Name) {
	for (int i = 0; i < linkctx->SymbolCount; i++) {
		if (!strcmp(linkctx->Symbols[i].SymbolName, Name)) {
			linkctx->Symbols[i].Locations = realloc(linkctx->Symbols[i].Locations, sizeof(*linkctx->Symbols[i].Locations) * (linkctx->Symbols[i].LocationCount + 1));
			linkctx->Symbols[i].Locations[linkctx->Symbols[i].LocationCount].Location = cgctx->DataPosition;
			linkctx->Symbols[i].LocationCount++;
			return linkctx->Symbols[i].Resolution;
		}
	}

	if (linkctx->Symbols)
		linkctx->Symbols = realloc(linkctx->Symbols, sizeof(*linkctx->Symbols) * (linkctx->SymbolCount + 1));
	else
		linkctx->Symbols = malloc(sizeof(*linkctx->Symbols));

	linkctx->Symbols[linkctx->SymbolCount].LocationCount = 1;
	linkctx->Symbols[linkctx->SymbolCount].Locations = malloc(sizeof(*linkctx->Symbols[0].Locations));
	linkctx->Symbols[linkctx->SymbolCount].Locations[0].Location = cgctx->DataPosition;
	linkctx->Symbols[linkctx->SymbolCount].Resolution = 0;
	linkctx->Symbols[linkctx->SymbolCount].Resolved = 0;
	linkctx->Symbols[linkctx->SymbolCount].SymbolName = malloc(strlen(Name) + 1);
	strcpy(linkctx->Symbols[linkctx->SymbolCount].SymbolName, Name);
	linkctx->SymbolCount++;

	return 0; 
}

void link_resolve(const char* Name, u64 Pointer) {
	for (int i = 0; i < linkctx->SymbolCount; i++) {
		if (!strcmp(linkctx->Symbols[i].SymbolName, Name)) {
			linkctx->Symbols[i].Resolution = Pointer;
			linkctx->Symbols[i].Resolved = 1;
			return;
		}
	}

	linkctx->Symbols = realloc(linkctx->Symbols, sizeof(*linkctx->Symbols) * (linkctx->SymbolCount + 1));
	linkctx->Symbols[linkctx->SymbolCount].LocationCount = 0;
	linkctx->Symbols[linkctx->SymbolCount].Locations = NULL;
	linkctx->Symbols[linkctx->SymbolCount].Resolution = Pointer;
	linkctx->Symbols[linkctx->SymbolCount].Resolved = 1;
	linkctx->Symbols[linkctx->SymbolCount].SymbolName = malloc(strlen(Name) + 1);
	strcpy(linkctx->Symbols[linkctx->SymbolCount].SymbolName, Name);
	linkctx->SymbolCount++;

	return;
}