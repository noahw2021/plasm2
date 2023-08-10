/*
hdrv_init.c
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "hdrv.h"
#include "../dvmgr/dvmgr.h"

void HddInit(void) {
    return;
}

int HddGetCount(void) {
    int DeviceCnt = __dgc();
    PRDEVICE ADevice = (void*)0;
    int ADevId = 0;

    for (int i = 0; i < DeviceCnt; i++) {
        PRDEVICE RDevice = *(RDEVICE**)(0x00 + (i * sizeof(RDEVICE)));
        if (RDevice->DeviceType == 0x10000006) { // Fixed Disk Controller
            ADevice = RDevice;
            ADevId = i;
            goto LetsGo;
        }
    }

    return 0;
LetsGo:
    __dsc(ADevId, 0x01); // GetDriveCount
    int DriveCnt = __dgd(ADevId);

    return DriveCnt;
}