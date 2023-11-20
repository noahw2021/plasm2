//
//  kb_handlers.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "kb.h"
#include <string.h>

WORD64  KbStatusQuery(WORD32 Device, WORD64 NullArg) {
	return KbCtx->Status;
}

WORD64 KbSendCommand(WORD32 Device, WORD64 Command) {
	switch (Command) {
	case 0x00: // needs data block
	case 0x01:
		KbCtx->AwaitingData = 1;
		break;
	case 0x02: // doesnt
		KbCtx->DataQueued = 1;
		KbCtx->OutputQueue = KbiGetKeyMapPointer();
		break;
	default:
		KbCtx->Status = DEVSTAUTS_INVL;
		return 0;
	}
	KbCtx->AwaitingCommand = (BYTE)Command;
	KbCtx->Status = DEVSTATUS_GOOD;
	return 0;
}

WORD64 KbSendData(WORD32 Device, WORD64 Data) {
	if (KbCtx->AwaitingData) {
		if (KbCtx->AwaitingCommand == 0x00)
			KbiSetKeyDownInterrupt(Data);
		else
			KbiSetKeyUpInterrupt(Data);
		KbCtx->AwaitingCommand = 0;
		KbCtx->AwaitingData = 0;
	}
	KbCtx->Status = DEVSTATUS_GOOD;
	return 0;
}

WORD64  KbGetData(WORD32 Device, WORD64 NullArg) {
	if (KbCtx->DataQueued) {
		KbCtx->DataQueued = 0;
		return KbCtx->OutputQueue;
	}
	return 0;
}

WORD64 KbReset(WORD32 Device, WORD64 NullArg) {
	for (int i = 0; i < 4; i++)
		KeysDown[i] = 0;
	return 0;
}

WORD64 KbOff(WORD32 Device, WORD64 NullArg) {
	KbCtx->HoldUp = 1;
	return 0;
}

WORD64  KbOn(WORD32 Device, WORD64 NullArg) {
	KbCtx->HoldUp = 0;
	return 0;
}
