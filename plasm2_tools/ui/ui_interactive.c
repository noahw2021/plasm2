#include "ui.h"
#include "../basetypes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
ui_interactive.c
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6031)

void ui_interactive(void) {
	int SelectedOption = 0;

	printf("Welcome to plasm2 tools! Please select an option below.\n");

	printf("\t1. BIOS Generator\n");
	printf("\t2. Exit\n");

	printf("\nPlease make your selection: ");
MakeTheSelection:
	scanf("%i", &SelectedOption);

	switch (SelectedOption) {
	case 1: // BIOS generator
	 	uim_biosgen();
		break;
	case 2: // Exit
		exit(0);
		break;
	default:
		printf("Invalid option selected. Try again: ");
		goto MakeTheSelection;
	}


}