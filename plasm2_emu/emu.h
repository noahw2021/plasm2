#pragma once
#include "basetypes.h"

/*
emu.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

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
	_bool BreakActive;
	_bool Step; // 0 = Continue, 1 = Enable Debug and Continue, 2 = Enable Debug and Step
	_bool Stepper;
	_bool DeBuggerOff;
}emuctx_t;
extern emuctx_t* emuctx;