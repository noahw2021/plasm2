//
//  devices.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include <stddef.h>
#include "../basetypes.h"

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
u64  devicesi_devcount(void);

#define DEVTYPE_KB			0x10000001
#define DEVTYPE_MOUSE		0x10000002
#define DEVTYPE_CPU			0x10000003
#define DEVTYPE_TERMINAL	0x10000004
#define DEVTYPE_VIDEO       0x10000005
#define DEVTYPE_FDISKC      0x10000006
#define DEVTYPE_RDISKC      0x10000007
#define DEVTYPE_NETWORK     0x10000008

#define DEVSTATUS_GOOD       0x1100100A
#define DEVSTATUS_FAIL       0x1100100B
#define DEVSTATUS_WARN       0x1100100C
#define DEVSTAUTS_INVL       0x11002000

#define DEVSTATUS_RSN_FIRST  0x00000000
#define DEVSTATUS_RSN_INVCMD 0x00010000
#define DEVSTATUS_RSN_CLEAR  0x00FF0000

typedef struct _devicesctx {
	u64 DeviceCount;
	struct {
		int DeviceType;
		char DeviceName[32];
		char DeviceVendor[32];
		u64 VendorId;
		u64 DeviceModel;
		u64 DeviceSerial;
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

