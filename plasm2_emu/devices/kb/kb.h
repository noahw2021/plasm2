#pragma once
#include "../devices.h"
#include "../../basetypes.h"
/*
kb.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void kbii_markkey(byte Key, byte Status);
extern u64 KeysDown[4];

void kb_init(void);
void kb_shutdown(void);
void kb_clock(void);
void kb_collect(void);

u64 kb_statusquery(u32 Device, u64 NullArg);
u64 kb_sendcommand(u32 Device, u64 Command);
u64 kb_senddata(u32 Device, u64 Command);
u64 kb_getdata(u32 Device, u64 NullArg);
u64 kb_reset(u32 Device, u64 NullArg);
u64 kb_off(u32 Device, u64 NullArg);
u64 kb_on(u32 Device, u64 NullArg);

typedef struct _kbctx {
	byte AwaitingData;
	byte DataQueued;
	u64 OutputQueue;
	byte AwaitingCommand;
	u64 Status;
	byte HoldUp;
	byte NotifyUp, NotifyDown;

	byte KeyUp, KeyDown;
	u64 KeyUpHandlr, KeyDownHandlr;
}kbctx_t;
extern kbctx_t* kbctx;

/*
commands:

00 SetKeyDownInt : sets the key up interrupt
01 SetKeyUpInt   : sets the key down interrupt
02 GetKeyMapPtr  : gets the key map pointer (shouldn't ever not be 0x23F0 physically, but virtual idk)

*/

void kbi_setkeydownint(u64 Interrupt);
void kbi_setkeyupint(u64 Interrupt);
u64  kbi_getkeymapptr(void);