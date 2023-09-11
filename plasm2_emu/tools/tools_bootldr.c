#include "../basetypes.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
tools_bootldr.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6031 6387)

#define BOOTIMG_MAGIC 0x504C4D4254494D47
typedef struct _bootimg {
	u64 Magic;
	u64 EntryPoint;
	u64 ExpectedMapPoint;
	u64 OsId;

	u64 gFont;
	u64 gRenderChar;
	u64 gRenderStr;
	u64 gHddCount;

	u64 RawDiskPtr;
	u64 RawDiskSize;
	u64 Reserved0[2];

	u64 Reserved1[4];
}bootimg_t;

void toolsi_bootloader(void) {
	char* Strbuf = malloc(240);

	printf("Please enter the path to the input binary: ");
	fgets(Strbuf, 240, stdin);
	
	FILE* _Input = fopen(Strbuf, "rb");
	if (!_Input) {
		printf("Failed to open the input file.\n");
		free(Strbuf);
		return;
	}

	printf("Please enter the path to the output: ");
	fgets(Strbuf, 240, stdin);
	FILE* _Output = fopen(Strbuf, "wb");
	free(Strbuf);
	if (!_Output) {
		fclose(_Input);
		printf("Failed to open the output file.\n");
		return;
	}

	fseek(_Input, 0, SEEK_END);
	u64 FileSize = ftell(_Input);
	fseek(_Input, 0, SEEK_SET);
	void* _Data = malloc(FileSize);
	fread(_Data, FileSize, 1, _Input);
	fclose(_Input);

	bootimg_t* BootImg = malloc(sizeof(bootimg_t));
	memset(BootImg, 0, sizeof(bootimg_t));
	
	BootImg->Magic = BOOTIMG_MAGIC;
	printf("Where is the entry point?: 0x");
	scanf("%llX", &BootImg->EntryPoint);
	printf("Where is the expected map point?: 0x");
	scanf("%llX", &BootImg->ExpectedMapPoint);
	printf("What is the OS ID?: 0x");
	scanf("%llX", &BootImg->OsId);
	printf("Where is the 'gFont' variable?: 0x");
	scanf("%llX", &BootImg->gFont);
	printf("Where is the 'gRenderChar' variable?: 0x");
	scanf("%llX", &BootImg->gRenderChar);
	printf("Where is the 'gRenderStr' variable?: 0x");
	scanf("%llX", &BootImg->gRenderStr);
	printf("Where is the 'gHddCount' variable? 0x");
	scanf("%llX", &BootImg->gHddCount);
	printf("Where does the OS start on the disk?: 0x");
	scanf("%llX", &BootImg->RawDiskPtr);
	printf("How much should the BIOS map?: 0x");
	scanf("%llX", &BootImg->RawDiskSize);

	fseek(_Output, 0, SEEK_SET);
	fwrite(BootImg, sizeof(bootimg_t), 1, _Output);
	fseek(_Output, sizeof(bootimg_t), SEEK_SET);
	fwrite(_Data, FileSize, 1, _Output);
	free(_Data);
	fclose(_Output);

	printf("Success!\n");
	return;
}