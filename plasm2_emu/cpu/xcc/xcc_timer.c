//
//  xcc_timer.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"

u64 cput_gettime(void) {
	return (cpuctx->SystemSeconds - cpuctx->SystemBoot) & 0x0FFFFFFFFFFFFFFF;
}
