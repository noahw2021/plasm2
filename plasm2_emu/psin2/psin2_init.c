//
//  psin2_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "psin2.h"
#include <stdlib.h>
#include <string.h>

PPSIN2_CTX Psin2Ctx;

#pragma warning(disable: 6001 6387)

void Psin2Init(void) {
	Psin2Ctx = malloc(sizeof(PSIN2_CTX));
	memset(Psin2Ctx, 0, sizeof(PSIN2_CTX));

	Psin2Load();

	return;
}

void Psin2Shutdown(void) {
	for (int i = 0; i < Psin2Ctx->InstructionCount; i++) {
		if (Psin2Ctx->Instructions[i].InstructionDescription)
			free(Psin2Ctx->Instructions[i].InstructionDescription);
	}
	free(Psin2Ctx->Instructions);
	free(Psin2Ctx);

	return;
}
