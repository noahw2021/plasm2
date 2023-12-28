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
			sprintf(ErrorMsg, "[E5000]: Unresolved symbol '%s'.", 
                LinkCtx->Symbols[s].SymbolName);
			CgeError(0xFFFF, ErrorMsg);
			free(ErrorMsg);
		}

		for (int l = 0; l < LinkCtx->Symbols[s].LocationCount; l++) {
            PLINK_CTX_SYMBOL ThisSymbol = &LinkCtx->Symbols[s];
            
            fseek(PrimaryOutput, (WORD32)ThisSymbol->Locations[l], SEEK_SET);
			fwrite(&ThisSymbol->Resolution, 8, 1, PrimaryOutput);
		}

        // @TODO (fix this ?? -nw 12/28/23)
		if (LinkCtx->Symbols[s].Locations)
			//free(linkctx->Symbols[s].Locations);
		LinkCtx->Symbols[s].LocationCount = 0;
	}

	return;
}
