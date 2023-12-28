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
0x2600: Beginning of first page table
0x3600: End of first page table
*/

int __t_argc;
char** __t_argv;
PPLASM2_CTX ECtx;

typedef struct _APP_ARGS {
    char** argv;
    int argc;
}APP_ARGS, *PAPP_ARGS;
int PlasmEmuNonVideoMain(PAPP_ARGS);

#include <SDL.h>

_bool ShouldStartVideo;
_bool ShouldStopVideo;
_bool VideoStopped;
int main(int argc, char** argv) {
    ShouldStartVideo = FALSE;
    ShouldStopVideo = FALSE;
    
    PAPP_ARGS Args = malloc(sizeof(APP_ARGS));
    Args->argc = argc;
    Args->argv = argv;
    
    SDL_CreateThread(PlasmEmuNonVideoMain, "Plasm2MainThread", Args);
    
    while (!ShouldStartVideo)
        SDL_Delay(100);
    VideoInit();
}

int PlasmEmuNonVideoMain(PAPP_ARGS Args) {
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
			printf("%s -f=drive.pff : Mounts a fixed disk. Allows multiple.\n", 
                argv[0]);
			
			printf("Alt Function Switches: \n\n");
			printf("%s -t | --tools : Loads PLASM2Emu toolkit.\n", argv[0]);

			printf("Misc Switches: (General Function Only)\n\n");
			printf("%s -d | --debug : Enables disassembler / debugger mode.\n", 
                argv[0]);
			printf("%s -h | --help : Shows this screen.\n", argv[0]);
            printf("%s --no-debug : Prevents the debugger from activating.\n", 
                argv[0]);
            printf("%s --no-print : Allows the debugger to activate, but "
                "without printing.\n", argv[0]);
            printf("%s --no-clock : Disables the clock speed regulation "
                "system. Drastically increases performance.\n", argv[0]);
            printf("%s --no-secure : Disables virtual memory security.\n", 
                argv[0]);
            
			printf("\nBy default, PLASM2Emu accepts a properly formed "
                "'bios.bin'.\n");
            
			return 0;
		}
		if (strstr(argv[i], "-f=")) {
            if (!FixedDisks) {
                FixedDisks = malloc(sizeof(char*) * (FixedDiskCount + 1));
            } else {
                FixedDisks = realloc(FixedDisks, 
                    (sizeof(char*) * (FixedDiskCount + 1)));
            }
            
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
        
        if (strstr(argv[i], "--no-debug")) {
            EmuCtx->Flags |= EMUFLAG_NODEBUG;
        }
        
        if (strstr(argv[i], "--no-print")) {
            EmuCtx->Flags |= EMUFLAG_NOPRINT;
        }
        
        if (strstr(argv[i], "--no-clock")) {
            EmuCtx->Flags |= EMUFLAG_NOCLOCK;
        }
        
        if (strstr(argv[i], "--no-secure")) {
            EmuCtx->Flags |= EMUFLAG_NOSECURE;
        }
	}
	
    ECtx = malloc(sizeof(PLASM2_CTX));
	memset(ECtx, 0, sizeof(*ECtx));

    CpuInit();
    
	ECtx->ControlRegisters.DeviceMap = 0x0000;
	ECtx->ip = 0x03A0;
	ECtx->ControlRegisters.StackPointerLowerBound = 0x24F0;
	ECtx->ControlRegisters.StackPointerUpperBound = 0x25F0;
	ECtx->sp = 0x24F0;

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
    
	fread((BYTE*)CpuCtx->PhysicalMemory + 0x3A0, BiosLength, 1, Bios); 
    // read the bios into ram
    // PM usage good (reason: comes from trust)
    
	DevicesInit();
	DevicesCollect();

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

	WORD64 PreMs = CpuTimerGetPreciseTimeMilliseconds();
    
	while (1) {
		if (EmuCheckClock(TheHaltReason)) {
			printf("Emergancy CPU Stop: Virtual Execution Error.\n");
			printf("%s\n", TheHaltReason);
			break;
		}
        
		KbClock();
		VideoClock();
		CpuClock();

		if (ECtx->Flags.HF && !ECtx->Flags.IF)
			break;
	}
    
    WORD64 PostMs = CpuTimerGetPreciseTimeMilliseconds();

	if (EmuCtx->DebuggerEnabled)
		DecoderShutdown();

	VideoClock();
    
	FILE* MemOut = fopen("memout.bin", "wb");
	if (MemOut) {
		fwrite(CpuCtx->PhysicalMemory, CpuCtx->PhysicalMemorySize, 1, MemOut);
		fclose(MemOut);
	}

	//fgetc(stdin);

    WORD64 SystemTickBackup = CpuCtx->SystemTicks;
	DevicesShutdown();
	CpuShutdown();
	EmuShutdown();

	printf("CPU Halted.\n");
    free(ECtx);
    
    WORD64 MsDifference = PostMs - PreMs;
    WORD64 SDifference = MsDifference / 1000;
    MsDifference %= 1000;
    
    printf("Completed execution of %llu instructions in %llus, %llums.\n",
        SystemTickBackup, SDifference, MsDifference);
    
    long double PreciseFloatingTime = SDifference + (MsDifference / 1000.0);
    long double PreciseFloatingInstructions = SystemTickBackup;
    long double PreciseClockSpeed = PreciseFloatingInstructions 
        / PreciseFloatingTime;
    WORD64 ImpreciseClockSpeed = (WORD64)PreciseClockSpeed;
    
    printf("Average Clock Speed: %llu Hz", ImpreciseClockSpeed);
    
	return 0;
}
