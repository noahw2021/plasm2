#include "vf.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
/*
vf_init.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/
vfctx_t* vfctx;

#pragma warning(disable: 6011 6387)

void vf_init(FILE* BaseFile) {
	vfctx = malloc(sizeof(vfctx_t));
	memset(vfctx, 0, sizeof(vfctx_t));

	vfctx->VirtualFile = malloc(sizeof(*vfctx->VirtualFile));
	vfctx->VirtualFile[0].Done = 0;
	vfctx->VirtualFile[0].IsLiveInput = 0;
	vfctx->VirtualFile[0].PhysicalFile = BaseFile;
	vfctx->VirtualFile[0].SortOrder = 0;
	vfctx->VirtualFileCount = 1;

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