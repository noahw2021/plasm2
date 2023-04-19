#include "devices.h"
#include "../cpu/cpu.h"
#include <string.h>
/*
devices_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

u32 devices_register(const char* DeviceName, u32 Serial, u32 Model, u64(*Calls[7])(u32, u64)) {

}

void devices_disconnect(u32 DeviceId) {

}