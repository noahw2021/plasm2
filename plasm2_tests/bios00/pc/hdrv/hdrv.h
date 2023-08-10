#pragma once
/*
hdrv.h
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "../plasm2.h"

#define BOOTIMG_MAGIC 0xC0010017

typedef struct _BOOTIMG {
    u32 Magic;
    u32 RequestedAddress;

    byte MapData[4096];
}BOOTIMG, *PBOOTIMAGE;

void HddInit(void);
int HddGetCount(void);

void HddReadBlock(int Drive, u64 BasePtr, u64 Size, void* Destination, u64 MaxSize);
void HddWriteBlock(int Drive, u64 BasePtr, u64 Size, void* Buffer);

// OS Loader
void OslInit(void);
void OslGo(void);