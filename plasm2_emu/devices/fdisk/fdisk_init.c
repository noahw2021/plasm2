#include "fdisk.h"
#include "../cpu/cpu.h"
#include <stdlib.h>
#include <string.h>
/*
fdisk_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6308 6387 26451)

fdiskctx_t* fdiskctx;

void fdisk_init(void) {
	fdiskctx = malloc(sizeof(fdiskctx_t));
	memset(fdiskctx, 0, sizeof(fdiskctx_t));

	fdiskctx->CurrentStatus = DEVSTATUS_GOOD;
}

void fdisk_shutdown(void) {
	for (int i = 0; i < fdiskctx->DriveCount; i++) {
		fclose(fdiskctx->Drives[i].DrivePhysicalPointer);
		for (int m = 0; m < 4; m++) {
			if (fdiskctx->Drives[i].CurrentLoadedChunks[m])
				free(fdiskctx->Drives[i].CurrentLoadedChunks[m]);
		}
	}
	free(fdiskctx->Drives);
	free(fdiskctx);
}

_bool fdisk_register(const char* DiskFile) {
	FILE* Tfp = fopen(DiskFile, "rb");
	if (!Tfp)
		return 0;

	fdiskhdr_t* PotHeader = malloc(sizeof(fdiskhdr_t));
	fread(PotHeader, sizeof(fdiskhdr_t), 1, Tfp);
	
	if (PotHeader->Magic != FDISKHDR_MAGIC) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	u64 RunningTotal = 0x00;
	for (int i = 0; i < sizeof(PotHeader->DeviceVendor); i++)
		RunningTotal += PotHeader->DeviceVendor[i];
	RunningTotal += PotHeader->DeviceVendorId;
	RunningTotal += PotHeader->DeviceSerial;
	RunningTotal += PotHeader->DeviceModelNum;



	fseek(Tfp, 0, SEEK_END);
	u64 FileSize = pftell(Tfp);


	if (!fdiskctx->Drives)
		fdiskctx->Drives = malloc(sizeof(*fdiskctx->Drives));
	else
		fdiskctx->Drives = realloc(fdiskctx->Drives, sizeof(*fdiskctx->Drives) * (fdiskctx->DriveCount + 1));
	memset(&fdiskctx->Drives[fdiskctx->DriveCount], 0, sizeof(*fdiskctx->Drives));

	fdiskctx->Drives[fdiskctx->DriveCount].CurrentFilePointer = 0x00;
	fdiskctx->Drives[fdiskctx->DriveCount].DeviceSerial = PotHeader->DeviceSerial;
	memcpy(fdiskctx->Drives[fdiskctx->DriveCount].DeviceVendor, PotHeader->DeviceVendor, 16);
	fdiskctx->Drives[fdiskctx->DriveCount].DeviceVendorId = PotHeader->DeviceVendorId;
	fdiskctx->Drives[fdiskctx->DriveCount].DrivePhysicalPointer = Tfp;
	fdiskctx->Drives[fdiskctx->DriveCount].DrivePhysicalSize = FileSize;
	fdiskctx->Drives[fdiskctx->DriveCount].IsDriveActive = 1;
	fdiskctx->Drives[fdiskctx->DriveCount].IsDriveAwake = 1;

	fdiskctx->DriveCount++;

	return 1;
}

void fdisk_clock(void) {
	int OldestPair = 0;
	u64 OldestChunk = 0xFFFFFFFFFFFFFFFF;
	for (int i = 0; i < fdiskctx->DriveCount; i++) {
		if (fdiskctx->Drives[i].NextChunkScan > cput_gettime())
			continue;
		for (int c = 0; c < 4; c++) {
			if (fdiskctx->Drives[i].LoadedChunkCpuTick[c] < OldestChunk) {
				OldestChunk = fdiskctx->Drives[i].LoadedChunkCpuTick[c];
				OldestPair = c;
			}
		}
		fdiskctx->Drives[i].OldestChunk = OldestPair;
	}

	// write cache
}