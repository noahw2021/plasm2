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

devicesctx_t* devicesctx;

#pragma warning(disable: 6011 6387)

extern _bool ShouldStartVideo;

void devices_init(void) {
	devicesctx = malloc(sizeof(devicesctx_t));
	memset(devicesctx, 0, sizeof(devicesctx_t));
	
	devicesctx->DeviceCount = 0;
	devicesctx->Devices = (void*)cpuctx->PhysicalMemory; // PM usage good: (reason: internal usage only)

    ShouldStartVideo = TRUE;
	kb_init();
	fdisk_init();
}
void devices_shutdown(void) {
	kb_shutdown();
	video_shutdown();
	return;
}
