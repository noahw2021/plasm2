#include <stdio.h>
#include <string.h>
#include "emu.h"
#include "basetypes.h"
#include "cpu/cpu.h"
#include "devices/devices.h"
#include "devices/kb/kb.h"
#include "devices/video/video.h"
/*
main.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

/*
Starting physical memory map:

0x0000: Devices 0-6 location (144 per device struct)
0x0360: End of devices, padding area (to be used)
0x03A0: Instruction pointer, BIOS mapping point
0x13AA: End of BIOS
0x13F0: BIOS (should) map 4KiB of HDD0 here
0x23F0: keyboard bitmap
0x24F0: TBD (end of kb bitmap)
*/

int main(int argc, char** argv) {
	//fgetc(stdin);
	emu_init();
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

	char TheHaltReason[256];

	while (1) {
		if (emu_aufhoren(TheHaltReason)) {
			printf("Emergancy CPU Stop: Virtual Execution Error.\n");
			printf("%s\n", TheHaltReason);
			break;
		}
		kb_clock();
		video_clock();
		cpu_clock();
		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}
	
	devices_shutdown();
	cpu_shutdown();
	emu_shutdown();

	printf("CPU Halted.\n");

	return 0;
}