#include "sdbg.h"
#include "../emu.h"
#include "../cpu/cpu.h"
#include <stdlib.h>
#include <string.h>
/*
sdbg_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

sdbgctx_t* sdbgctx;


void sdbg_init(void) {
	sdbgctx = malloc(sizeof(sdbgctx_t));
	memset(sdbgctx, 0, sizeof(sdbgctx_t));

	sdbgctx->CollectionBufferIn = malloc(sizeof(char) * 256);
	sdbgctx->CollectionBufferOut = malloc(sizeof(char) * 256);
	sdbgctx->Active = 1;
	sdbgctx->DeviceStatus = DEVSTATUS_GOOD;

	return;
}

void sdbg_shutdown(void) {
	free(sdbgctx);
}

void sdbg_clock(void) {
	if (sdbgctx->LastSend > cput_gettime()) {
		if (sdbgctx->ReadyOut) {
			printf("%s", sdbgctx->CollectionBufferOut);
			sdbgctx->ReadyOut = 0;
		}
	}
}

void sdbg_collect(void) {
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_FDISKC;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceModel = 1;
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "PLASM2EMU Serialish Debugger");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "noahw2021");
	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 4279;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.Active = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.On = 1;

	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = sdbg_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = sdbg_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = sdbg_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = sdbg_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = sdbg_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = sdbg_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = sdbg_on;

	devicesctx->DeviceCount++;

	return;
}