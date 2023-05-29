#include "tools.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
tls_bios.c
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6386 6387)

int tl_bios_gen(const char* InputFile, const char* OutputFile) {
	FILE* Input = fopen(InputFile, "rb");
	if (!Input) {
		return 1;
	}
	FILE* Output = fopen(OutputFile, "wb");
	if (!Output) {
		fclose(Input);
		return 2;
	}

	fseek(Input, 0, SEEK_END);
	int TotalSize = ftell(Input);
	if (TotalSize > 4096) {
		fclose(Input);
		fclose(Output);
		return 3;
	}

	void* Buffer = malloc(4096);
	memset(Buffer, 0, 4096);

	fseek(Output, 0, SEEK_SET);
	fseek(Input, 0, SEEK_SET);

	fread(Buffer, TotalSize, 1, Input);
	fwrite(Buffer, 4096, 1, Output);

	return 0;
}