#include "link.h"
#include "../cg/cg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
link_go.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

extern FILE* PrimaryOutput;
#pragma warning(disable: 6001 6386 6387)

void link_go(void) {
	for (int s = 0; s < linkctx->SymbolCount; s++) {
		if (!linkctx->Symbols[s].Resolved) {
			char* ErrorMsg = malloc(512);
			sprintf(ErrorMsg, "[E5000]: Unresolved symbol '%s'.", linkctx->Symbols[s].SymbolName);
			cge_error(0xFFFF, ErrorMsg);
			free(ErrorMsg);
		}

		for (int l = 0; l < linkctx->Symbols[s].LocationCount; l++) {
			fseek(PrimaryOutput, (u32)linkctx->Symbols[s].Locations[l], SEEK_SET);
			linkctx->Symbols[s].Resolution; // += cgctx->ReferencePtr;
			fwrite(&linkctx->Symbols[s].Resolution, 8, 1, PrimaryOutput);
		}

		if (linkctx->Symbols[s].Locations)
			//free(linkctx->Symbols[s].Locations);
		linkctx->Symbols[s].LocationCount = 0;
	}

	return;
}