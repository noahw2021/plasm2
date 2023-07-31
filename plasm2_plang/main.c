#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basetypes.h"
#include "parse/parse.h"
#include "error/error.h"
#include "cg/cg.h"
/*
main.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

byte Flags;
FILE* PrimaryInput, *PrimaryOutput;

#pragma warning(disable: 6387)

int main(int argc, char** argv) {
	Flags = 0x00;
	for (int i = 0; i < argv; i++) {
		if (strstr(argv[i], "-f=")) {
			PrimaryInput = fopen(argv[i] + 3, "r");
			if (!PrimaryInput) {
				printf("[ERR]: Failed to open source file: %s\n", argv[i]);
				return -1;
			}
		}
		if (strstr(argv[i], "-o=")) {
			PrimaryOutput = fopen(argv[i] + 3, "wb");
			if (!PrimaryOutput) {
				printf("[ERR]: Could not open output file: %s\n", argv[i]);
			}
		}
		if (strstr(argv[i], "-a"))
			Flags |= FLAG_ASSEMBLY;
		if (strstr(argv[i], "-h")) {
			printf("%s -h: Shows this message.\n", argv[0]);
			printf("%s -f=in.plg -o=out.bin: Compiles a file to machine code.\n", argv[0]);
			printf("%s -f=in.plg -o=out.asm -a: Compiles a file to assembly.\n", argv[0]);
			return 0;
		}
	}

	parse_init();
	error_init();
	cg_init();

	char* Buffer = malloc(4096);
	while (!(Flags & FLAG_COMPLETE)) {
		fgets(Buffer, 4096, PrimaryInput);
		if (PrimaryInput != stdin)
			fputs(Buffer, stdout);

		parse_line(Buffer);
	}
	free(Buffer);

	cg_shutdown();
	error_shutdown();
	parse_shutdown();
}