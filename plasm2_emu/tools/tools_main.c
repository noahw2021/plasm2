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
	printf("2) Font Generator\n");
	printf("3) Font Viewr\n");
	printf("4) Quit\n");

	char Inputs[3];
	fgets(Inputs, 3, stdin);
	int Input = atoi(Inputs);
	
	switch (Input) {
	case 1:
		toolsi_hddgen();
		break;
	case 2:
		toolsi_fontgen();
		break;
	case 3:
		toolsi_fontview();
		break;
	case 4:
		break;
	default:
		break;
	}

	return;
}