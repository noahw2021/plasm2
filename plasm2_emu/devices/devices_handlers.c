#include "devices.h"
#include "../cpu/cpu.h"
#include <string.h>
/*
devices_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

u64 devicesi_statusquery(u64 Device) {
	if (Device >= devicesctx->DeviceCount)
		return 0;
	return devicesctx->Devices[Device].Callbacks[0](Device, 0);
}

void devicesi_sendcommand(u64 Device, u64 Command) {
	if (Device >= devicesctx->DeviceCount)
		return;
	devicesctx->Devices[Device].Callbacks[1](Device, Command);
}

void devicesi_senddata(u64 Device, u64 Command) {
	if (Device >= devicesctx->DeviceCount)
		return;
	devicesctx->Devices[Device].Callbacks[2](Device, Command);
}

u64  devicesi_getdata(u64 Device) {
	if (Device >= devicesctx->DeviceCount)
		return 0;
	return devicesctx->Devices[Device].Callbacks[3](Device, 0);
}

void devicesi_reset(u64 Device) {
	if (Device >= devicesctx->DeviceCount)
		return;
	devicesctx->Devices[Device].Callbacks[4](Device, 0);
}

void devicesi_off(u64 Device) {
	if (Device >= devicesctx->DeviceCount)
		return;
	devicesctx->Devices[Device].Callbacks[5](Device, 0);
}

void devicesi_on(u64 Device) {
	if (Device >= devicesctx->DeviceCount)
		return;
	devicesctx->Devices[Device].Callbacks[6](Device, 0);
}

u64  devicesi_devcount(void) {
	return devicesctx->DeviceCount;
}