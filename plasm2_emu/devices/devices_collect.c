#include "devices.h"
#include "../cpu/cpu.h"
#include <string.h>

// specific devices
#include "kb/kb.h"
#include "video/video.h"

/*
devices_collect.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void devices_collect(void) {
	video_collect();
	kb_collect();

	return;
}