#pragma once
#include <stddef.h>
#include "../basetypes.h"
/*
devices.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void devices_init(void);
void devices_shutdown(void);
void devices_collect(void);

u64 devicesi_statusquery(u32 Device);
void devicesi_sendcommand(u32 Device, u64 Command);
void devicesi_senddata(u32 Device, u64 Command);
u64  devicesi_getdata(u32 Device);
void devicesi_reset(u32 Device);
void devicesi_off(u32 Device);
void devicesi_on(u32 Device);
u32  devicesi_devcount(void);

#define DEVTYPE_KB			0x10000001
#define DEVTYPE_MOUSE		0x10000002
#define DEVTYPE_CPU			0x10000003
#define DEVTYPE_TERMINAL	0x10000004
#define DEVTYPE_VIDEO       0x10000005

#define DEVSTATUS_GOOD 0x1100100A
#define DEVSTATUS_FAIL 0x1100100B
#define DEVSTATUS_WARN 0x1100100C
#define DEVSTAUTS_INVL 0x11002000

typedef struct _devicesctx {
	u32 DeviceCount;
	struct {
		int DeviceType;
		char DeviceName[32];
		char DeviceVendor[32];
		u32 VendorId;
		u32 DeviceModel;
		u32 DeviceSerial;
		u64(*Callbacks[7])(u32 DeviceId, u64 Argument);
		union {
			u32 FlagsRaw;
			struct {
				byte Active : 1;
				byte On : 1;
				byte Sleep : 1;
				byte Hotswappable : 1;
			};
		}Flags;
	}*Devices;
}devicesctx_t;
extern devicesctx_t* devicesctx;

