#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
cg_compile.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

extern FILE* PrimaryInput, * PrimaryOutput;

void cg_compile(void) {
	int ErrorCount = cge_errorcnt();
	for (int i = 0; i < ErrorCount; i++)
		fprintf(stdout, "%s\n", cge_geterr(i));
	fprintf(stdout, "%i errors generated.\n", ErrorCount);

	if (PrimaryOutput != stdout)
		fclose(PrimaryOutput);
	else
		fprintf(PrimaryOutput, "Complation complete, executible size: 0x%08llX", cgctx->DataPosition);
	if (PrimaryInput != stdin)
		fclose(PrimaryInput);


}