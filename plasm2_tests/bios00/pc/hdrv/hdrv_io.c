/*
hdrv_io.c
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "hdrv.h"
#include "../dvmgr/dvmgr.h"

void HddReadBlock(int Drive, u64 BasePtr, u64 Size, void* Destination, u64 MaxSize) {
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

LetsGo:
    __dsc(ADevId, 0x00); // SetActiveDrive
    __dsd(ADevId, Drive);
    __dsc(ADevId, 0x0F); // DriveEnableFpIncrement

    __dsc(ADevId, 0x10); // DriveSeek
    __dsd(ADevId, BasePtr);

    u64* As64s = (u64*)Destination;

    for (int i = 0; i < (Size / 8); i++) {
        __dsc(ADevId, 0x06); // DriveRead
        As64s[i] = __dgd(ADevId);
    }

    if ((Size % 8) != 0x00) {
        __dsc(ADevId, 0x06); // DriveRead
        u64 ReadData = __dgd(ADevId);
        for (int i = 0; i < (Size % 8); i++) {
            *(byte*)(Destination + (Size - (Size % 8) + i)) = ReadData & (0xFF << (i * 8) >> (i * 8);
        }
    }

    __dsc(ADevId, 0x10); // DriveSeek
    __dsd(ADevId, Size);
}

void HddWriteBlock(int Drive, u64 BasePtr, u64 Size, void* Buffer) {

}