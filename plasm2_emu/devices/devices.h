//
//  devices.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _devices_h
#define _devices_h

#include <stddef.h>
#include "../basetypes.h"

void DevicesInit(void);
void DevicesShutdown(void);
void DevicesCollect(void);

WORD64 DevicesiStatusQuery(WORD32 Device);
void DevicesiSendCommand(WORD32 Device, WORD64 Command);
void DevicesiSendData(WORD32 Device, WORD64 Command);
WORD64  DevicesiGetData(WORD32 Device);
void DevicesiReset(WORD32 Device);
void DevicesiOff(WORD32 Device);
void DevicesiOn(WORD32 Device);
WORD64  DevicesiDevCount(void);

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

typedef union _CPU_DEVICE_FLAGS {
    WORD32 FlagsRaw;
    struct {
        WORD32 Active : 1;
        WORD32 On : 1;
        WORD32 Sleep : 1;
        WORD32 Hotswappable : 1;
    };
}CPU_DEVICE_FLAGS, *PCPU_DEVICE_FLAGS;

typedef struct _CPU_DEVICE {
    int DeviceType;
    char DeviceName[32];
    char DeviceVendor[32];
    WORD64 VendorId;
    WORD64 DeviceModel;
    WORD64 DeviceSerial;
    WORD64(*Callbacks[7])(WORD32 DeviceId, WORD64 Argument);
    CPU_DEVICE_FLAGS Flags;
}CPU_DEVICE, *PCPU_DEVICE;

typedef struct _DEVICES_CTX {
	WORD64 DeviceCount;
    PCPU_DEVICE Devices;
}DEVICES_CTX, *PDEVICES_CTX;

extern PDEVICES_CTX DevicesCtx;

#endif
