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
#include "tools/tools.h"
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

int __t_argc;
char** __t_argv;
PPLASM2_CTX i;

int main(int argc, char** argv) {
	FILE* a = freopen("rstd", "w", stdout);

	emu_init();
	psin2_init();
	
	char** FixedDisks = NULL;
	int FixedDiskCount = 0;

	for (int i = 1; i < argc; i++) {
		if (strstr(argv[i], "-d") || strstr(argv[i], "--debug")) {
			emuctx->DebuggerEnabled = 1;
		}
		if (strstr(argv[i], "-h") || strstr(argv[i], "--help")) {
			printf("PLASM2Emu: Help & Usage\n\n");
			
			printf("General Switches: \n\n");
			printf("%s -f=drive.pff : Mounts a fixed disk. Allows multiple.\n", argv[0]);
			
			printf("Alt Function Switches: \n\n");
			printf("%s -t | --tools : Loads PLASM2Emu toolkit.\n", argv[0]);

			printf("Misc Switches: (General Function Only)\n\n");
			printf("%s -d | --debug : Enables disassembler / debugger mode.\n", argv[0]);
			printf("%s -h | --help : Shows this screen.\n", argv[0]);
            
			printf("\nBy default, PLASM2Emu accepts a properly formed 'bios.bin'.\n");
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
		if (strstr(argv[i], "-t") || strstr(argv[i], "--tools")) {
            __t_argc = argc;
            __t_argv = argv;
			tools_main();
			return 0;
		}
	}
	
    i = malloc(sizeof(PLASM2_CTX));
	memset(i, 0, sizeof(*i));

    cpu_init();
    
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
	// pm usage good (reason: internal use only)
	fread((byte*)cpuctx->PhysicalMemory + 0x3A0, 4096, 1, Bios); // read the bios into ram

	devices_init();
	devices_collect();// PM usage good (reason: comes from trust)

	for (int i = 0; i < FixedDiskCount; i++) {
		if (!fdisk_register(FixedDisks[i])) {
			printf("[ERR]: Failed to open fixed disk '%s'!\n", FixedDisks[i]);
			emu_register_fatal("Could not obtain drive.");
		}
		free(FixedDisks[i]);
	}
	if (FixedDisks)
		free(FixedDisks);

	if (emuctx->DebuggerEnabled)
		decoder_init();

	char TheHaltReason[256];

	time_t Startup, Startdown;
	Startup = time(NULL);
	u64 ClockCnt = 0;

	while (1) {
		if (emu_aufhoren(TheHaltReason)) {
			printf("Emergancy CPU Stop: Virtual Execution Error.\n");
			printf("%s\n", TheHaltReason);
			break;
		}
		kb_clock();
		video_clock();
		cpu_clock();
		ClockCnt++;

		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}

	time(&Startdown);

	if (emuctx->DebuggerEnabled)
		decoder_shutdown();

	video_clock();
	printf("Debug Shutdown Interrupt.\n");

	FILE* MemOut = fopen("memout.bin", "wb");
	if (MemOut) {
		fwrite(cpuctx->PhysicalMemory, cpuctx->PhysicalMemorySize, 1, MemOut);
		fclose(MemOut);
	}

	fgetc(stdin);

	devices_shutdown();
	cpu_shutdown();
	emu_shutdown();

	printf("CPU Halted.\n");

	printf("Total Clocks: %llu\n", ClockCnt);
	time_t Diff = (Startdown - Startup) + 1;   
    printf("Total Time: %ldm %lds\n", Diff / 60, Diff % 60);
	printf("Clocks Per Sec: %llu\n", (ClockCnt) / (Diff));

	fclose(a);
    free(i);
    
	return 0;
}
