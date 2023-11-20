#include "vf.h"
#include <stdlib.h>
#include <string.h>
//
//  vf_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
vfctx_t* vfctx;

#pragma warning(disable: 6011 6387)

void vf_init(FILE* BaseFile) {
	vfctx = malloc(sizeof(vfctx_t));
	memset(vfctx, 0, sizeof(vfctx_t));

	vfctx->FileBase = tmpfile();

	vfctx->VirtualFile = malloc(sizeof(*vfctx->VirtualFile));
	if (BaseFile == stdin) {
		vfctx->VirtualFile[0].IsLiveInput = 1;
		vfctx->VirtualFile[0].SortOrder = 0;
	} else {
		vfctx->VirtualFile[0].SortOrder = 1;
	}
	vfctx->VirtualFile[0].PhysicalFile = BaseFile;
	vfctx->VirtualFileCount++;
	vfctx->Started = 1;

	return;
}

void vf_shutdown(void) {
	fclose(vfctx->FileBase);
	for (int i = 0; i < vfctx->VirtualFileCount; i++) {
		if (vfctx->VirtualFile[i].PhysicalFile != stdin)
			fclose(vfctx->VirtualFile[i].PhysicalFile);
	}
	free(vfctx->VirtualFile);
	free(vfctx);
	return;
}
