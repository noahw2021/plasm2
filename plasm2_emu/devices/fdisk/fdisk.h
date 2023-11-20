//
//  fdisk.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../devices.h"
#include "../../basetypes.h"
#include <stdio.h>

#ifdef _WIN32
#define pfseek _fseeki64
#define pftell _ftelli64
#elif __linux__
#define pfseek fseeko
#define pftell ftello
#else
#define pfseek fseek
#define pftell ftell
#endif

// Fixed Disk Controller

void FdiskInit(void);
void FdiskShutdown(void);
void FdiskClock(void);
void FdiskCollect(void);
_bool FdiskRegister(const char* DiskFile);

WORD64 FdiskStatusQuery(WORD32 Device, WORD64 NullArg);
WORD64 FdiskSendCommand(WORD32 Device, WORD64 Command);
WORD64 FdiskSendData(WORD32 Device, WORD64 Command);
WORD64 FdiskGetData(WORD32 Device, WORD64 NullArg);
WORD64 FdiskReset(WORD32 Device, WORD64 NullArg);
WORD64 FdiskOff(WORD32 Device, WORD64 NullArg);
WORD64 FdiskOn(WORD32 Device, WORD64 NullArg);

#define FDISK_CACHE_TOTAL 1048576
#define FDISK_CACHE_CHUNK FDISK_CACHE_TOTAL / 4

#define FDISKHDR_MAGIC 'PLFD'
typedef struct _FDISK_HDR {
	WORD64 Magic;

	WORD64 DriveVirtualSize;
	WORD64 ExpectedPhysicalSize;

	char DeviceVendor[16];
	WORD64 DeviceSerial;
	WORD64 DeviceVendorId;
	WORD64 DeviceModelNum;
	WORD64 PartsSum; // not really a hash, but will tell us if something is wrong
}FDISK_HDR;

typedef struct _FDISK_CTX {
	int DriveCount;
	int CurrentDrive;
	struct {
		_bool IsDriveAwake;
		_bool IsDriveActive;

		FILE* DrivePhysicalPointer;
		WORD64 DrivePhysicalSize;
		void* CurrentLoadedChunks[4];
		WORD64 LoadedChunkSize[4];
		WORD64 LoadedChunkBaseAddr[4];
		WORD64 LoadedChunkCpuTick[4];
		int OldestChunk;
		WORD64 NextChunkScan;

		char DeviceVendor[16];
		WORD64 DeviceSerial;
		WORD64 DeviceVendorId;
		WORD64 DeviceModelNum;

		WORD64 CurrentFilePointer;
		WORD64 SpeculativeSeek;
		BYTE DisableInc;
		BYTE SkipInc;
	}*Drives;

	WORD64 OutgoingData;
	BYTE RecvData;
	BYTE CurrentCommand;
	BYTE ExpectingData;
	WORD64 CurrentStatus;
}FDISK_CTX, *PFDISK_CTX;
extern PFDISK_CTX FdiskCtx;

/*
commands:
c
00 SetActiveDrive : Sets the active drive
01 GetDriveCount : Gets the total number of drives
02 DriveSleep : Puts a drive to sleep
03 DriveAwake: Wakes up a drive
04 GetDriveSize : Returns the drive size in bytes
05 IsDriveReady : Determines if a drive is ready for read/writes
06 DriveRead : Reads 8 bytes from a drive
07 DriveWrite : Writes 8 bytes to a drive
08 GetDriveSerial : Determines the serial of the drive
09 GetDriveVendorName : Writes the drive vendor name to a given pointer.
0A GetDriveModelNumber: Gets the drive's model number
0B GetDriveVendorId : Gets the drive's vendor's ID
0C FarSeek : Specutively preloads a far address to seek to
0D DriveSkipFpIncrement : Disables the seek pos increment for the next read/write
0E DriveDisableFpIncrement : Disables the seek pos increment until reenabled
0F DriveEnableFpIncrement : Reenables the seek pos increment
10 DriveSeek : Sets the drive seek pos (current)
11 DriveReadStack : Reads 8 bytes from a drive, with the seek pos given in the stack
12 DriveWriteStack : Writes 8 bytes to a drive, with the seek pos given in the stack
*/

void FdiskiSetActiveDrive(int DriveId);
int  FdiskiGetDriveCount(void);
void FdiskiDriveSleep(int DriveId);
void FdiskiDriveAwake(int DriveId);
WORD64 FdiskiGetDriveSize(int DriveId);
_bool FdiskiIsDriveReady(int DriveId);
WORD64 FdiskiDriveRead(int DriveId);
void FdiskiDriveWrite(int DriveId, WORD64 Data);
WORD64 FdiskiGetDriveSerial(int DriveId);
void FdiskiGetDriveVendorString(int DriveId, WORD64 Pointer);
WORD64 FdiskiGetDriveModel(int DriveId);
WORD64 FdiskiGetDriveVendorId(int DriveId);
void FdiskiFarSeek(int DriveId, WORD64 SpecPos);
void FdiskiSkipFpIncrement(int DriveId);
void FdiskiEnableFpIncrement(int DriveId);
void FdiskiDisableFpIncrement(int DriveId);
void FdiskiDriveSeek(int DriveId, WORD64 NewPos);
WORD64 FdiskiDriveReadStack(int DriveId);
void FdiskiDriveWriteStack(int DriveId, WORD64 Data);
