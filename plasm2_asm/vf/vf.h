#pragma once
#include "../basetypes.h"
#include <stdio.h>
/*
vf.h
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

typedef struct _vfctx {
	byte Started;
	FILE* FileBase;

	int VirtualFileCount;
	int LastFile;
	struct {
		FILE* PhysicalFile;
		int SortOrder;

		byte IsLiveInput;
		byte Done;
	}*VirtualFile;
	int LastSortOrder;
}vfctx_t;
extern vfctx_t* vfctx;

void vf_init(FILE* BaseFile);
void vf_shutdown(void);

void vf_register(const char* Filename);

char* vf_cur(void);
char* vf_get(void);
FILE* vf_ci(void);
void vf_eof(void);

void vfi_sort(void);

void vfg_write(const char* Filename, u64 Dp);