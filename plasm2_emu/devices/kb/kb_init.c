#include "kb.h"
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include <string.h>
#include <stdlib.h>
/*
kb_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/
u64 KeysDown[4];
kbctx_t* kbctx;

#pragma warning(disable: 6387)

void kb_clock(void) {
	if (kbctx->NotifyUp) {
		mmu_push(kbctx->NotifyUp);
		cpui_inst_int(kbctx->KeyUp);
		kbctx->NotifyUp = 0;
	}
	if (kbctx->NotifyDown) {
		mmu_push(kbctx->NotifyDown);
		cpui_inst_int(kbctx->KeyDown);
		kbctx->NotifyDown = 0;
	}
}

void kb_init(void) {
	kbctx = malloc(sizeof(kbctx_t));
	memset(kbctx, 0, sizeof(kbctx_t));

	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceModel, "Standard ENUS");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "Keyboard");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "plasm2 Vendor");

	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 0x1000200030004000;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_KB;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 0x0010020030040567;
	
	devicesctx->DeviceCount++;

	return;
}

void kb_shutdown(void) {
	free(kbctx);
	return;
}