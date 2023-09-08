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

#pragma warning(disable: 6308 6385 6387 28182 26451)
#define VF_SIZE(x) (sizeof(*vfctx->VirtualFile) * x)

void vf_register(const char* Filename) {
	FILE* RealFile = fopen(Filename, "r");
	if (!RealFile) {
		printf("[ERR]: Failed to open object: %s\n", Filename);
		return;
	}

	vfctx->VirtualFile = realloc(vfctx->VirtualFile, (sizeof(vfctx->VirtualFile[0]) * (vfctx->VirtualFileCount + 1)));
	memset(&vfctx->VirtualFile[vfctx->VirtualFileCount], 0, sizeof(vfctx->VirtualFile[0]));

	vfctx->VirtualFile[vfctx->VirtualFileCount].PhysicalFile = RealFile;
	vfctx->VirtualFileCount++;

	return;
}	

char* vf_get(void) {
	char* Return = malloc(512);
	for (int i = 0; i < vfctx->VirtualFileCount; i++) {
		if (vfctx->VirtualFile[i].Done)
			continue;
		fgets(Return, 512, vfctx->VirtualFile[i].PhysicalFile);
		if (feof(vfctx->VirtualFile[i].PhysicalFile))
			vfctx->VirtualFile[i].Done = 1;
		return Return;
	}
	return NULL;
}

FILE* vf_ci(void) {
	for (int i = 0; i < vfctx->VirtualFileCount; i++) {
		if (!vfctx->VirtualFile[i].Done)
			return vfctx->VirtualFile[i].PhysicalFile;
	}
}

void vfi_sort(void) {
	return;
}

char* vf_cur(void) {
	return NULL;
}

void vf_eof(void) {
	for (int i = 0; i < vfctx->VirtualFileCount; i++) {
		if (vfctx->VirtualFile[i].Done)
			continue;
		vfctx->VirtualFile[i].Done = 1;
		return;
	}
}