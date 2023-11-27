//
//  devices_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "devices.h"
#include "../cpu/cpu.h"
#include <string.h>

WORD64 DevicesiStatusQuery(WORD32 Device) {
	if (Device >= DevicesCtx->DeviceCount)
		return 0;
	return DevicesCtx->Devices[Device].Callbacks[0](Device, 0);
}

void DevicesiSendCommand(WORD32 Device, WORD64 Command) {
	if (Device >= DevicesCtx->DeviceCount)
		return;
	DevicesCtx->Devices[Device].Callbacks[1](Device, Command);
}

void DevicesiSendData(WORD32 Device, WORD64 Command) {
	if (Device >= DevicesCtx->DeviceCount)
		return;
	DevicesCtx->Devices[Device].Callbacks[2](Device, Command);
}

WORD64  DevicesiGetData(WORD32 Device) {
	if (Device >= DevicesCtx->DeviceCount)
		return 0;
	return DevicesCtx->Devices[Device].Callbacks[3](Device, 0);
}

void DevicesiReset(WORD32 Device) {
	if (Device >= DevicesCtx->DeviceCount)
		return;
	DevicesCtx->Devices[Device].Callbacks[4](Device, 0);
}

void DevicesiOff(WORD32 Device) {
	if (Device >= DevicesCtx->DeviceCount)
		return;
	DevicesCtx->Devices[Device].Callbacks[5](Device, 0);
}

void DevicesiOn(WORD32 Device) {
	if (Device >= DevicesCtx->DeviceCount)
		return;
	DevicesCtx->Devices[Device].Callbacks[6](Device, 0);
}

WORD64  DevicesiDevCount(void) {
	return DevicesCtx->DeviceCount;
}
