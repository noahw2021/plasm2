#include "vf.h"
#include "../cg/cg.h"
//
//  vf_g.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

void VfgWrite(const char* Filename, WORD64 Dp) {
	FILE* InFile = fopen(Filename, "rb");
	if (!InFile) {
		CgeError(CgCtx->CurrentLine, "[E1002]: Invalid file");
		return;
	}

	WORD64 OldDp = CgCtx->DataPosition;
	CgCtx->DataPosition = Dp;
	
	fseek(InFile, 0, SEEK_END);
	WORD64 FileSize = ftell(InFile);
	fseek(InFile, 0, SEEK_SET);

	for (int i = 0; i < (FileSize / 8); i++) {
		WORD64 Temp;
		fseek(InFile, (i * 8), SEEK_SET);
		fread(&Temp, 8, 1, InFile);
		CgpPut8(Temp);
	}

	for (int i = 0; i < (FileSize % 8); i++) {
		BYTE Temp;
		fseek(InFile, (WORD32)(FileSize - (FileSize % 8) + i), 
            SEEK_SET);
		fread(&Temp, 1, 1, InFile);
		CgpPut1(Temp);
	}

	fclose(InFile);
	CgCtx->DataPosition = OldDp + FileSize;

	return;
}
