#include "cg.h"
#include "../psin2/psin2.h"
#include "../link/link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
cg_put.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

extern FILE* PrimaryOutput;

void cgp_put1(byte Data) {
	fseek(PrimaryOutput, cgctx->DataPosition, SEEK_SET);
	fputc(Data, PrimaryOutput);
	cgctx->DataPosition++;
	if (cgctx->DataPosition > cgctx->HighestPosition)
		cgctx->HighestPosition = cgctx->DataPosition;
	return;
}
void cgp_put8(u64 Data) {
	cgp_putx(Data, 8);
}
void cgp_putx(u64 Data, byte ByteSize) {
	union {
		byte Bytes[8];
		u64 Data;
	}Union;
	Union.Data = Data;
	for (int i = 0; i < ByteSize; i++)
		cgp_put1(Union.Bytes[i]);
}