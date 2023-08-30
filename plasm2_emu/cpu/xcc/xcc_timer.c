#include "../cpu.h"

u64 cput_gettime(void) {
	return (cpuctx->SystemSeconds - cpuctx->SystemBoot) & 0x0FFFFFFFFFFFFFFF;
}