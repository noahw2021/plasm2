//
//  main.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
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

typedef struct _appargs {
    char** argv;
    int argc;
}appargs_t;
int __nonvideo_main(appargs_t*);

#include <SDL.h>

_bool ShouldStartVideo;
int main(int argc, char** argv) {
    ShouldStartVideo = FALSE;
    
    appargs_t* Args = malloc(sizeof(appargs_t));
    Args->argc = argc;
    Args->argv = argv;
    
    SDL_CreateThread(__nonvideo_main, "Plasm2MainThread", Args);
    
    while (!ShouldStartVideo)
        SDL_Delay(100);
    VideoInit();
}

int __nonvideo_main(appargs_t* Args) {
    int argc = Args->argc;
    char** argv = Args->argv;
    
    EmuInit();
	Psin2Init();
    
    EmuCtx->VideoMutex = EmutexCreate();
	
	char** FixedDisks = NULL;
	int FixedDiskCount = 0;

	for (int i = 1; i < argc; i++) {
		if (strstr(argv[i], "-d") || strstr(argv[i], "--debug")) {
			EmuCtx->DebuggerEnabled = 1;
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
			ToolsMain();
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
    fseek(Bios, 0, SEEK_END);
    WORD32 BiosLength = ftell(Bios);
    if (BiosLength > 4096)
        BiosLength = 4906;
    fseek(Bios, 0, SEEK_SET);
    
	fread((BYTE*)cpuctx->PhysicalMemory + 0x3A0, BiosLength, 1, Bios); // read the bios into ram
    
	DevicesInit();
	DevicesCollect();// PM usage good (reason: comes from trust)

	for (int i = 0; i < FixedDiskCount; i++) {
		if (!FdiskRegister(FixedDisks[i])) {
			printf("[ERR]: Failed to open fixed disk '%s'!\n", FixedDisks[i]);
			EmuRegisterFatal("Could not obtain drive.");
		}
		free(FixedDisks[i]);
	}
	if (FixedDisks)
		free(FixedDisks);

	if (EmuCtx->DebuggerEnabled)
		DecoderInit();

	char TheHaltReason[256];

	time_t Startup, Startdown;
	Startup = time(NULL);
	WORD64 ClockCnt = 0;
    
	while (1) {
		if (EmuCheckClock(TheHaltReason)) {
			printf("Emergancy CPU Stop: Virtual Execution Error.\n");
			printf("%s\n", TheHaltReason);
			break;
		}
        
		KbClock();
		VideoClock();
		cpu_clock();
		ClockCnt++;

		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}

	time(&Startdown);

	if (EmuCtx->DebuggerEnabled)
		DecoderShutdown();

	VideoClock();
	printf("Debug Shutdown Interrupt.\n");

	FILE* MemOut = fopen("memout.bin", "wb");
	if (MemOut) {
		fwrite(cpuctx->PhysicalMemory, cpuctx->PhysicalMemorySize, 1, MemOut);
		fclose(MemOut);
	}

	fgetc(stdin);

	DevicesShutdown();
	cpu_shutdown();
	EmuShutdown();

	printf("CPU Halted.\n");

	printf("Total Clocks: %llu\n", ClockCnt);
	time_t Diff = (Startdown - Startup) + 1;   
    printf("Total Time: %ldm %lds\n", Diff / 60, Diff % 60);
	printf("Clocks Per Sec: %llu\n", (ClockCnt) / (Diff));

	//fclose(a);
    free(i);
    
	return 0;
}
