//
//  kb_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "kb.h"
#include "../../cpu/cpu.h"
#include <string.h>

void KbCollect(void) {
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceType = DEVTYPE_KB;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceModel = 1;
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceName, "PLASM2EMU Emulated I/O Keyboard");
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceVendor, "noahw2021");
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceSerial = 4271;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].VendorId = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.Active = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.On = 1;

	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[0] = KbStatusQuery;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[1] = KbSendCommand;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[2] = KbSendData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[3] = KbGetData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[4] = KbReset;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[5] = KbOff;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[6] = KbOn;

	DevicesCtx->DeviceCount++;

	return;
}
