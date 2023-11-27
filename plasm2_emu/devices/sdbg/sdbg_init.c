//
//  sdbg_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "sdbg.h"
#include "../../emu.h"
#include "../../cpu/cpu.h"
#include <stdlib.h>
#include <string.h>

PSDBG_CTX sdbgctx;

#pragma warning(disable: 6011 6387)

void SdbgInit(void) {
	sdbgctx = malloc(sizeof(SDBG_CTX));
	memset(sdbgctx, 0, sizeof(SDBG_CTX));

	sdbgctx->CollectionBufferIn = malloc(sizeof(char) * 256);
	sdbgctx->CollectionBufferOut = malloc(sizeof(char) * 256);
	sdbgctx->Active = 1;
	sdbgctx->DeviceStatus = DEVSTATUS_GOOD;

	return;
}

void SdbgShutdown(void) {
	free(sdbgctx);
}

void SdbgClock(void) {
	if (sdbgctx->LastSend > CpuTimerGetTime()) {
		if (sdbgctx->ReadyOut) {
			printf("%s", sdbgctx->CollectionBufferOut);
			sdbgctx->ReadyOut = 0;
		}
	}
}

void SdbgCollect(void) {
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceType = DEVTYPE_TERMINAL;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceModel = 1;

	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceName, "PLASM2EMU Serialish Debugger");
	strcpy(DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceVendor, "noahw2021");
	DevicesCtx->Devices[DevicesCtx->DeviceCount].DeviceSerial = 4279;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].VendorId = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.Active = 1;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Flags.On = 1;

	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[0] = SdbgStatusQuery;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[1] = SdbgSendCommand;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[2] = SdbgSendData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[3] = SdbgGetData;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[4] = SdbgReset;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[5] = SdbgOff;
	DevicesCtx->Devices[DevicesCtx->DeviceCount].Callbacks[6] = SdbgOn;

	DevicesCtx->DeviceCount++;

	return;
}
