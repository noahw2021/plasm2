//
//  fdisk_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "fdisk.h"
#include <string.h>

void fdisk_collect(void) {
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_FDISKC;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceModel = 1;

	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "PLASM2EMU Fixed Disk Controller");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "noahw2021");
	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 4272;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.Active = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.On = 1;

	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = fdisk_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = fdisk_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = fdisk_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = fdisk_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = fdisk_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = fdisk_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = fdisk_on;

	devicesctx->DeviceCount++;

	return;
}
