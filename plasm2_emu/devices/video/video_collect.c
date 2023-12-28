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
	
    PCPU_DEVICE VideoDevice =
        &DevicesCtx->Devices[DevicesCtx->DeviceCount];
    DevicesCtx->DeviceCount++;
    
    VideoDevice->DeviceType = DEVTYPE_VIDEO;
	VideoDevice->DeviceModel = 1;
	strcpy(VideoDevice->DeviceName, "PLASM2EMU Emulated Video Driver");
	strcpy(VideoDevice->DeviceVendor, "noahw2021");
	VideoDevice->DeviceSerial = 4270;
	VideoDevice->VendorId = 1;
	VideoDevice->Flags.Active = 1;
	VideoDevice->Flags.On = 1;
	
	VideoDevice->Callbacks[0] = VideoStatusQuery;
	VideoDevice->Callbacks[1] = VideoSendCommand;
	VideoDevice->Callbacks[2] = VideoSendData;
	VideoDevice->Callbacks[3] = VideoGetData;
	VideoDevice->Callbacks[4] = VideoReset;
	VideoDevice->Callbacks[5] = VideoOff;
    VideoDevice->Callbacks[6] = VideoOn;
	return;
}
