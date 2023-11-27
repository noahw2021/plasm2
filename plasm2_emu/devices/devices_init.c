//
//  devices_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "devices.h"
#include "../cpu/cpu.h"
#include "kb/kb.h"
#include "video/video.h"
#include "fdisk/fdisk.h"
#include <string.h>
#include <stdlib.h>
#include <SDL.h>

PDEVICES_CTX DevicesCtx;

#pragma warning(disable: 6011 6387)

extern _bool ShouldStartVideo;
extern _bool VideoStarted;

void DevicesInit(void) {
	DevicesCtx = malloc(sizeof(DEVICES_CTX));
	memset(DevicesCtx, 0, sizeof(DEVICES_CTX));
	
	DevicesCtx->DeviceCount = 0;
	DevicesCtx->Devices = (void*)cpuctx->PhysicalMemory; // PM usage good: (reason: internal usage only)

    ShouldStartVideo = TRUE;
    while (!VideoStarted)
        SDL_Delay(100);
    
	KbInit();
	FdiskInit();
}
void DevicesShutdown(void) {
	KbShutdown();
	VideoShutdown();
	return;
}
