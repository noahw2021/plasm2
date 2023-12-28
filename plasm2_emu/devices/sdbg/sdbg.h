//
//  sdbg.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _sdbg_h
#define _sdbg_h

#include "../devices.h"
#include "../../basetypes.h"
#include <stdio.h>

/*
serial debugger
*/

// @TODO 11/20/23 remove sdbg, unused & deprecated

void SdbgInit(void);
void SdbgShutdown(void);
void SdbgClock(void);
void SdbgCollect(void);

WORD64 SdbgStatusQuery(WORD32 Device, WORD64 NullArg);
WORD64 SdbgSendCommand(WORD32 Device, WORD64 Command);
WORD64 SdbgSendData(WORD32 Device, WORD64 Command);
WORD64 SdbgGetData(WORD32 Device, WORD64 NullArg);
WORD64 SdbgReset(WORD32 Device, WORD64 NullArg);
WORD64 SdbgOff(WORD32 Device, WORD64 NullArg);
WORD64 SdbgOn(WORD32 Device, WORD64 NullArg);

typedef struct SDBG_CTX {
	char* CollectionBufferOut, *CollectionBufferIn;
	WORD64 CollectionSizeOut, CollectionSizeIn;
	BYTE ReadyOut, ReadyIn;

	WORD64 LastSend;
	WORD64 VirtualPointer;
	WORD64 VirtualSize;

	WORD64 DeviceStatus;
	BYTE Active;
	BYTE AwaitingData;
	WORD64 LastCommand;
}SDBG_CTX, *PSDBG_CTX;

extern PSDBG_CTX sdbgctx;

/*
00 Send
01 SetSendLoc
02 SetSendSz
*/

void SdbgSend(void);
void SdbgSetLocation(WORD64 Location);
void SdbgSetSize(WORD64 Size);

#endif
