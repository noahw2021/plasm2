#include "vf.h"
#include "../cg/cg.h"
/*
vf_g.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void vfg_write(const char* Filename, u64 Dp) {
	FILE* InFile = fopen(Filename, "rb");
	if (!InFile) {
		cge_error(cgctx->CurrentLine, "[E1002]: Invalid file");
		return;
	}

	u64 OldDp = cgctx->DataPosition;
	cgctx->DataPosition = Dp;
	
	fseek(InFile, 0, SEEK_END);
	u64 FileSize = ftell(InFile);
	fseek(InFile, 0, SEEK_SET);

	for (int i = 0; i < (FileSize / 8); i++) {
		u64 Temp;
		fseek(InFile, (i * 8), SEEK_SET);
		fread(&Temp, 8, 1, InFile);
		cgp_put8(Temp);
	}

	for (int i = 0; i < (FileSize % 8); i++) {
		byte Temp;
		fseek(InFile, (FileSize - (FileSize % 8) + i), SEEK_SET);
		fread(&Temp, 1, 1, InFile);
		cgp_put1(Temp);
	}

	fclose(InFile);
	cgctx->DataPosition = OldDp;

	return;
}