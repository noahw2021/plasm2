#include <stdio.h>
#include <string.h>
#include "basetypes.h"
#include "cpu/cpu.h"
#include "devices/devices.h"
#include "devices/kb/kb.h"
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
0x23F0: keyboard bitmap
0x24F0: TBD (end of kb bitmap)
*/

int main(int argc, char** argv) {
	fgetc(stdin);
	cpu_init();

	memset(i, 0, sizeof(*i));

	i->pti.dvptr = 0x000;
	i->ip = 0x3A0;

	FILE* Bios = fopen("bios.bin", "rb");
	if (!Bios) {
		printf("[ERR]: Failed to open BIOS!\n");
		return -1;
	}
	fread((byte*)cpuctx->PhysicalMemory + 0x3A0, 4096, 1, Bios); // read the bios into ram

	devices_init();
	devices_collect();

	while (1) {
		kb_clock();
		cpu_clock();
		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}

	printf("CPU Halted.\n");

	return 0;
}