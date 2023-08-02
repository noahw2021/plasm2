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

// Fixed Disk Controller

void fdisk_init(void);
void fdisk_shutdown(void);
void fdisk_clock(void);
void fdisk_collect(void);
BOOL fdisk_register(const char* DiskFile);

u64  fdisk_statusquery(u32 Device);
void fdisk_sendcommand(u32 Device, u64 Command);
void fdisk_senddata(u32 Device, u64 Command);
u64  fdisk_getdata(u32 Device);
void fdisk_reset(u32 Device);
void fdisk_off(u32 Device);
void fdisk_on(u32 Device);

#define FDISKHDR_MAGIC 'PLFD'
typedef struct _fdiskhdr {
	u32 Magic;

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
	struct {
		BOOL IsDriveAwake;
		BOOL IsDriveActive;

		FILE* DrivePhysicalPointer;
		u64 DrivePhysicalSize;
		void* CurrentLoadedChunks[4];
		u32 LoadedChunkSize[4];
		u64 LoadedChunkBaseAddr[4];
		u64 LoadedChunkCpuTick[4];

		char DeviceVendor[16];
		u64 DeviceSerial;
		u64 DeviceVendorId;
		u64 DeviceModelNum;

		u64 CurrentFilePointer;
		u64 SpeculativeSeek;
	}*Drives;
}fdiskctx_t;
extern fdiskctx_t* fdiskctx;

/*
commands:

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

void kbi_setactivedrive(int DriveId);
int  kbi_getdrivecnt(void);
void kbi_drivesleep(int DriveId);
void kbi_driveawake(int DriveId);
u64  kbi_getdrivesize(int DriveId);
BOOL kbi_isdriveready(int DriveId);
u64  kbi_driveread(int DriveId);
void kbi_drivewrite(int DriveId, u64 Data);
u64  kbi_drivegetserial(int DriveId);
void kbi_drivegetvendorsz(int DriveId, u64 Pointer);
u64  kbi_drivegetmodel(int DriveId);
u64  kbi_drivegetvendorid(int DriveId);
void kbi_farseek(int DriveId, u64 SpecPos);
void kbi_skipfpinc(int DriveId);
void kbi_fpincon(int DriveId);
void kbi_fpincoff(int DriveId);
void kbi_driveseek(int DriveId, u64 NewPos);
u64  kbi_drivereadstack(int DriveId);
void kbi_drivewritestack(int DriveId, u64 Data);