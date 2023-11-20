//
//  cg_put.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern FILE* PrimaryOutput;

void CgpPut1(BYTE Data) {
	fseek(PrimaryOutput, (WORD32)CgCtx->DataPosition, SEEK_SET);
	fwrite(&Data, 1, 1, PrimaryOutput);
	CgCtx->DataPosition++;
	if (CgCtx->DataPosition > CgCtx->HighestPosition)
		CgCtx->HighestPosition = CgCtx->DataPosition;
	return;
}
void CgpPut8(WORD64 Data) {
	CgpPutX(Data, 8);
}
void CgpPutX(WORD64 Data, BYTE ByteSize) {
	union {
		BYTE Bytes[8];
		WORD64 Data;
	}Union;
	Union.Data = Data;
	for (int i = 0; i < ByteSize; i++)
		CgpPut1(Union.Bytes[i]);
}
