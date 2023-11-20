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

void cgp_put1(byte Data) {
	fseek(PrimaryOutput, (u32)cgctx->DataPosition, SEEK_SET);
	fwrite(&Data, 1, 1, PrimaryOutput);
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
