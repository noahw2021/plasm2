#include "../cpu.h"

u64 cput_gettime(void) {
	return (cpuctx->LastClockTime - cpuctx->SystemBoot); // well yknow
}