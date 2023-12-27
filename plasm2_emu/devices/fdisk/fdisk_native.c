//
//  fdisk_native.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 6387)

void FdiskiSetActiveDrive(WORD32 DriveId) {
	FdiskCtx->CurrentDrive = DriveId;
}

WORD32 FdiskiGetDriveCount(void) {
	return FdiskCtx->DriveCount;
}

void FdiskiDriveSleep(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
		free(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[i]);
		FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[i] |= 
            0xF000000000000000;
	}

	FdiskCtx->Drives[DriveId].IsDriveAwake = 0;
}

void FdiskiDriveAwake(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;
	
	for (int i = 0; i < 4; i++) {
		FdiskCtx->Drives[DriveId].CurrentLoadedChunks[i] = malloc(FdiskCtx->Drives[DriveId].LoadedChunkSize[i]);
		pfseek(FdiskCtx->Drives[i].DrivePhysicalPointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i] + sizeof(FDISK_HDR), SEEK_SET);
		fread(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[i], FdiskCtx->Drives[DriveId].LoadedChunkSize[i], 1, FdiskCtx->Drives[DriveId].DrivePhysicalPointer);
		FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[i] &= ~(0xF000000000000000);
	}

	FdiskCtx->Drives[DriveId].IsDriveAwake = 1;
	return;
} 

WORD64  FdiskiGetDriveSize(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;

	return FdiskCtx->Drives[DriveId].DrivePhysicalSize;
}

_bool FdiskiIsDriveReady(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;

	return 1;
}

WORD64  FdiskiDriveRead(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;


	for (int i = 0; i < 4; i++) {
		if (InRange(FdiskCtx->Drives[DriveId].CurrentFilePointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i], FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i] + FdiskCtx->Drives[DriveId].LoadedChunkSize[i])) {
			BYTE* AsBytes = ((BYTE*)FdiskCtx->Drives[DriveId].CurrentLoadedChunks[i] + (FdiskCtx->Drives[DriveId].CurrentFilePointer - FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i]));
			WORD64* AsU64 = (WORD64*)AsBytes;
			FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[i] = CpuTimerGetTime();
			if (!FdiskCtx->Drives[DriveId].SkipInc && !FdiskCtx->Drives[DriveId].DisableInc) {
				FdiskCtx->Drives[DriveId].CurrentFilePointer += 0x8;
			}
			else {
				if (FdiskCtx->Drives[DriveId].SkipInc)
					FdiskCtx->Drives[DriveId].SkipInc = 0;
			}
			return AsU64[0];
		}
	}

	// hdd cache miss eh
	int NewChunkId = FdiskCtx->Drives[DriveId].OldestChunk;
	free(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = FdiskCtx->Drives[DriveId].CurrentFilePointer - (FDISK_CACHE_CHUNK / 2 );
	FdiskCtx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(FdiskCtx->Drives[DriveId].DrivePhysicalPointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, FdiskCtx->Drives[DriveId].DrivePhysicalPointer);
	FdiskCtx->Drives[DriveId].NextChunkScan = 0; // force a chunk scan

	BYTE* AsBytes = ((BYTE*)FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] + (FdiskCtx->Drives[DriveId].CurrentFilePointer - FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId]));
	WORD64* AsU64 = (WORD64*)AsBytes;
	if (!FdiskCtx->Drives[DriveId].SkipInc && !FdiskCtx->Drives[DriveId].DisableInc) {
		FdiskCtx->Drives[DriveId].CurrentFilePointer += 0x8;
	}
	else {
		if (FdiskCtx->Drives[DriveId].SkipInc)
			FdiskCtx->Drives[DriveId].SkipInc = 0;
	}
	return AsU64[0];
}

void FdiskiDriveWrite(WORD32 DriveId, WORD64 Data) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
		if (InRange(FdiskCtx->Drives[DriveId].CurrentFilePointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i], FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i] + FdiskCtx->Drives[DriveId].LoadedChunkSize[i])) {
			BYTE* AsBytes = ((BYTE*)FdiskCtx->Drives[DriveId].CurrentLoadedChunks[i] + (FdiskCtx->Drives[DriveId].CurrentFilePointer - FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[i]));
			WORD64* AsU64 = (WORD64*)AsBytes;
			FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[i] = CpuTimerGetTime();
			AsU64[0] = Data;
			if (!FdiskCtx->Drives[DriveId].SkipInc && !FdiskCtx->Drives[DriveId].DisableInc) {
				FdiskCtx->Drives[DriveId].CurrentFilePointer += 0x8;
			} else {
				if (FdiskCtx->Drives[DriveId].SkipInc)
					FdiskCtx->Drives[DriveId].SkipInc = 0;
			}
			return;
		}
	}

	int NewChunkId = FdiskCtx->Drives[DriveId].OldestChunk;
	free(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = FdiskCtx->Drives[DriveId].CurrentFilePointer - (FDISK_CACHE_CHUNK / 2);
	FdiskCtx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(FdiskCtx->Drives[DriveId].DrivePhysicalPointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, FdiskCtx->Drives[DriveId].DrivePhysicalPointer);
	FdiskCtx->Drives[DriveId].NextChunkScan = 0; // force a chunk scan

	BYTE* AsBytes = ((BYTE*)FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] + (FdiskCtx->Drives[DriveId].CurrentFilePointer - FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId]));
	WORD64* AsU64 = (WORD64*)AsBytes;
	AsU64[0] = Data;
	if (!FdiskCtx->Drives[DriveId].SkipInc && !FdiskCtx->Drives[DriveId].DisableInc) {
		FdiskCtx->Drives[DriveId].CurrentFilePointer += 0x8;
	}
	else {
		if (FdiskCtx->Drives[DriveId].SkipInc)
			FdiskCtx->Drives[DriveId].SkipInc = 0;
	}

	return;
}

WORD64  FdiskiGetDriveSerial(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;

	return FdiskCtx->Drives[DriveId].DeviceSerial;
}

void FdiskiGetDriveVendorString(WORD32 DriveId, WORD64 Pointer) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	WORD64 RPointer = MmuTranslate(Pointer, REASON_WRTE, 16);
	BYTE* TPointer = ((BYTE*)CpuCtx->PhysicalMemory + RPointer);
	memcpy(TPointer, FdiskCtx->Drives[DriveId].DeviceVendor, 16);

	return;
}

WORD64  FdiskiGetDriveModel(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;

	return FdiskCtx->Drives[DriveId].DeviceModelNum;
}

WORD64  FdiskiGetDriveVendorId(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;
	
	return FdiskCtx->Drives[DriveId].DeviceVendorId;
}

void FdiskiFarSeek(WORD32 DriveId, WORD64 SpecPos) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	// forces a chunk search, as it targets this specific pointer
	FdiskCtx->Drives[DriveId].SpeculativeSeek = SpecPos;
	int NewChunkId = FdiskCtx->Drives[DriveId].OldestChunk;
	free(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = FdiskCtx->Drives[DriveId].SpeculativeSeek - (FDISK_CACHE_CHUNK / 2);
	FdiskCtx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	FdiskCtx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(FdiskCtx->Drives[DriveId].DrivePhysicalPointer, FdiskCtx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(FdiskCtx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, FdiskCtx->Drives[DriveId].DrivePhysicalPointer);
	FdiskCtx->Drives[DriveId].NextChunkScan = 0;

	return;
}

void FdiskiSkipFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	FdiskCtx->Drives[DriveId].SkipInc = 1;
}

void FdiskiEnableFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	FdiskCtx->Drives[DriveId].DisableInc = 0;
}

void FdiskiDisableFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	FdiskCtx->Drives[DriveId].DisableInc = 1;
}

void FdiskiDriveSeek(WORD32 DriveId, WORD64 NewPos) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	FdiskCtx->Drives[DriveId].CurrentFilePointer = NewPos;
}

WORD64  FdiskiDriveReadStack(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return 0;

	WORD64 CFPBackup = FdiskCtx->Drives[DriveId].CurrentFilePointer;
	FdiskCtx->Drives[DriveId].CurrentFilePointer = MmuPop();
	WORD64 Return = FdiskiDriveRead(DriveId);
	FdiskCtx->Drives[DriveId].CurrentFilePointer = CFPBackup;
	return Return;
}

void FdiskiDriveWriteStack(WORD32 DriveId, WORD64 Data) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
	if (!FdiskCtx->Drives[DriveId].IsDriveAwake)
		return;

	WORD64 CFPBackup = FdiskCtx->Drives[DriveId].CurrentFilePointer;
	FdiskCtx->Drives[DriveId].CurrentFilePointer = MmuPop();
	FdiskiDriveWrite(DriveId, Data);
	FdiskCtx->Drives[DriveId].CurrentFilePointer = CFPBackup;

	return;
}
