//
//  emu.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _emu_h
#define _emu_h

#include "basetypes.h"
#include <stdio.h>
#include <SDL.h>

/*
This is an emulator, and thus
there may be problems that the CPU
cannot handle.
*/

#define EMUFLAG_NOPRINT  0x0001
#define EMUFLAG_NODEBUG  0x0002
#define EMUFLAG_NOCLOCK  0x0004
#define EMUFLAG_NOSECURE 0x0008

void EmuInit(void);
void EmuShutdown(void);

void EmuHalt(void);
void EmuRegisterFatal(const char* Reason);

_bool EmuCheckClock(char* ThePtr); // query if its time to stop

typedef struct _EMU_CTX {
	_bool RequiresHalt;
	char LastReason[256];

	_bool DebuggerEnabled;
    
    void* VideoMutex;
    WORD64 Flags;
}EMU_CTX, *PEMU_CTX;

void* EmutexCreate(void);
void EmutexLock(void* Mutex);
void EmutexUnlock(void* Mutex);
void EmutexDestory(void* Mutex);

extern PEMU_CTX EmuCtx;

#endif
