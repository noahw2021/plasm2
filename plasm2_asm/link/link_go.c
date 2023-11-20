#include "link.h"
#include "../cg/cg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//
//  link_go.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
extern FILE* PrimaryOutput;
#pragma warning(disable: 6001 6386 6387)

void LinkGo(void) {
	for (int s = 0; s < LinkCtx->SymbolCount; s++) {
		if (!LinkCtx->Symbols[s].Resolved) {
			char* ErrorMsg = malloc(512);
			sprintf(ErrorMsg, "[E5000]: Unresolved symbol '%s'.", LinkCtx->Symbols[s].SymbolName);
			CgeError(0xFFFF, ErrorMsg);
			free(ErrorMsg);
		}

		for (int l = 0; l < LinkCtx->Symbols[s].LocationCount; l++) {
			fseek(PrimaryOutput, (WORD32)LinkCtx->Symbols[s].Locations[l], SEEK_SET);
			// linkctx->Symbols[s].Resolution; // += cgctx->ReferencePtr; WHY??
			fwrite(&LinkCtx->Symbols[s].Resolution, 8, 1, PrimaryOutput);
		}

		if (LinkCtx->Symbols[s].Locations)
			//free(linkctx->Symbols[s].Locations);
		LinkCtx->Symbols[s].LocationCount = 0;
	}

	return;
}
