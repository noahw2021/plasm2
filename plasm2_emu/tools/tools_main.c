#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
/*
tools_main.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6031)

void tools_main(void) {
	printf("PLASM2EMU Tools: \n\n");
	printf("1) HDD Generator\n");
	printf("2) Quit\n");

	char Inputs[2];
	fgets(Inputs, 2, stdin);
	Inputs[0] -= '0';
	
	switch (Inputs[0]) {
	case 1:
		toolsi_hddgen();
		break;
	case 2:
		break;
	default:
		break;
	}

	return;
}