//
//  cg_compile.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern FILE* PrimaryInput, * PrimaryOutput;

void CgCompile(void) {
	fprintf(stdout, "\nBeginning Compliation...\n\n");

	LinkGo();

	int ErrorCount = cge_errorcnt();
	for (int i = 0; i < ErrorCount; i++)
		fprintf(stdout, "%s\n", cge_geterr(i));
	fprintf(stdout, "%i errors generated.\n", ErrorCount);

	fclose(PrimaryOutput);
	fprintf(stdout, "Complation complete, executible size: 0x%08llX\n", CgCtx->DataPosition);

	if (PrimaryInput != stdin)
		fclose(PrimaryInput);

	PrimaryInput = NULL;

	CgShutdown();
	Psin2Shutdown();

	return;
}
