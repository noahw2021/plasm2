#include "devices.h"
#include "../cpu/cpu.h"
#include "kb/kb.h"
#include "video/video.h"
#include <string.h>
/*
devices_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/
devicesctx_t* devicesctx;

void devices_init(void) {
	devicesctx = malloc(sizeof(devicesctx_t));
	memset(devicesctx, 0, sizeof(devicesctx_t));
	
	devicesctx->DeviceCount = 2;
	devicesctx->Devices = cpuctx->PhysicalMemory;

	kb_init();
	video_init();
}
void devices_shutdown(void) {

}