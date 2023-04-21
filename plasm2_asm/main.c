#include <stdio.h>
#include <stdlib.h>
#include "basetypes.h"
#include "psin2/psin2.h"
/*
main.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

int main(void) {
	psin2_init();
	psin2_load();

	for (int i = 0; i < psin2_getcnt(); i++) {
		printf("Instruction:            %s\n", psin2i_getname(i));
		printf("Description:            %s\n", psin2i_description(i));
		printf("Operand Count:          %i\n", psin2i_getoperandcnt(i));
		for (int o = 0; o < psin2i_getoperandcnt(i); o++) {
			printf("\tOperand %i Name:       %s\n", o + 1, psin2i_getoperandname(i, o));
			printf("\tOperand %i Phys Size:  %i\n", o + 1, psin2i_getphyssize(i, o));
			printf("\tOperand %i Avail Size: %i\n", o + 1, psin2i_getavailsize(i, o));
			printf("\tOperand %i Type:       %s\n", o + 1, (psin2i_getoperandtype(i, o) ? "Register" : "Immediate"));
		}
		printf("Total Instruction Size: %i\n\n", psin2i_totalsize(i));
	}

	return 0;
}