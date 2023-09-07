#include "vf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
vf_files.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6385 6387)
#define VF_SIZE(x) (sizeof(*vfctx->VirtualFile) * x)

void vf_register(const char* Filename) {
	FILE* RealFile = fopen(Filename, "r");
	if (!RealFile) {
		printf("[ERR]: Failed to open object: %s\n", Filename);
		return;
	}

	vfctx->VirtualFile = realloc(vfctx->VirtualFile, VF_SIZE(vfctx->VirtualFileCount + 1));
	vfctx->VirtualFile[vfctx->VirtualFileCount].PhysicalFile = RealFile;
	vfctx->VirtualFile[vfctx->VirtualFileCount].IsLiveInput = 0;
	vfctx->VirtualFile[vfctx->VirtualFileCount].SortOrder = ++vfctx->LastSortOrder;
	vfctx->VirtualFileCount++;

	vfi_sort();
	return;
}	

char* vf_get(void) {
	static int Recursion = 0;
	char* Return = malloc(512);
	for (int i = 0; i < vfctx->VirtualFileCount; i++) {
		if (vfctx->VirtualFile[i].Done)
			continue;
		if (!vfctx->VirtualFile[i].IsLiveInput) {
			vfctx->LastFile = i;
			fgets(Return, 512, vfctx->VirtualFile[i].PhysicalFile);
			if (!strcmp(Return, "-c"))
				return "; -c\n";
			return Return;
		} else {
			vfctx->LastFile = i;
			fgets(Return, 512, vfctx->VirtualFile[i].PhysicalFile);
			return Return;
		}
	}
	free(Return);
	return NULL;
}

FILE* vf_ci(void) {
	if (vfctx->VirtualFile[0].PhysicalFile == NULL)
		vfi_sort();
	return vfctx->VirtualFile[0].PhysicalFile;
}

void vfi_sort(void) {
	byte TempBuffer[VF_SIZE(1)];

	for (int i = 0; i < vfctx->VirtualFileCount - 1; i++) {
		int Mindex = i;
		for (int c = i + 1; c < vfctx->VirtualFileCount; c++) {
			if (vfctx->VirtualFile[c].SortOrder > vfctx->VirtualFile[Mindex].SortOrder)
				Mindex = c;

			if (Mindex != i) {
				memcpy(TempBuffer, &vfctx->VirtualFile[Mindex], VF_SIZE(1));
				memcpy(&vfctx->VirtualFile[Mindex], &vfctx->VirtualFile[i], VF_SIZE(1));
				memcpy(&vfctx->VirtualFile[i], TempBuffer, VF_SIZE(1));
			}
		}
	}
	
	return;
}

char* vf_cur(void) {
	return NULL;
}

void vf_eof(void) {
	vfctx->VirtualFile[vfctx->LastFile].Done = 1;
}