#include "fdisk.h"
/*
fdisk_handlers.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

u64 fdisk_statusquery(u32 Device, u64 NullArg) {
	u64 Return = fdiskctx->CurrentStatus;
	fdiskctx->CurrentStatus &= ~(DEVSTATUS_RSN_CLEAR);
	return Return;
}

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

u64 fdisk_sendcommand(u32 Device, u64 Command) {
	switch (Command) {
	case 0x00:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	case 0x01:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_getdrivecnt();
		break;
	case 0x02:
		fdiski_drivesleep(fdiskctx->CurrentDrive);
		break;
	case 0x03:
		fdiski_driveawake(fdiskctx->CurrentDrive);
		break;
	case 0x04:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_getdrivesize(fdiskctx->CurrentDrive);
		break;
	case 0x05:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_isdriveready(fdiskctx->CurrentDrive);
		break;
	case 0x06:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_driveread(fdiskctx->CurrentDrive);
		break;
	case 0x07:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	case 0x08:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_drivegetserial(fdiskctx->CurrentDrive);
		break;
	case 0x09:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	case 0x0A:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_drivegetmodel(fdiskctx->CurrentDrive);
		break;
	case 0x0B:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_drivegetvendorid(fdiskctx->CurrentDrive);
		break;
	case 0x0C:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	case 0x0D:
		fdiski_skipfpinc(fdiskctx->CurrentDrive);
		break;
	case 0x0E:
		fdiski_fpincoff(fdiskctx->CurrentCommand);
		break;
	case 0x0F:
		fdiski_fpincon(fdiskctx->CurrentCommand);
		break;
	case 0x10:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	case 0x11:
		fdiskctx->RecvData = 1;
		fdiskctx->OutgoingData = fdiski_drivereadstack(fdiskctx->CurrentCommand);
		break;
	case 0x12:
		fdiskctx->CurrentCommand = (byte)Command;
		fdiskctx->ExpectingData = 1;
		break;
	default:
		fdiskctx->CurrentStatus |= DEVSTATUS_RSN_INVCMD;
		break;
	}
	return 0;
}

u64 fdisk_senddata(u32 Device, u64 Data) {
	if (fdiskctx->ExpectingData) {
		switch (fdiskctx->CurrentCommand) {
		case 0x00:
			fdiski_setactivedrive((int)Data);
			break;
		case 0x07:
			fdiski_drivewrite(fdiskctx->CurrentDrive, Data);
			break;
		case 0x09:
			fdiski_drivegetvendorsz(fdiskctx->CurrentDrive, Data);
			break;
		case 0x0C:
			fdiski_farseek(fdiskctx->CurrentDrive, Data);
			break;
		case 0x10:
			fdiski_driveseek(fdiskctx->CurrentCommand, Data);
			break;
		case 0x12:
			fdiski_drivewritestack(fdiskctx->CurrentCommand, Data);
			break;
		}
		fdiskctx->ExpectingData = 0;
	}
	return 0;
}

u64  fdisk_getdata(u32 Device, u64 NullArg) {
	if (fdiskctx->RecvData) {
		fdiskctx->RecvData = 0; 
		return fdiskctx->OutgoingData;
	}
	return 0;
}

u64 fdisk_reset(u32 Device, u64 NullArg) {
	return 0;
}

u64 fdisk_off(u32 Device, u64 NullArg) {
	for (int i = 0; i < fdiskctx->DriveCount; i++)
		fdiski_drivesleep(i);
	return 0;
}

u64 fdisk_on(u32 Device, u64 NullArg) {
	for (int i = 0; i < fdiskctx->DriveCount; i++)
		fdiski_driveawake(i);
	return 0;
}