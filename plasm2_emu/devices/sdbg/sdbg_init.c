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
    
    PCPU_DEVICE SdbDevice = &DevicesCtx->Devices[DevicesCtx->DeviceCount];
    DevicesCtx->DeviceCount++;
    
	SdbDevice->DeviceType = DEVTYPE_TERMINAL;
	SdbDevice->DeviceModel = 1;

	strcpy(SdbDevice->DeviceName, "PLASM2EMU Serialish Debugger");
	strcpy(SdbDevice->DeviceVendor, "noahw2021");
	SdbDevice->DeviceSerial = 4279;
	SdbDevice->VendorId = 1;
	SdbDevice->Flags.Active = 1;
	SdbDevice->Flags.On = 1;

	SdbDevice->Callbacks[0] = SdbgStatusQuery;
	SdbDevice->Callbacks[1] = SdbgSendCommand;
	SdbDevice->Callbacks[2] = SdbgSendData;
	SdbDevice->Callbacks[3] = SdbgGetData;
	SdbDevice->Callbacks[4] = SdbgReset;
	SdbDevice->Callbacks[5] = SdbgOff;
	SdbDevice->Callbacks[6] = SdbgOn;
	return;
}
