#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
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

	char* InputFileName = malloc(240);
	if (!__t_argv) {
		printf(" Output file name: ");

		fgetc(stdin);
		fgets(InputFileName, 240, stdin);
		if (strstr(InputFileName, "\n"))
			strstr(InputFileName, "\n")[0] = 0x00;
	} else {
		for (int i = 0; i < __t_argc; i++) {
			if (strstr(__t_argv[i], "--out="))
				strcpy(InputFileName, strstr(__t_argv[i], "--out=") + 6);
		}
		if (!InputFileName[0]) {
			printf("[E]: Missing argument 'output file name'.\n");
			free(InputFileName);
			return;
		}
	}
	FILE* AttemptedFile = fopen(InputFileName, "wb");
	if (!AttemptedFile) {
		printf("[E]: Failed to open file.\n");
		free(InputFileName);
		return;
	}

	char YNInput = 'N';
	if (!__t_argv) {
		printf(" Would you like to mount a binary file in the disk? [Y/N]: ");
		YNInput = fgetc(stdin);
		fgetc(stdin); // absorb said newline
	} else {
		for (int i = 0; i < __t_argc; i++) {
			if (!strcmp(__t_argv[i], "--mountbin")) {
				YNInput = 'Y';
			}
		}
	}

	u64 WhereToMount = 0xFFFFFFFF, DestinedSize = 0;
	void* AttemptedBuffer = NULL;
	if ((YNInput & ~(0x20)) == 'Y') {
		if (!__t_argv) {
			printf("  Please enter the path: ");
			fgets(InputFileName, 239, stdin);
			if (strstr(InputFileName, "\n"))
				strstr(InputFileName, "\n")[0] = 0x00;
		} else {
			for (int i = 0; i < __t_argc; i++) {
				if (strstr(__t_argv[i], "--mountpath="))
					strcpy(InputFileName, strstr(__t_argv[i], "--mountpath=") + 12);
			}
			if (!InputFileName[0]) {
				printf("[E]: Mount file path required.\n");
				free(InputFileName);
				fclose(AttemptedFile);
				return;
			}
		}
		FILE* AttemptedMount = fopen(InputFileName, "rb");
		if (!AttemptedMount) {
			printf("  Failed to open the file.\n");
			free(InputFileName);
			fclose(AttemptedFile);
			return;
		}

	GoBackHere:
		if (!__t_argv) {
			printf("  Where would you like to mount this file?: 0x");
			scanf("%llX ", &WhereToMount);
			if (WhereToMount == 0xFFFFFFFF) {
				printf("  Please choose a different mounting point.\n"); // this is dumb but whatever
				goto GoBackHere;
			}
		}
		else {
			for (int i = 0; i < __t_argc; i++) {
				if (strstr(__t_argv[i], "--mountpoint="))
					sscanf(strstr(__t_argv[i], "--mountpoint=5") + 15, "%llX", &WhereToMount);
			}
			if (WhereToMount == 0xFFFFFFFF) {
				printf("[E]: Mounting point required.\n");
				free(InputFileName);
				fclose(AttemptedFile);
				return;
			}
		}

		fseek(AttemptedMount, 0, SEEK_END);
		DestinedSize = ftell(AttemptedMount);
		AttemptedBuffer = malloc(DestinedSize);

		fseek(AttemptedMount, 0, SEEK_SET);
		fread(AttemptedBuffer, DestinedSize, 1, AttemptedMount);

		fclose(AttemptedMount);
	}

	u64 WantedSize = 0x00000000;
	char* TempBfr = malloc(32);
SizeTryAgain:
	if (!__t_argv) {
		printf(" How large would you like the drive in bytes?: ");
		fgets(TempBfr, 31, stdin);
		WantedSize = strtoull(TempBfr, NULL, 10);
		if (WhereToMount != 0xFFFFFFFF) {
			if (WantedSize < (WhereToMount + DestinedSize)) {
				printf(" Invalid drive size. Drive must be able to at least the given map.\n");
				goto SizeTryAgain;
			}
		}
	} else {
		for (int i = 0; i < __t_argc; i++) {
			if (strstr(__t_argv[i], "--size="))
				sscanf(strstr(__t_argv[i], "--size=") + 7, "%llu", &WantedSize);
		}
		if (!WantedSize) {
			printf("[E]: Drive size required.\n");
			return;
		}
	}
	free(TempBfr);

	u64 LBreak = 0x00;
	if (WantedSize == DestinedSize) {
		LBreak = WantedSize;
	} else {
		if (WhereToMount != 0xFFFFFFFF) {
			byte* Zeros = malloc(1024);
			memset(Zeros, 0, 1024);
			for (int i = 0; i < (WantedSize / 1024); i++) {
				fseek(AttemptedFile, (i * 1024) + sizeof(fdiskhdr_t), SEEK_SET);
				fwrite(Zeros, 1024, 1, AttemptedFile);
				if ((i * 1024) > WhereToMount) {
					LBreak = (i * 1024);
					break;
				}
			}

			if (!LBreak) {
				Zeros = realloc(Zeros, WantedSize % 1024);
				fseek(AttemptedFile, (long)(sizeof(fdiskhdr_t) + (WantedSize / 1024)), SEEK_SET);
				if (Zeros) {
					fwrite(Zeros, WantedSize % 1024, 1, AttemptedFile);
					free(Zeros);
				}
			} // shouldnt ever happen

			fseek(AttemptedFile, (long)WhereToMount + sizeof(fdiskhdr_t), SEEK_SET);
			fwrite(AttemptedBuffer, DestinedSize, 1, AttemptedFile);
			free(AttemptedBuffer);
		}
	}


	free(InputFileName);
	
	fdiskhdr_t* FdHdr = malloc(sizeof(fdiskhdr_t));
	memset(FdHdr, 0, sizeof(fdiskhdr_t));
	
	FdHdr->Magic = FDISKHDR_MAGIC;
	srand((u32)time(NULL));
	FdHdr->DeviceSerial = rand();
	FdHdr->DeviceVendorId = 0x4273;
	strcpy(FdHdr->DeviceVendor, "PLASM2 HDDGnr8r");
	FdHdr->PartsSum = FdHdr->DeviceSerial + FdHdr->DeviceVendorId;
	for (int i = 0; i < 16; i++)
		FdHdr->PartsSum += FdHdr->DeviceVendor[i];
	FdHdr->DriveVirtualSize = WantedSize;
	if (LBreak)
		FdHdr->ExpectedPhysicalSize = LBreak + sizeof(fdiskhdr_t);
	else
		FdHdr->ExpectedPhysicalSize = sizeof(fdiskhdr_t);

	fseek(AttemptedFile, 0, SEEK_SET);
	fwrite(FdHdr, sizeof(fdiskhdr_t), 1, AttemptedFile);
	fclose(AttemptedFile);
	free(FdHdr);

	printf(" Successfully wrote HDD file.\n");
	return;
}