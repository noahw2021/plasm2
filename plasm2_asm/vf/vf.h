//
//  vf.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#pragma once
#include "../basetypes.h"
#include <stdio.h>

typedef struct _VF_CTX_FILE {
    FILE* PhysicalFile;
    int SortOrder;

    BYTE IsLiveInput;
}VF_CTX_FILE, *PVF_CTX_FILE;

typedef struct _VF_CTX {
	BYTE Started;
	FILE* FileBase;

	int VirtualFileCount;
    PVF_CTX_FILE VirtualFile;
    
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
