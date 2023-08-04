#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../basetypes.h"
#include "../devices/fdisk/fdisk.h"
/*
tools_hddgen.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6001 6011 6031 6308 6387 26451 28183)

void toolsi_hddgen(void) {
	printf("HDD File Generator: \n");
	printf("\tOutput file name: ");

	char* InputFileName = malloc(240);
	scanf("%s", InputFileName);

	FILE* AttemptedFile = fopen(InputFileName, "wb");
	if (!AttemptedFile) {
		printf("Failed to open file.\n");
		free(InputFileName);
		return;
	}

	char YNInput;
	printf("\tWould you like to mount a binary file in the disk? [Y/N]: ");
	scanf("%c", &YNInput);

	u32 WhereToMount = 0xFFFFFFFF, DestinedSize = 0;
	void* AttemptedBuffer = NULL;
	if ((YNInput & ~(0x20)) == 'Y') {
		printf("Please enter the path: ");
		scanf("%s", InputFileName);

		FILE* AttemptedMount = fopen(InputFileName, "rb");
		if (!AttemptedMount) {
			printf("Failed to open the file.\n");
			free(InputFileName);
			fclose(AttemptedFile);
			return;
		}

	GoBackHere:
		printf("Where would you like to mount this file?: 0x");
		scanf("%ulX", &WhereToMount);
		if (WhereToMount == 0xFFFFFFFF) {
			printf("Please choose a different mounting point.\n"); // this is dumb but whatever
			goto GoBackHere;
		}

		fseek(AttemptedMount, 0, SEEK_END);
		DestinedSize = ftell(AttemptedMount);
		AttemptedBuffer = malloc(DestinedSize);

		fseek(AttemptedMount, 0, SEEK_SET);
		fread(AttemptedBuffer, DestinedSize, 1, AttemptedMount);

		fclose(AttemptedMount);
	}

	u64 WantedSize;
SizeTryAgain:
	printf("How large would you like the drive in bytes?: ");
	scanf("%llu", &WantedSize);
	if (WhereToMount != 0xFFFFFFFF) {
		if (WantedSize < (WhereToMount + DestinedSize)) {
			printf("Invalid drive size. Drive must be able to at least the given map.\n");
			goto SizeTryAgain;
		}
	}

	byte* Zeros = malloc(1024);
	memset(Zeros, 0, 1024);
	for (int i = 0; i < (WantedSize / 1024); i++) {
		fseek(AttemptedFile, (i * 1024) + sizeof(fdiskhdr_t), SEEK_SET);
		fwrite(Zeros, 1024, 1, AttemptedFile);
	}

	Zeros = realloc(Zeros, WantedSize % 1024);
	fseek(AttemptedFile, (WantedSize / 1024), SEEK_SET);
	fwrite(Zeros, WantedSize % 1024, 1, AttemptedFile);
	free(Zeros);

	if (WhereToMount != 0xFFFFFFFF) {
		fseek(AttemptedFile, WhereToMount + sizeof(fdiskhdr_t), SEEK_SET);
		fwrite(AttemptedBuffer, DestinedSize, 1, AttemptedFile);
		free(AttemptedBuffer);
	}

	free(InputFileName);

	printf("Successfully wrote HDD file.\n");
	return;
}