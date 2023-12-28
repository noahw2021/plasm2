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
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
        
		free(ThisDrive->CurrentLoadedChunks[i]);
		ThisDrive->LoadedChunkCpuTick[i] |= 0xF000000000000000;
	}

	ThisDrive->IsDriveAwake = 0;
    return;
}

void FdiskiDriveAwake(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (ThisDrive->IsDriveAwake)
		return;
	
	for (int i = 0; i < 4; i++) {
		ThisDrive->CurrentLoadedChunks[i] = malloc(
            ThisDrive->LoadedChunkSize[i]);
		pfseek(FdiskCtx->Drives[i].DrivePhysicalPointer, 
            ThisDrive->LoadedChunkBaseAddr[i] + sizeof(FDISK_HDR), SEEK_SET);
		fread(ThisDrive->CurrentLoadedChunks[i], 
            ThisDrive->LoadedChunkSize[i], 1,
            ThisDrive->DrivePhysicalPointer);
		ThisDrive->LoadedChunkCpuTick[i] &= ~(0xF000000000000000);
	}

	ThisDrive->IsDriveAwake = 1;
	return;
} 

WORD64  FdiskiGetDriveSize(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;

	return ThisDrive->DrivePhysicalSize;
}

_bool FdiskiIsDriveReady(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;

	return 1;
}

WORD64  FdiskiDriveRead(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;

	for (int i = 0; i < 4; i++) {
		if (InRange(ThisDrive->CurrentFilePointer, 
                ThisDrive->LoadedChunkBaseAddr[i],
                ThisDrive->LoadedChunkBaseAddr[i]
                + ThisDrive->LoadedChunkSize[i])
        ) {
			BYTE* AsBytes = ((BYTE*)ThisDrive->CurrentLoadedChunks[i] +
                (ThisDrive->CurrentFilePointer -
                ThisDrive->LoadedChunkBaseAddr[i]));
            
			WORD64* AsU64 = (WORD64*)AsBytes;
            
			ThisDrive->LoadedChunkCpuTick[i] = CpuTimerGetTime();
			
            if (!ThisDrive->SkipInc && !ThisDrive->DisableInc) {
				ThisDrive->CurrentFilePointer += 0x8;
			} else {
				if (ThisDrive->SkipInc)
					ThisDrive->SkipInc = 0;
			}
            
			return AsU64[0];
		}
	}

	// hdd cache miss
	int NewChunkId = ThisDrive->OldestChunk;
	free(ThisDrive->CurrentLoadedChunks[NewChunkId]);
	ThisDrive->LoadedChunkBaseAddr[NewChunkId] = 
        ThisDrive->CurrentFilePointer - (FDISK_CACHE_CHUNK / 2 );
	ThisDrive->LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	ThisDrive->LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	ThisDrive->CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(ThisDrive->DrivePhysicalPointer, 
        ThisDrive->LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(ThisDrive->CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 
        1, ThisDrive->DrivePhysicalPointer);
	ThisDrive->NextChunkScan = 0; // force a chunk scan

	BYTE* AsBytes = ((BYTE*)ThisDrive->CurrentLoadedChunks[NewChunkId]
        + (ThisDrive->CurrentFilePointer -
        ThisDrive->LoadedChunkBaseAddr[NewChunkId]));
    
	WORD64* AsU64 = (WORD64*)AsBytes;
	
    if (!ThisDrive->SkipInc && !ThisDrive->DisableInc) {
		ThisDrive->CurrentFilePointer += 0x8;
	} else {
		if (ThisDrive->SkipInc)
			ThisDrive->SkipInc = 0;
	}
    
	return AsU64[0];
}

void FdiskiDriveWrite(WORD32 DriveId, WORD64 Data) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	for (int i = 0; i < 4; i++) {
		if (InRange(ThisDrive->CurrentFilePointer, 
                ThisDrive->LoadedChunkBaseAddr[i],
                ThisDrive->LoadedChunkBaseAddr[i]
                + ThisDrive->LoadedChunkSize[i])
        ) {
			BYTE* AsBytes = ((BYTE*)ThisDrive->CurrentLoadedChunks[i] + 
                (ThisDrive->CurrentFilePointer -
                ThisDrive->LoadedChunkBaseAddr[i]));
            
			WORD64* AsU64 = (WORD64*)AsBytes;
			ThisDrive->LoadedChunkCpuTick[i] = CpuTimerGetTime();
			AsU64[0] = Data;
            
			if (!ThisDrive->SkipInc && !ThisDrive->DisableInc) {
				ThisDrive->CurrentFilePointer += 0x8;
			} else {
				if (ThisDrive->SkipInc)
					ThisDrive->SkipInc = 0;
			}
			return;
		}
	}

	int NewChunkId = ThisDrive->OldestChunk;
	free(ThisDrive->CurrentLoadedChunks[NewChunkId]);
	ThisDrive->LoadedChunkBaseAddr[NewChunkId] = 
        ThisDrive->CurrentFilePointer - (FDISK_CACHE_CHUNK / 2);
	ThisDrive->LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	ThisDrive->LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	ThisDrive->CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(ThisDrive->DrivePhysicalPointer, 
        ThisDrive->LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(ThisDrive->CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, 
        ThisDrive->DrivePhysicalPointer);
	ThisDrive->NextChunkScan = 0; // force a chunk scan

	BYTE* AsBytes = ((BYTE*)ThisDrive->CurrentLoadedChunks[NewChunkId]
        + (ThisDrive->CurrentFilePointer -
        ThisDrive->LoadedChunkBaseAddr[NewChunkId]));
    
	WORD64* AsU64 = (WORD64*)AsBytes;
	AsU64[0] = Data;
	
    if (!ThisDrive->SkipInc && !ThisDrive->DisableInc) {
		ThisDrive->CurrentFilePointer += 0x8;
	} else {
		if (ThisDrive->SkipInc)
			ThisDrive->SkipInc = 0;
	}

	return;
}

WORD64 FdiskiGetDriveSerial(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
	if (!ThisDrive->IsDriveAwake)
		return 0;

	return ThisDrive->DeviceSerial;
}

void FdiskiGetDriveVendorString(WORD32 DriveId, WORD64 Pointer) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	WORD64 RPointer = MmuTranslate(Pointer, REASON_WRTE, 16);
	BYTE* TPointer = ((BYTE*)CpuCtx->PhysicalMemory + RPointer);
	memcpy(TPointer, ThisDrive->DeviceVendor, 16);

	return;
}

WORD64  FdiskiGetDriveModel(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;

	return ThisDrive->DeviceModelNum;
}

WORD64  FdiskiGetDriveVendorId(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;
	
	return ThisDrive->DeviceVendorId;
}

void FdiskiFarSeek(WORD32 DriveId, WORD64 SpecPos) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	// forces a chunk search, as it targets this specific pointer
	ThisDrive->SpeculativeSeek = SpecPos;
	int NewChunkId = ThisDrive->OldestChunk;
	free(ThisDrive->CurrentLoadedChunks[NewChunkId]);
	ThisDrive->LoadedChunkBaseAddr[NewChunkId] = 
        ThisDrive->SpeculativeSeek - (FDISK_CACHE_CHUNK / 2);
	ThisDrive->LoadedChunkSize[NewChunkId] = FDISK_CACHE_CHUNK;
	ThisDrive->LoadedChunkCpuTick[NewChunkId] = CpuTimerGetTime();
	ThisDrive->CurrentLoadedChunks[NewChunkId] = malloc(FDISK_CACHE_CHUNK);
	pfseek(ThisDrive->DrivePhysicalPointer, 
        ThisDrive->LoadedChunkBaseAddr[NewChunkId], SEEK_SET);
	fread(ThisDrive->CurrentLoadedChunks[NewChunkId], FDISK_CACHE_CHUNK, 1, 
        ThisDrive->DrivePhysicalPointer);
	ThisDrive->NextChunkScan = 0;

	return;
}

void FdiskiSkipFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	ThisDrive->SkipInc = 1;
    return;
}

void FdiskiEnableFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	ThisDrive->DisableInc = 0;
    return;
}

void FdiskiDisableFpIncrement(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	ThisDrive->DisableInc = 1;
    return;
}

void FdiskiDriveSeek(WORD32 DriveId, WORD64 NewPos) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	ThisDrive->CurrentFilePointer = NewPos;
    return;
}

WORD64  FdiskiDriveReadStack(WORD32 DriveId) {
	if (DriveId >= FdiskCtx->DriveCount)
		return 0;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return 0;

	WORD64 CFPBackup = ThisDrive->CurrentFilePointer;
	ThisDrive->CurrentFilePointer = MmuPop();
	WORD64 Return = FdiskiDriveRead(DriveId);
	ThisDrive->CurrentFilePointer = CFPBackup;
	return Return;
}

void FdiskiDriveWriteStack(WORD32 DriveId, WORD64 Data) {
	if (DriveId >= FdiskCtx->DriveCount)
		return;
    PFDISK_DRIVE ThisDrive = &FdiskCtx->Drives[DriveId];
    
	if (!ThisDrive->IsDriveAwake)
		return;

	WORD64 CFPBackup = ThisDrive->CurrentFilePointer;
	ThisDrive->CurrentFilePointer = MmuPop();
	FdiskiDriveWrite(DriveId, Data);
	ThisDrive->CurrentFilePointer = CFPBackup;

	return;
}
