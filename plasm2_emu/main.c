#include <stdio.h>
#include "basetypes.h"
#include "cpu/cpu.h"
#include "devices/devices.h"
/*
main.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

/*
Starting physical memory map:

0x0000: Devices 0-7 location
0x03A0: Instruction pointer, BIOS mapping point
0x13AA: End of BIOS
0x13F0: BIOS maps 4KiB of HDD0 here
*/

int main(int argc, char** argv) {
	cpu_init();

	i->pti.dvptr = 0x000;
	i->ip = 0x3A0;

	FILE* Bios = fopen("bios.bin", "rb");
	fread(cpuctx->PhysicalMemory + 0x3A0, 4096, 1, Bios); // read the bios into ram

	devices_init();
	devices_collect();

	while (1) {
		cpu_clock();
		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}

	return 0;
}