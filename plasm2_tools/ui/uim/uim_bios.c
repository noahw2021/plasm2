#include "../ui.h"
#include "../../basetypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
uim_bios.c
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6031 6387)

void uim_biosgen(void) {
	char* DesiredInput, *DesiredOutput;
	DesiredInput = malloc(1024);
	DesiredOutput = malloc(1024);

	byte FoundInput = 0, FoundOutput = 0;

	if (__argc == 1) { // interactive mode
		printf("What is the desired input file: ");
		scanf("%s", DesiredInput);
		printf("What is the desired output file: ");
		scanf("%s", DesiredOutput);
	} else {
		for (int i = 0; i < __argc; i++) {
			if (strstr(__argv[i], "-di=")) {
				strcpy(DesiredInput, strstr(__argv[i], "-di=") + 4);
				FoundInput = 1;
			}
			if (strstr(__argv[i], "-do=")) {
				strcpy(DesiredInput, strstr(__argv[i], "-do=") + 4);
				FoundOutput = 1;
			}
		}
		if (!FoundInput) {
			printf("Missing desired input!\n");
			free(DesiredInput);
			free(DesiredOutput);
			return;
		}
		if (!FoundOutput) {
			printf("Missing desired output!\n");
			free(DesiredInput);
			free(DesiredOutput);
			return;
		}
	}


}