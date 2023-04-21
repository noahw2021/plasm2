#include "../devices.h"
#include "kb.h"
#include "../../cpu/cpu.h"
#include <string.h>
/*
kb_collect.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void kb_collect(void) {
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_KB;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceModel = 1;
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "PLASM2EMU Emulated I/O Keyboard");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "noahw2021");
	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 4271;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.Active = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.On = 1;

	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = kb_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = kb_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = kb_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = kb_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = kb_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = kb_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = kb_on;

	devicesctx->DeviceCount++;

	return;
}