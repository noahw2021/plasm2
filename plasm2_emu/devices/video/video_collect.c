#include "../devices.h"
#include "video.h"
#include "../../cpu/cpu.h"
#include <string.h>
/*
video_collect.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void video_collect(void) {
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_VIDEO;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceModel = 1;
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "PLASM2EMU Emulated Video Driver");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "noahw2021");
	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 4270;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.Active = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.On = 1;
	
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = video_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = video_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = video_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = video_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = video_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = video_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = video_on;

	devicesctx->DeviceCount++;

	return;
}