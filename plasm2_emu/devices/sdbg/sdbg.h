#pragma once
#include "../devices.h"
#include "../../basetypes.h"
#include <stdio.h>
/*
sdbg.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

/*
serial debugger
*/

void sdbg_init(void);
void sdbg_shutdown(void);
void sdbg_clock(void);
void sdbg_collect(void);

u64  sdbg_statusquery(u64 Device);
void sdbg_sendcommand(u64 Device, u64 Command);
void sdbg_senddata(u64 Device, u64 Command);
u64  sdbg_getdata(u64 Device);
void sdbg_reset(u64 Device);
void sdbg_off(u64 Device);
void sdbg_on(u64 Device);

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