//
//  fdisk_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"
#include <string.h>

void FdiskCollect(void) {
    PCPU_DEVICE FdiskDevice = &DevicesCtx->Devices[DevicesCtx->DeviceCount];
    DevicesCtx->DeviceCount++;
    
	FdiskDevice->DeviceType = DEVTYPE_FDISKC;
	FdiskDevice->DeviceModel = 1;

	strcpy(FdiskDevice->DeviceName, "PLASM2EMU Fixed Disk Controller");
	strcpy(FdiskDevice->DeviceVendor, "noahw2021");
	FdiskDevice->DeviceSerial = 4272;
	FdiskDevice->VendorId = 1;
	FdiskDevice->Flags.Active = 1;
	FdiskDevice->Flags.On = 1;

	FdiskDevice->Callbacks[0] = FdiskStatusQuery;
	FdiskDevice->Callbacks[1] = FdiskSendCommand;
	FdiskDevice->Callbacks[2] = FdiskSendData;
	FdiskDevice->Callbacks[3] = FdiskGetData;
	FdiskDevice->Callbacks[4] = FdiskReset;
	FdiskDevice->Callbacks[5] = FdiskOff;
	FdiskDevice->Callbacks[6] = FdiskOn;
	return;
}
