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

u64  sdbg_statusquery(u32 Device);
void sdbg_sendcommand(u32 Device, u64 Command);
void sdbg_senddata(u32 Device, u64 Command);
u64  sdbg_getdata(u32 Device);
void sdbg_reset(u32 Device);
void sdbg_off(u32 Device);
void sdbg_on(u32 Device);

#ifdef _WIN32
#include <Windows.h>
#endif

typedef struct _sdbgctx {
	char* CollectionBufferOut, *CollectionBufferIn;
	u32 CollectionSizeOut, CollectionSizeIn;
	byte ReadyOut, ReadyIn;

	u64 LastSend;

#ifdef _WIN32
	HANDLE SecondProcess;
	HANDLE DebugPipe;
#endif
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