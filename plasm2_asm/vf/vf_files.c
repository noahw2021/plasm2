#include "vf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
//  vf_files.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#pragma warning(disable: 6385 6387)
#define VF_SIZE(x) (sizeof(*VfCtx->VirtualFile) * x)

void VfRegister(const char* Filename) {
	FILE* RealFile = fopen(Filename, "r");
	if (!RealFile) {
		printf("[ERR]: Failed to open object: %s\n", Filename);
		return;
	}

	VfCtx->VirtualFile = realloc(VfCtx->VirtualFile, VF_SIZE(VfCtx->VirtualFileCount + 1));
	VfCtx->VirtualFile[VfCtx->VirtualFileCount].PhysicalFile = RealFile;
	VfCtx->VirtualFile[VfCtx->VirtualFileCount].IsLiveInput = 0;
	VfCtx->VirtualFile[VfCtx->VirtualFileCount].SortOrder = ++VfCtx->LastSortOrder;
	VfCtx->VirtualFileCount++;

	VfiSort();
	return;
}	

char* VfGet(void) {
	static char Return[512];
	for (int i = 0; i < VfCtx->VirtualFileCount; i++) {
		if (!VfCtx->VirtualFile[i].IsLiveInput) {
			if (!feof(VfCtx->VirtualFile[i].PhysicalFile)) {
				fgets(Return, 512, VfCtx->VirtualFile[i].PhysicalFile);
				if (!strcmp(Return, "-c"))
					return "; -c\n";
				return Return;
			}
		} else {
			fgets(Return, 512, VfCtx->VirtualFile[i].PhysicalFile);
			return Return;
		}
	}
	return NULL;
}

FILE* VfCi(void) {
	return VfCtx->VirtualFile[0].PhysicalFile;
}

void VfiSort(void) {
	void* TempBuffer = malloc(VF_SIZE(1));

	for (int i = 0; i < VfCtx->VirtualFileCount - 1; i++) {
		int Mindex = i;
		for (int c = i + 1; c < VfCtx->VirtualFileCount; c++) {
			if (VfCtx->VirtualFile[c].SortOrder > VfCtx->VirtualFile[Mindex].SortOrder)
				Mindex = c;

			if (Mindex != i) {
				memcpy(TempBuffer, &VfCtx->VirtualFile[Mindex], VF_SIZE(1));
				memcpy(&VfCtx->VirtualFile[Mindex], &VfCtx->VirtualFile[i], VF_SIZE(1));
				memcpy(&VfCtx->VirtualFile[i], TempBuffer, VF_SIZE(1));
			}
		}
	}
	
	free(TempBuffer);
	return;
}
