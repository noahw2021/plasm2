#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "emu.h"
#include "basetypes.h"
#include "cpu/cpu.h"
#include "devices/devices.h"
#include "devices/kb/kb.h"
#include "devices/video/video.h"
#include "devices/fdisk/fdisk.h"
#include "psin2/psin2.h"
#include "decoder/decoder.h"

#pragma warning(disable: 6308 6387 26451 28182)

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
0x24F0: Base Stack Pointer
0x25F0: End of stack
*/

int main(int argc, char** argv) {
	//fgetc(stdin);
	emu_init();
	psin2_init();
	
	char** FixedDisks = NULL;
	int FixedDiskCount = 0;

	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "-d") || strstr(argv[i], "--debug")) {
			emuctx->DebuggerEnabled = 1;
		}
		if (strstr(argv[i], "-h") || strstr(argv[i], "--help")) {
			printf("%s -d | --debug : Enables disassembler / debugger mode.\n", argv[0]);
			printf("%s -h | --help : Shows this screen.\n", argv[0]);
			printf("\nBy default, PLASM emulator accepts a properly formed 'bios.bin'.\n");

			return 0;
		}
		if (strstr(argv[i], "-f=")) {
			if (!FixedDisks)
				FixedDisks = malloc(sizeof(char*) * (FixedDiskCount + 1));
			else
				FixedDisks = realloc(FixedDisks, (sizeof(char*) * (FixedDiskCount + 1)));

			FixedDisks[FixedDiskCount] = malloc(strlen(argv[i]) + 1);
			strcpy(FixedDisks[FixedDiskCount], argv[i] + 3);
			FixedDiskCount++;
		}
	}
	
	cpu_init();

	memset(i, 0, sizeof(*i));

	i->pti.dvptr = 0x0000;
	i->ip = 0x03A0;
	i->pti.slb = 0x24F0;
	i->pti.spb = 0x25F0;
	i->sp = 0x24F0;

	FILE* Bios = fopen("bios.bin", "rb");
	if (!Bios) {
		printf("[ERR]: Failed to open BIOS!\n");
		return -1;
	}
	fread((byte*)cpuctx->PhysicalMemory + 0x3A0, 4096, 1, Bios); // read the bios into ram

	devices_init();
	devices_collect();

	for (int i = 0; i < FixedDiskCount; i++) {
		fdisk_register(FixedDisks[i]);
		free(FixedDisks[i]);
	}
	if (FixedDisks)
		free(FixedDisks);

	if (emuctx->DebuggerEnabled)
		decoder_init();

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
	
	if (emuctx->DebuggerEnabled)
		decoder_shutdown();

	devices_shutdown();
	cpu_shutdown();
	emu_shutdown();

	printf("CPU Halted.\n");

	return 0;
}