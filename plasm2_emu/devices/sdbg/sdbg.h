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

typedef struct _sdbgctx {
	char* CollectionBufferOut, *CollectionBufferIn;
	u32 CollectionSizeOut, CollectionSizeIn;
}sdbgctx_t;
extern sdbgctx_t* sdbgctx;

/*
00 AddToCollection
01 Send
02 SetRecievePtr
03 SetRecieveBfrSize
04 Recieve
05 SetRecieveHandler
06 DisableRecieveHandler
07 EnableRecieveHandler
*/

void sdbg_collect(u64 Data);
void sdbg_send(void);
void sdbg_setrecvptr(u64 Data);
void sdbg_setbfrsize(u32 MaxBufferSize);
void sdbg_recv(void);
void sdbg_setrecvhndlr(u64 Address);
void sdbg_recvhon(void);
void sdbg_recvhoff(void);