#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basetypes.h"
#include "psin2/psin2.h"
#include "cg/cg.h"
#include "link/link.h"
/*
main.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

FILE* PrimaryInput;
FILE* PrimaryOutput;

#pragma warning(disable: 6387)

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char** argv) {
#ifdef _WIN32
	if (!IsDebuggerPresent())
		fgetc(stdin);
#endif
	cg_init(); 
	psin2_init();
	link_init();

	psin2_load();

	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-h")) {
			printf("%s -h: Shows this message.\n", argv[0]);
			printf("%s -v: Displays version.\n", argv[0]);
			printf("\n");
			printf("%s -f=input: Assembles an input file.\n", argv[0]);
			printf("%s -f=input -o=output: Inputs and outputs.\n", argv[0]);
			printf("%s -o=output: Output to file in interactive mode.\n", argv[0]);
			return 0;
		}
		if (!strcmp(argv[i], "-v")) {
			printf("plasm2_asm v0.1 beta\n(c) Noah Wooten 2023, All Rights Reserved.\n");
			return 0;
		}
		if (strstr(argv[i], "-f=")) {
			char* FilePtr = strstr(argv[i], "-f=");
			if (!FilePtr) {
				printf("[ERR]: Missing file path.\n");
				return -1;
			}
			PrimaryInput = fopen(FilePtr + 3, "r");
			if (!PrimaryInput) {
				printf("[ERR]: Failed to open file %s.\n", FilePtr + 3);
				return -1;
			}
		}
		if (strstr(argv[i], "-o=")) {
			char* OutFilePtr = strstr(argv[i], "-o=");
			if (!OutFilePtr) {
				printf("[ERR]: Missing file path.\n");
				return -1;
			}
			PrimaryOutput = fopen(OutFilePtr + 3, "w");
			if (!PrimaryOutput) {
				printf("[ERR]: Failed top open file %s for write.\n", OutFilePtr + 3);
				if (PrimaryInput != stdin && PrimaryInput)
					fclose(PrimaryInput);
				return -1;
			}
		}
	}

	if (!PrimaryOutput) {
		PrimaryOutput = fopen("assemout.bin", "wb");
		if (!PrimaryOutput) {
			printf("[ERR]: Failed to open 'assemout.bin' for writing. Please enable access, or specify an output file.\n");
			if (PrimaryInput != stdin && PrimaryInput)
				fclose(PrimaryInput);
			return -1;
		}
	}

	if (!PrimaryInput)
		PrimaryInput = stdin;

	char* Line = malloc(2048);
	while (cgctx) {
		printf("[%04hX]: ", cgctx->DataPosition);
		fgets(Line, 2048, PrimaryInput);
		cg_parse(Line);
	}

	return 0;
}