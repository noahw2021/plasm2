#include "psin2.h"
#include <stdlib.h>
#include <string.h>
/*
psin2_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

psin2ctx_t* psin2ctx;

#pragma warning(disable: 6001 6387)

void psin2_init(void) {
	psin2ctx = malloc(sizeof(psin2ctx_t));
	memset(psin2ctx, 0, sizeof(psin2ctx_t));

	psin2_load();

	return;
}

void psin2_shutdown(void) {
	for (int i = 0; i < psin2ctx->InstructionCount; i++) {
		if (psin2ctx->Instructions[i].InstructionDescription)
			free(psin2ctx->Instructions[i].InstructionDescription);
	}
	free(psin2ctx->Instructions);
	free(psin2ctx);

	return;
}