#pragma once
#include "../devices.h"
#include "../../basetypes.h"
#include <stdio.h>
/*
fdisk.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

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

void fdisk_init(void);
void fdisk_shutdown(void);
void fdisk_clock(void);
void fdisk_collect(void);
_bool fdisk_register(const char* DiskFile);

u64  fdisk_statusquery(u64 Device);
void fdisk_sendcommand(u64 Device, u64 Command);
void fdisk_senddata(u64 Device, u64 Command);
u64  fdisk_getdata(u64 Device);
void fdisk_reset(u64 Device);
void fdisk_off(u64 Device);
void fdisk_on(u64 Device);

#define FDISK_CACHE_TOTAL 1048576
#define FDISK_CACHE_CHUNK FDISK_CACHE_TOTAL / 4

#define FDISKHDR_MAGIC 'PLFD'
typedef struct _fdiskhdr {
	u64 Magic;

	u64 DriveVirtualSize;
	u64 ExpectedPhysicalSize;

	char DeviceVendor[16];
	u64 DeviceSerial;
	u64 DeviceVendorId;
	u64 DeviceModelNum;
	u64 PartsSum; // not really a hash, but will tell us if something is wrong
}fdiskhdr_t;

typedef struct _fdiskctx {
	int DriveCount;
	int CurrentDrive;
	struct {
		_bool IsDriveAwake;
		_bool IsDriveActive;

		FILE* DrivePhysicalPointer;
		u64 DrivePhysicalSize;
		void* CurrentLoadedChunks[4];
		u64 LoadedChunkSize[4];
		u64 LoadedChunkBaseAddr[4];
		u64 LoadedChunkCpuTick[4];
		int OldestChunk;
		u64 NextChunkScan;

		char DeviceVendor[16];
		u64 DeviceSerial;
		u64 DeviceVendorId;
		u64 DeviceModelNum;

		u64 CurrentFilePointer;
		u64 SpeculativeSeek;
		byte DisableInc;
		byte SkipInc;
	}*Drives;

	u64 OutgoingData;
	byte RecvData;
	byte CurrentCommand;
	byte ExpectingData;
	u64 CurrentStatus;
}fdiskctx_t;
extern fdiskctx_t* fdiskctx;

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

void fdiski_setactivedrive(int DriveId);
int  fdiski_getdrivecnt(void);
void fdiski_drivesleep(int DriveId);
void fdiski_driveawake(int DriveId);
u64  fdiski_getdrivesize(int DriveId);
_bool fdiski_isdriveready(int DriveId);
u64  fdiski_driveread(int DriveId);
void fdiski_drivewrite(int DriveId, u64 Data);
u64  fdiski_drivegetserial(int DriveId);
void fdiski_drivegetvendorsz(int DriveId, u64 Pointer);
u64  fdiski_drivegetmodel(int DriveId);
u64  fdiski_drivegetvendorid(int DriveId);
void fdiski_farseek(int DriveId, u64 SpecPos);
void fdiski_skipfpinc(int DriveId);
void fdiski_fpincon(int DriveId);
void fdiski_fpincoff(int DriveId);
void fdiski_driveseek(int DriveId, u64 NewPos);
u64  fdiski_drivereadstack(int DriveId);
void fdiski_drivewritestack(int DriveId, u64 Data);