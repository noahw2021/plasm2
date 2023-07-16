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

void link_go(void) {
	for (int s = 0; s < linkctx->SymbolCount; s++) {
		if (!linkctx->Symbols[s].Resolved) {
			char* ErrorMsg = malloc(512);
			sprintf(ErrorMsg, "[E5000]: Unresolved symbol '%s'.", linkctx->Symbols[s].SymbolName);
			cge_error(0xFFFF, ErrorMsg);
			free(ErrorMsg);
		}

		for (int l = 0; l < linkctx->Symbols[s].LocationCount; l++) {
			fseek(PrimaryOutput, linkctx->Symbols[s].Locations[l].Location, SEEK_SET);
			cgp_put8(linkctx->Symbols[s].Resolution);
		}

		free(linkctx->Symbols[s].Locations);
		linkctx->Symbols[s].LocationCount = 0;
	}

	return;
}