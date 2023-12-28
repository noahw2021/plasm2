//
//  kb.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _kb_h
#define _kb_h

#include "../devices.h"
#include "../../basetypes.h"

void KbiiMarkKeyState(BYTE Key, BYTE Status);
extern WORD64 KeysDown[4];

void KbInit(void);
void KbShutdown(void);
void KbClock(void);
void KbCollect(void);

WORD64 KbStatusQuery(WORD32 Device, WORD64 NullArg);
WORD64 KbSendCommand(WORD32 Device, WORD64 Command);
WORD64 KbSendData(WORD32 Device, WORD64 Command);
WORD64 KbGetData(WORD32 Device, WORD64 NullArg);
WORD64 KbReset(WORD32 Device, WORD64 NullArg);
WORD64 KbOff(WORD32 Device, WORD64 NullArg);
WORD64 KbOn(WORD32 Device, WORD64 NullArg);

typedef struct _KB_CTX {
	BYTE AwaitingData;
	BYTE DataQueued;
	WORD64 OutputQueue;
	BYTE AwaitingCommand;
	WORD64 Status;
	BYTE HoldUp;
	BYTE NotifyUp, NotifyDown;

	BYTE KeyUp, KeyDown;
	WORD64 KeyUpHandlr, KeyDownHandlr;
}KB_CTX, *PKB_CTX;
extern PKB_CTX KbCtx;

/*
commands:

00 SetKeyDownInt : sets the key up interrupt
01 SetKeyUpInt   : sets the key down interrupt
02 GetKeyMapPtr  : gets the key map pointer (shouldn't ever not be 0x23F0 physically, but virtual idk)

*/

void KbiSetKeyDownInterrupt(WORD64 Interrupt);
void KbiSetKeyUpInterrupt(WORD64 Interrupt);
WORD64 KbiGetKeyMapPointer(void);

#endif
