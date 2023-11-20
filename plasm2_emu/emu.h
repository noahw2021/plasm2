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

void emu_init(void);
void emu_shutdown(void);

void emu_halt(void);
void emu_register_fatal(const char* Reason);

_bool emu_aufhoren(char* ThePtr); // query if its time to stop

typedef struct _emuctx {
	_bool RequiresHalt;
	char LastReason[256];

	_bool DebuggerEnabled;
}emuctx_t;
extern emuctx_t* emuctx;
