#pragma once
/*
dvmgr.h
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "../plasm2.h"

typedef struct _RDEVICE {
    int DeviceType;
    char DeviceName[32];
    char DeviceVendor[32];
    u64 VendorId;
    u64 DeviceModel;
    u64 DeviceSerial;
    void* Callbacks[7];
    union {
        u32 FlagsRaw;
        struct {
            byte Active : 1;
            byte On : 1;
            byte Sleep : 1;
            byte Hotswappable : 1;
        };
    }Flags;
}RDEVICE, *PRDEVICE;

void DiInit(void);