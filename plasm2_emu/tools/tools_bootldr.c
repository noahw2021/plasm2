//
//  tools_bootldr.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//


#include "../basetypes.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 6011 6031 6387)

#define BOOTIMG_MAGIC 0x504C4D4254494D47
typedef struct _BOOTIMG {
	WORD64 Magic;
	WORD64 EntryPoint;
	WORD64 ExpectedMapPoint;
	WORD64 OsId;

	WORD64 gFont;
	WORD64 gRenderChar;
	WORD64 gRenderStr;
	WORD64 gHddCount;

	WORD64 RawDiskPtr;
	WORD64 RawDiskSize;
	WORD64 Reserved0[2];

	WORD64 Reserved1[4];
}BOOT_IMG, *PBOOT_IMG;

#define BOOTLDRFLAG_SAVE 0x01
#define BOOTLDRFLAG_LOAD 0x02

extern int __t_argc;
extern char** __t_argv;

void ToolsiBootloader(void) {
	char* Strbuf = malloc(240);

	if (!__t_argv) {
		printf("Please enter the path to the input binary: ");
		fgets(Strbuf, 240, stdin);
	} else {
		for (int i = 0; i < __t_argc; i++) {
			if (strstr(__t_argv[i], "--data="))
				strcpy(Strbuf, strstr(__t_argv[i], "--data=") + 7);
		}
		if (!Strbuf[0]) {
			printf("[E]: Input file name required.\n");
			free(Strbuf);
			return;
		}
	}

	FILE* _Input = fopen(Strbuf, "rb");
	if (!_Input) {
		printf("Failed to open the input file.\n");
		free(Strbuf);
		return;
	}

	memset(Strbuf, 0, 240);

	if (!__t_argv) {
		printf("Please enter the path to the output: ");
		fgets(Strbuf, 240, stdin);
	} else {
		for (int i = 0; i < __t_argc; i++) {
			if (strstr(__t_argv[i], "--out="))
				strcpy(Strbuf, strstr(__t_argv[i], "--out=") + 7);
		}
		if (!Strbuf[0]) {
			printf("[E]: Output file name required.\n");
			free(Strbuf);
			fclose(_Input);
			return;
		}
	}

	FILE* _Output = fopen(Strbuf, "wb");
	free(Strbuf);
	if (!_Output) {
		fclose(_Input);
		printf("Failed to open the output file.\n");
		return;
	}

	fseek(_Input, 0, SEEK_END);
	WORD64 FileSize = ftell(_Input);
	fseek(_Input, 0, SEEK_SET);
	void* _Data = malloc(FileSize);
	fread(_Data, FileSize, 1, _Input);
	fclose(_Input);

	PBOOT_IMG BootImg = malloc(sizeof(BOOT_IMG));
	memset(BootImg, 0, sizeof(BOOT_IMG));
	
	BYTE Flags = 0x00;
	if (__t_argv) {
		for (int i = 0; i < __t_argc; i++) {
			if (!strcmp(__t_argv[i], "--savehdr"))
				Flags |= BOOTLDRFLAG_SAVE;
			if (!strcmp(__t_argv[i], "--loadhdr"))
				Flags |= BOOTLDRFLAG_LOAD;
		}
	}

	if (Flags & BOOTLDRFLAG_LOAD) {
		FILE* __Input = fopen("boothdr.bin", "rb");
		if (!__Input) {
			printf("[E]: Failed to open boot header.\n");
			free(_Data);
			fclose(_Output);
			return;
		}

		fread(BootImg, sizeof(BOOT_IMG), 1, __Input);
		fclose(__Input);
	}
	else {
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
	
		if (Flags & BOOTLDRFLAG_SAVE) {
			FILE* __Output = fopen("boothdr.bin", "wb");
			if (!__Output) {
				printf("[E]: Failed to open output file.\n");
				free(BootImg);
				free(_Data);
				fclose(_Output);
				return;
			}

			fwrite(BootImg, sizeof(BOOT_IMG), 1, __Output);
			fclose(__Output);
		}
	}
	fseek(_Output, 0, SEEK_SET);
	fwrite(BootImg, sizeof(BOOT_IMG), 1, _Output);
	fseek(_Output, sizeof(BOOT_IMG), SEEK_SET);
	fwrite(_Data, FileSize, 1, _Output);
	free(_Data);
	fclose(_Output);

	printf("Success!\n");
	return;
}
