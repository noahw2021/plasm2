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
    PCPU_DEVICE KbDevice = &DevicesCtx->Devices[DevicesCtx->DeviceCount];
    DevicesCtx->DeviceCount++;
    
	KbDevice->DeviceType = DEVTYPE_KB;
	KbDevice->DeviceModel = 1;
	strcpy(KbDevice->DeviceName, "PLASM2EMU Emulated I/O Keyboard");
	strcpy(KbDevice->DeviceVendor, "noahw2021");
	KbDevice->DeviceSerial = 4271;
	KbDevice->VendorId = 1;
	KbDevice->Flags.Active = 1;
	KbDevice->Flags.On = 1;

	KbDevice->Callbacks[0] = KbStatusQuery;
	KbDevice->Callbacks[1] = KbSendCommand;
	KbDevice->Callbacks[2] = KbSendData;
	KbDevice->Callbacks[3] = KbGetData;
	KbDevice->Callbacks[4] = KbReset;
	KbDevice->Callbacks[5] = KbOff;
	KbDevice->Callbacks[6] = KbOn;
	return;
}
