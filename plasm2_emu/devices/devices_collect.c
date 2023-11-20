//
//  devices_collect.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "devices.h"
#include "../cpu/cpu.h"
#include <string.h>

// specific devices
#include "kb/kb.h"
#include "video/video.h"
#include "fdisk/fdisk.h"

/*
devices_collect.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void devices_collect(void) {
	video_collect();
	kb_collect();
	fdisk_collect();

	return;
}
