//
//  fdisk_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"
#include <string.h>

void FdiskCollect(void) {
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceType = DEVTYPE_FDISKC;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceModel = 1;

	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceName, "PLASM2EMU Fixed Disk Controller");
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceVendor, "noahw2021");
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceSerial = 4272;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].VendorId = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.Active = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.On = 1;

	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[0] = FdiskStatusQuery;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[1] = FdiskSendCommand;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[2] = FdiskSendData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[3] = FdiskGetData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[4] = FdiskReset;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[5] = FdiskOff;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[6] = FdiskOn;

	DevicesCtx->DeviceCount++;

	return;
}
