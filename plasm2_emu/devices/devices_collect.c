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


void DevicesCollect(void) {
	VideoCollect();
	KbCollect();
	FdiskCollect();

	return;
}
