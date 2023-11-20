#pragma once
#include "../basetypes.h"
#include <stdio.h>
//
//  vf.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

typedef struct _VF_CTX {
	BYTE Started;
	FILE* FileBase;

	int VirtualFileCount;
	struct {
		FILE* PhysicalFile;
		int SortOrder;

		BYTE IsLiveInput;
	}*VirtualFile;
	int LastSortOrder;
}VF_CTX, *PVF_CTX;
extern PVF_CTX VfCtx;

void VfInit(FILE* BaseFile);
void VfShutdown(void);
void VfRegister(const char* Filename);
char* VfGet(void);
FILE* VfCi(void);
void VfiSort(void);
void VfgWrite(const char* Filename, WORD64 Dp);
