/*
dvmgr_init.c
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "dvmgr.h"

extern int SerialDevice; // txto's serial device

void DiInit(void) {
    int DeviceCnt = __dgc();
    
    for (int i = 0; i < DeviceCnt; i++) {
        PRDEVICE RDevice = *(RDEVICE**)(0x0000 + (i * sizeof(RDEVICE)));
        if (RDevice->DeviceType == 0x10000004) // Serial
            SerialDevice = i;
    }

    return;
}