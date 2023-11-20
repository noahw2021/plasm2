//
//  sdbg.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../devices.h"
#include "../../basetypes.h"
#include <stdio.h>

/*
serial debugger
*/

void sdbg_init(void);
void sdbg_shutdown(void);
void sdbg_clock(void);
void sdbg_collect(void);

u64 sdbg_statusquery(u32 Device, u64 NullArg);
u64 sdbg_sendcommand(u32 Device, u64 Command);
u64 sdbg_senddata(u32 Device, u64 Command);
u64 sdbg_getdata(u32 Device, u64 NullArg);
u64 sdbg_reset(u32 Device, u64 NullArg);
u64 sdbg_off(u32 Device, u64 NullArg);
u64 sdbg_on(u32 Device, u64 NullArg);

typedef struct _sdbgctx {
	char* CollectionBufferOut, *CollectionBufferIn;
	u64 CollectionSizeOut, CollectionSizeIn;
	byte ReadyOut, ReadyIn;

	u64 LastSend;
	u64 VirtualPointer;
	u64 VirtualSize;

	u64 DeviceStatus;
	byte Active;
	byte AwaitingData;
	u64 LastCommand;
}sdbgctx_t;
extern sdbgctx_t* sdbgctx;

/*
00 Send
01 SetSendLoc
02 SetSendSz
*/

void sdbg_send(void);
void sdbg_setsendloc(u64 Location);
void sdbg_setsendsz(u64 Size);
