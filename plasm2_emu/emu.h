//
//  emu.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "basetypes.h"

/*
This is an emulator, and thus
there may be problems that the CPU
cannot handle.
*/

void EmuInit(void);
void EmuShutdown(void);

void EmuHalt(void);
void EmuRegisterFatal(const char* Reason);

_bool EmuCheckClock(char* ThePtr); // query if its time to stop

typedef struct _EMUCTX {
	_bool RequiresHalt;
	char LastReason[256];

	_bool DebuggerEnabled;
}EMU_CTX, *PEMU_CTX;
extern PEMU_CTX EmuCtx;
