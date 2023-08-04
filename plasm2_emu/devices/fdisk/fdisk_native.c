#include "fdisk.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
/*
fdisk_native.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6387)

void fdiski_setactivedrive(int DriveId) {
	fdiskctx->CurrentDrive = DriveId;
}

int  fdiski_getdrivecnt(void) {
	return fdiskctx->DriveCount;
}

void fdiski_drivesleep(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
		free(fdiskctx->Drives[DriveId].CurrentLoadedChunks[i]);
		fdiskctx->Drives[DriveId].LoadedChunkCpuTick[i] |= 0xF000000000000000;
	}

	fdiskctx->Drives[DriveId].IsDriveAwake = 0;
}

void fdiski_driveawake(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (fdiskctx->Drives[DriveId].IsDriveAwake)
		return;
	
	for (int i = 0; i < 4; i++) {
		fdiskctx->Drives[DriveId].CurrentLoadedChunks[i] = malloc(fdiskctx->Drives[DriveId].LoadedChunkSize[i]);
		pfseek(fdiskctx->Drives[i].DrivePhysicalPointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i] + sizeof(fdiskhdr_t), SEEK_SET);
		fread(fdiskctx->Drives[DriveId].CurrentLoadedChunks[i], fdiskctx->Drives[DriveId].LoadedChunkSize[i], 1, fdiskctx->Drives[DriveId].DrivePhysicalPointer);
		fdiskctx->Drives[DriveId].LoadedChunkCpuTick[i] &= ~(0xF000000000000000);
	}

	fdiskctx->Drives[DriveId].IsDriveAwake = 1;
	return;
} 

u64  fdiski_getdrivesize(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;

	return fdiskctx->Drives[DriveId].DrivePhysicalSize;
}

_bool fdiski_isdriveready(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;

	return 1;
}

u64  fdiski_driveread(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;


	for (int i = 0; i < 4; i++) {
		if (InRange(fdiskctx->Drives[DriveId].CurrentFilePointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i], fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i] + fdiskctx->Drives[DriveId].LoadedChunkSize[i])) {
			byte* AsBytes = ((byte*)fdiskctx->Drives[DriveId].CurrentLoadedChunks[i] + (fdiskctx->Drives[DriveId].CurrentFilePointer - fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i]));
			u64* AsU64 = (u64*)AsBytes;
			fdiskctx->Drives[DriveId].LoadedChunkCpuTick[i] = cput_gettime();
			if (!fdiskctx->Drives[DriveId].SkipInc && !fdiskctx->Drives[DriveId].DisableInc) {
				fdiskctx->Drives[DriveId].CurrentFilePointer += 0x8;
			}
			else {
				if (fdiskctx->Drives[DriveId].SkipInc)
					fdiskctx->Drives[DriveId].SkipInc = 0;
			}
			return AsU64[0];
		}
	}

	// hdd cache miss eh
	int NewChunkId = fdiskctx->Drives[DriveId].OldestChunk;
	free(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = fdiskctx->Drives[DriveId].CurrentFilePointer - (FDISK_CACHE_CHUNK / 2 );
	fdiskctx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	fdiskctx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = cput_gettime();
	fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(fdiskctx->Drives[DriveId].DrivePhysicalPointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, fdiskctx->Drives[DriveId].DrivePhysicalPointer);
	fdiskctx->Drives[DriveId].NextChunkScan = 0; // force a chunk scan

	byte* AsBytes = ((byte*)fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] + (fdiskctx->Drives[DriveId].CurrentFilePointer - fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId]));
	u64* AsU64 = (u64*)AsBytes;
	if (!fdiskctx->Drives[DriveId].SkipInc && !fdiskctx->Drives[DriveId].DisableInc) {
		fdiskctx->Drives[DriveId].CurrentFilePointer += 0x8;
	}
	else {
		if (fdiskctx->Drives[DriveId].SkipInc)
			fdiskctx->Drives[DriveId].SkipInc = 0;
	}
	return AsU64[0];
}

void fdiski_drivewrite(int DriveId, u64 Data) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
		if (InRange(fdiskctx->Drives[DriveId].CurrentFilePointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i], fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i] + fdiskctx->Drives[DriveId].LoadedChunkSize[i])) {
			byte* AsBytes = ((byte*)fdiskctx->Drives[DriveId].CurrentLoadedChunks[i] + (fdiskctx->Drives[DriveId].CurrentFilePointer - fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[i]));
			u64* AsU64 = (u64*)AsBytes;
			fdiskctx->Drives[DriveId].LoadedChunkCpuTick[i] = cput_gettime();
			AsU64[0] = Data;
			if (!fdiskctx->Drives[DriveId].SkipInc && !fdiskctx->Drives[DriveId].DisableInc) {
				fdiskctx->Drives[DriveId].CurrentFilePointer += 0x8;
			} else {
				if (fdiskctx->Drives[DriveId].SkipInc)
					fdiskctx->Drives[DriveId].SkipInc = 0;
			}
			return;
		}
	}

	int NewChunkId = fdiskctx->Drives[DriveId].OldestChunk;
	free(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = fdiskctx->Drives[DriveId].CurrentFilePointer - (FDISK_CACHE_CHUNK / 2);
	fdiskctx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	fdiskctx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = cput_gettime();
	fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(fdiskctx->Drives[DriveId].DrivePhysicalPointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, fdiskctx->Drives[DriveId].DrivePhysicalPointer);
	fdiskctx->Drives[DriveId].NextChunkScan = 0; // force a chunk scan

	byte* AsBytes = ((byte*)fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] + (fdiskctx->Drives[DriveId].CurrentFilePointer - fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId]));
	u64* AsU64 = (u64*)AsBytes;
	AsU64[0] = Data;
	if (!fdiskctx->Drives[DriveId].SkipInc && !fdiskctx->Drives[DriveId].DisableInc) {
		fdiskctx->Drives[DriveId].CurrentFilePointer += 0x8;
	}
	else {
		if (fdiskctx->Drives[DriveId].SkipInc)
			fdiskctx->Drives[DriveId].SkipInc = 0;
	}

	return;
}

u64  fdiski_drivegetserial(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;

	return fdiskctx->Drives[DriveId].DeviceSerial;
}

void fdiski_drivegetvendorsz(int DriveId, u64 Pointer) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	u64 RPointer = mmu_translate(Pointer, REASON_WRTE);
	char* TPointer = ((byte*)cpuctx->PhysicalMemory + RPointer); // PM usage good (reason: virtual memory)
	memcpy(TPointer, fdiskctx->Drives[DriveId].DeviceVendor, 16);

	return;
}

u64  fdiski_drivegetmodel(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;

	return fdiskctx->Drives[DriveId].DeviceModelNum;
}

u64  fdiski_drivegetvendorid(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;
	
	return fdiskctx->Drives[DriveId].DeviceVendorId;
}

void fdiski_farseek(int DriveId, u64 SpecPos) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	// forces a chunk search, as it targets this specific pointer
	fdiskctx->Drives[DriveId].SpeculativeSeek = SpecPos;
	int NewChunkId = fdiskctx->Drives[DriveId].OldestChunk;
	free(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId]);
	fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId] = fdiskctx->Drives[DriveId].SpeculativeSeek - (FDISK_CACHE_CHUNK / 2);
	fdiskctx->Drives[DriveId].LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	fdiskctx->Drives[DriveId].LoadedChunkCpuTick[NewChunkId] = cput_gettime();
	fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(fdiskctx->Drives[DriveId].DrivePhysicalPointer, fdiskctx->Drives[DriveId].LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(fdiskctx->Drives[DriveId].CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, fdiskctx->Drives[DriveId].DrivePhysicalPointer);
	fdiskctx->Drives[DriveId].NextChunkScan = 0;

	return;
}

void fdiski_skipfpinc(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	fdiskctx->Drives[DriveId].SkipInc = 1;
}

void fdiski_fpincon(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	fdiskctx->Drives[DriveId].DisableInc = 0;
}

void fdiski_fpincoff(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	fdiskctx->Drives[DriveId].DisableInc = 1;
}

void fdiski_driveseek(int DriveId, u64 NewPos) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	fdiskctx->Drives[DriveId].CurrentFilePointer = NewPos;
}

u64  fdiski_drivereadstack(int DriveId) {
	if (DriveId >= fdiskctx->DriveCount)
		return 0;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return 0;

	u64 CFPBackup = fdiskctx->Drives[DriveId].CurrentFilePointer;
	fdiskctx->Drives[DriveId].CurrentFilePointer = mmu_pop();
	u64 Return = fdiski_driveread(DriveId);
	fdiskctx->Drives[DriveId].CurrentFilePointer = CFPBackup;
	return Return;
}

void fdiski_drivewritestack(int DriveId, u64 Data) {
	if (DriveId >= fdiskctx->DriveCount)
		return;
	if (!fdiskctx->Drives[DriveId].IsDriveAwake)
		return;

	u64 CFPBackup = fdiskctx->Drives[DriveId].CurrentFilePointer;
	fdiskctx->Drives[DriveId].CurrentFilePointer = mmu_pop();
	fdiski_drivewrite(DriveId, Data);
	fdiskctx->Drives[DriveId].CurrentFilePointer = CFPBackup;

	return;
}