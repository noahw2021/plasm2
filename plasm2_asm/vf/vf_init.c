//
//  vf_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "vf.h"
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 6011 6387)

PVF_CTX VfCtx;

void VfInit(FILE* BaseFile) {
	VfCtx = malloc(sizeof(VF_CTX));
	memset(VfCtx, 0, sizeof(VF_CTX));

	VfCtx->FileBase = tmpfile();

	VfCtx->VirtualFile = malloc(sizeof(*VfCtx->VirtualFile));
	if (BaseFile == stdin) {
		VfCtx->VirtualFile[0].IsLiveInput = 1;
		VfCtx->VirtualFile[0].SortOrder = 0;
	} else {
		VfCtx->VirtualFile[0].SortOrder = 1;
	}
	VfCtx->VirtualFile[0].PhysicalFile = BaseFile;
	VfCtx->VirtualFileCount++;
	VfCtx->Started = 1;

	return;
}

void VfShutdown(void) {
	fclose(VfCtx->FileBase);
	for (int i = 0; i < VfCtx->VirtualFileCount; i++) {
		if (VfCtx->VirtualFile[i].PhysicalFile != stdin)
			fclose(VfCtx->VirtualFile[i].PhysicalFile);
	}
	free(VfCtx->VirtualFile);
	free(VfCtx);
	return;
}
