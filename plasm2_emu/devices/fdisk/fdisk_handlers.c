//
//  fdisk_handlers.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"

WORD64 FdiskStatusQuery(WORD32 Device, WORD64 NullArg) {
	WORD64 Return = FdiskCtx->CurrentStatus;
	FdiskCtx->CurrentStatus &= ~(DEVSTATUS_RSN_CLEAR);
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

WORD64 FdiskSendCommand(WORD32 Device, WORD64 Command) {
	switch (Command) {
	case 0x00:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	case 0x01:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiGetDriveCount();
		break;
	case 0x02:
		FdiskiDriveSleep(FdiskCtx->CurrentDrive);
		break;
	case 0x03:
		FdiskiDriveAwake(FdiskCtx->CurrentDrive);
		break;
	case 0x04:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiGetDriveSize(FdiskCtx->CurrentDrive);
		break;
	case 0x05:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiIsDriveReady(FdiskCtx->CurrentDrive);
		break;
	case 0x06:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiDriveRead(FdiskCtx->CurrentDrive);
		break;
	case 0x07:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	case 0x08:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiGetDriveSerial(FdiskCtx->CurrentDrive);
		break;
	case 0x09:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	case 0x0A:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiGetDriveModel(FdiskCtx->CurrentDrive);
		break;
	case 0x0B:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiGetDriveVendorId(FdiskCtx->CurrentDrive);
		break;
	case 0x0C:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	case 0x0D:
		FdiskiSkipFpIncrement(FdiskCtx->CurrentDrive);
		break;
	case 0x0E:
		FdiskiDisableFpIncrement(FdiskCtx->CurrentCommand);
		break;
	case 0x0F:
		FdiskiEnableFpIncrement(FdiskCtx->CurrentCommand);
		break;
	case 0x10:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	case 0x11:
		FdiskCtx->RecvData = 1;
		FdiskCtx->OutgoingData = FdiskiDriveReadStack(FdiskCtx->CurrentCommand);
		break;
	case 0x12:
		FdiskCtx->CurrentCommand = (BYTE)Command;
		FdiskCtx->ExpectingData = 1;
		break;
	default:
		FdiskCtx->CurrentStatus |= DEVSTATUS_RSN_INVCMD;
		break;
	}
	return 0;
}

WORD64 FdiskSendData(WORD32 Device, WORD64 Data) {
	if (FdiskCtx->ExpectingData) {
		switch (FdiskCtx->CurrentCommand) {
		case 0x00:
			FdiskiSetActiveDrive((int)Data);
			break;
		case 0x07:
			FdiskiDriveWrite(FdiskCtx->CurrentDrive, Data);
			break;
		case 0x09:
			FdiskiGetDriveVendorString(FdiskCtx->CurrentDrive, Data);
			break;
		case 0x0C:
			FdiskiFarSeek(FdiskCtx->CurrentDrive, Data);
			break;
		case 0x10:
			FdiskiDriveSeek(FdiskCtx->CurrentCommand, Data);
			break;
		case 0x12:
			FdiskiDriveWriteStack(FdiskCtx->CurrentCommand, Data);
			break;
		}
		FdiskCtx->ExpectingData = 0;
	}
	return 0;
}

WORD64  FdiskGetData(WORD32 Device, WORD64 NullArg) {
	if (FdiskCtx->RecvData) {
		FdiskCtx->RecvData = 0; 
		return FdiskCtx->OutgoingData;
	}
	return 0;
}

WORD64 FdiskReset(WORD32 Device, WORD64 NullArg) {
	return 0;
}

WORD64 FdiskOff(WORD32 Device, WORD64 NullArg) {
	for (int i = 0; i < FdiskCtx->DriveCount; i++)
		FdiskiDriveSleep(i);
	return 0;
}

WORD64 FdiskOn(WORD32 Device, WORD64 NullArg) {
	for (int i = 0; i < FdiskCtx->DriveCount; i++)
		FdiskiDriveAwake(i);
	return 0;
}
