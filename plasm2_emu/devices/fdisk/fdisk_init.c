//
//  fdisk_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"
#include "../../cpu/cpu.h"
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 6011 6308 6387 26451)

PFDISK_CTX FdiskCtx;

void FdiskInit(void) {
	FdiskCtx = malloc(sizeof(FDISK_CTX));
	memset(FdiskCtx, 0, sizeof(FDISK_CTX));

	FdiskCtx->CurrentStatus = DEVSTATUS_GOOD;
}

void FdiskShutdown(void) {
	for (int i = 0; i < FdiskCtx->DriveCount; i++) {
		fclose(FdiskCtx->Drives[i].DrivePhysicalPointer);
		for (int m = 0; m < 4; m++) {
			if (FdiskCtx->Drives[i].CurrentLoadedChunks[m])
				free(FdiskCtx->Drives[i].CurrentLoadedChunks[m]);
		}
	}
	free(FdiskCtx->Drives);
	free(FdiskCtx);
}

_bool FdiskRegister(const char* DiskFile) {
	FILE* Tfp = fopen(DiskFile, "wb+");
	if (!Tfp)
		return 0;

	FDISK_HDR* PotHeader = malloc(sizeof(FDISK_HDR));
	fread(PotHeader, sizeof(FDISK_HDR), 1, Tfp);
	
	if (PotHeader->Magic != FDISKHDR_MAGIC) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	WORD64 RunningTotal = 0x00;
	for (int i = 0; i < sizeof(PotHeader->DeviceVendor); i++)
		RunningTotal += PotHeader->DeviceVendor[i];
	RunningTotal += PotHeader->DeviceVendorId;
	RunningTotal += PotHeader->DeviceSerial;
	RunningTotal += PotHeader->DeviceModelNum;

	if (RunningTotal != PotHeader->PartsSum) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	fseek(Tfp, 0, SEEK_END);
	WORD64 FileSize = pftell(Tfp);
	FileSize -= sizeof(FDISK_HDR);

	if (FileSize != PotHeader->ExpectedPhysicalSize) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	if (!FdiskCtx->Drives)
		FdiskCtx->Drives = malloc(sizeof(*FdiskCtx->Drives));
	else
		FdiskCtx->Drives = realloc(FdiskCtx->Drives, sizeof(*FdiskCtx->Drives) * (FdiskCtx->DriveCount + 1));
	memset(&FdiskCtx->Drives[FdiskCtx->DriveCount], 0, sizeof(*FdiskCtx->Drives));

	FdiskCtx->Drives[FdiskCtx->DriveCount].CurrentFilePointer = 0x00;
	FdiskCtx->Drives[FdiskCtx->DriveCount].DeviceSerial = PotHeader->DeviceSerial;
	memcpy(FdiskCtx->Drives[FdiskCtx->DriveCount].DeviceVendor, PotHeader->DeviceVendor, 16);
	FdiskCtx->Drives[FdiskCtx->DriveCount].DeviceVendorId = PotHeader->DeviceVendorId;
	FdiskCtx->Drives[FdiskCtx->DriveCount].DrivePhysicalPointer = Tfp;
	FdiskCtx->Drives[FdiskCtx->DriveCount].DrivePhysicalSize = FileSize;
	FdiskCtx->Drives[FdiskCtx->DriveCount].IsDriveActive = 1;
	FdiskCtx->Drives[FdiskCtx->DriveCount].IsDriveAwake = 1;

	FdiskCtx->DriveCount++;

	return 0;
}

void FdiskClock(void) {
	int OldestPair = 0;
	WORD64 OldestChunk = 0xFFFFFFFFFFFFFFFF;
	for (int i = 0; i < FdiskCtx->DriveCount; i++) {
		if (FdiskCtx->Drives[i].NextChunkScan > cput_gettime())
			continue;
		for (int c = 0; c < 4; c++) {
			if (FdiskCtx->Drives[i].LoadedChunkCpuTick[c] < OldestChunk) {
				OldestChunk = FdiskCtx->Drives[i].LoadedChunkCpuTick[c];
				OldestPair = c;
			}
		}
		FdiskCtx->Drives[i].OldestChunk = OldestPair;
	}

	// write cache
}
