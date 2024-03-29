//
//  tools_hddgen.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../basetypes.h"
#include "../devices/fdisk/fdisk.h"

#pragma warning(disable: 6001 6011 6031 6308 6387 26451 28183)

void ToolsiHddGen(void) {
	printf("HDD File Generator: \n");
	printf(" Output file name: ");
	
	fgetc(stdin);
	char* InputFileName = malloc(240);
	fgets(InputFileName, 240, stdin);
	if (strstr(InputFileName, "\n"))
		strstr(InputFileName, "\n")[0] = 0x00;

	FILE* AttemptedFile = fopen(InputFileName, "wb");
	if (!AttemptedFile) {
		printf("Failed to open file.\n");
		free(InputFileName);
		return;
	}

	printf(" Would you like to mount a binary file in the disk? [Y/N]: ");
	char YNInput = fgetc(stdin);
	fgetc(stdin); // absorb said newline
	

	WORD64 WhereToMount = 0xFFFFFFFF, DestinedSize = 0;
	void* AttemptedBuffer = NULL;
	if ((YNInput & ~(0x20)) == 'Y') {
		printf("  Please enter the path: ");
		fgets(InputFileName, 239, stdin);

		FILE* AttemptedMount = fopen(InputFileName, "rb");
		if (!AttemptedMount) {
			printf("  Failed to open the file.\n");
			free(InputFileName);
			fclose(AttemptedFile);
			return;
		}

	GoBackHere:
		printf("  Where would you like to mount this file?: 0x");
		scanf("%llX ", &WhereToMount);
		if (WhereToMount == 0xFFFFFFFF) {
            // this is dumb but whatever (@TODO 12/27/23 FIX - nw)
			printf("  Please choose a different mounting point.\n");
			goto GoBackHere;
		}

		fseek(AttemptedMount, 0, SEEK_END);
		DestinedSize = ftell(AttemptedMount);
		AttemptedBuffer = malloc(DestinedSize);

		fseek(AttemptedMount, 0, SEEK_SET);
		fread(AttemptedBuffer, DestinedSize, 1, AttemptedMount);

		fclose(AttemptedMount);
	}

	WORD64 WantedSize;
	char* TempBfr = malloc(32);
SizeTryAgain:
	printf(" How large would you like the drive in bytes?: ");
	fgets(TempBfr, 31, stdin);
	WantedSize = strtoull(TempBfr, NULL, 10);
	if (WhereToMount != 0xFFFFFFFF) {
		if (WantedSize < (WhereToMount + DestinedSize)) {
			printf(" Invalid drive size. Drive must be able to at "
                   "least the given map.\n");
			goto SizeTryAgain;
		}
	}
	free(TempBfr);

	WORD64 LBreak = 0x00;
	if (WantedSize == DestinedSize) {
		LBreak = WantedSize;
	} else {
		if (WhereToMount != 0xFFFFFFFF) {
			BYTE* Zeros = malloc(1024);
			memset(Zeros, 0, 1024);
			for (int i = 0; i < (WantedSize / 1024); i++) {
				fseek(AttemptedFile, (i * 1024) + sizeof(FDISK_HDR), 
                    SEEK_SET);
				fwrite(Zeros, 1024, 1, AttemptedFile);
				if ((i * 1024) > WhereToMount) {
					LBreak = (i * 1024);
					break;
				}
			}

			if (!LBreak) {
				Zeros = realloc(Zeros, WantedSize % 1024);
				fseek(AttemptedFile, (long)(sizeof(FDISK_HDR) + 
                    (WantedSize / 1024)), SEEK_SET);
				if (Zeros) {
					fwrite(Zeros, WantedSize % 1024, 1, AttemptedFile);
					free(Zeros);
				}
			} // shouldn't ever happen

			fseek(AttemptedFile, (long)WhereToMount + sizeof(FDISK_HDR), 
                SEEK_SET);
			fwrite(AttemptedBuffer, DestinedSize, 1, AttemptedFile);
			free(AttemptedBuffer);
		}
	}


	free(InputFileName);
	
	FDISK_HDR* FdHdr = malloc(sizeof(FDISK_HDR));
	memset(FdHdr, 0, sizeof(FDISK_HDR));
	
	FdHdr->Magic = FDISKHDR_MAGIC;
	srand(time(NULL) & 0xFFFFFFFF);
	FdHdr->DeviceSerial = rand();
	FdHdr->DeviceVendorId = 0x4273;
    
	strcpy(FdHdr->DeviceVendor, "PLASM2 HDDGnr8r");
	FdHdr->PartsSum = FdHdr->DeviceSerial + FdHdr->DeviceVendorId;
	for (int i = 0; i < 16; i++)
		FdHdr->PartsSum += FdHdr->DeviceVendor[i];
    
	FdHdr->DriveVirtualSize = WantedSize;
	if (LBreak)
		FdHdr->ExpectedPhysicalSize = LBreak + sizeof(FDISK_HDR);
	else
		FdHdr->ExpectedPhysicalSize = sizeof(FDISK_HDR);

	fwrite(FdHdr, sizeof(FDISK_HDR), 1, AttemptedFile);
	fclose(AttemptedFile);
	free(FdHdr);

	printf(" Successfully wrote HDD file.\n");
	return;
}
