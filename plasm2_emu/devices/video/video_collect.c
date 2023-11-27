//
//  video_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "video.h"
#include "../../cpu/cpu.h"
#include <string.h>

void VideoCollect(void) {
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceType = DEVTYPE_VIDEO;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceModel = 1;
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceName, "PLASM2EMU Emulated Video Driver");
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceVendor, "noahw2021");
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceSerial = 4270;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].VendorId = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.Active = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.On = 1;
	
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[0] = VideoStatusQuery;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[1] = VideoSendCommand;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[2] = VideoSendData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[3] = VideoGetData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[4] = VideoReset;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[5] = VideoOff;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[6] = VideoOn;

	DevicesCtx->DeviceCount++;

	return;
}
