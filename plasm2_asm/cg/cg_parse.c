#include "cg.h"
#include "../psin2/psin2.h"
#include <stdlib.h>
#include <string.h>
/*
cg_parse.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6387)

void cg_parse(const char* Line) {
	// MOV r0, r1
	int c = 0;

	char* Temporary = malloc(64);
	memset(Temporary, 0, 64);

	while (Line[c] != ' ')
		Temporary[c] = Line[c++];
	c++;
	Temporary[c] = 0x00;
	int Psin = psin2i_getbyoperandname(Temporary);



	switch (psin2i_getoperandcnt(Psin)) {
	case 0:
		goto DoneWithOperands;
		break;
	case 1:

		break;
	case 2:
		break;
	}

DoneWithOperands:

	free(Temporary);
	return;
}