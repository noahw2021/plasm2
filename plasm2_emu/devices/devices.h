#pragma once
#include "../basetypes.h"
/*
devices.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void devices_init(void);
void devices_shutdown(void);

u32 devices_register(const char* DeviceName, u32 Serial, u32 Model, u64(*Calls[7])(u32, u64));
void devices_disconnect(u32 DeviceId);

u64 devicesi_statusquery(u32 Device);
void devicesi_sendcommand(u32 Device, u64 Command);
void devicesi_senddata(u32 Device, u64 Command);
u64  devicesi_getdata(u32 Device);
void devicesi_reset(u32 Device);
void devicesi_off(u32 Device);
void devicesi_on(u32 Device);
u32  devicesi_devcount(void);

typedef struct _devicesctx {
	u32 DeviceCount;
}devicesctx_t;
extern devicesctx_t* devicesctx;
