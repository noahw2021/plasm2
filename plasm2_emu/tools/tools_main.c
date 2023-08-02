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
	printf("1) HDD Generator");
	printf("2) Quit");

	int Input;
	scanf("%i", &Input);
	
	switch (Input) {
	case 1:
		toolsi_hddgen();
	case 2:
		exit(0);
	default:
		break;
	}
}