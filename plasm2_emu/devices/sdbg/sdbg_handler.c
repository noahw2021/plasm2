//
//  sdbg_handler.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "sdbg.h"

WORD64  SdbgStatusQuery(WORD32 Device, WORD64 NullArg) {
	return sdbgctx->DeviceStatus;
}
/*
00 Send
01 SetSendLoc
02 SetSendSz
*/
WORD64 SdbgSendCommand(WORD32 Device, WORD64 Command) {
	switch (Command) {
	case 0x00: // Send
		SdbgSend();
		break;
	case 0x01: // Set Send Location
	case 0x02: // Set Send Size
		sdbgctx->AwaitingData = 1;
		sdbgctx->LastCommand = Command;
		break;
	}
	return 0;
}

WORD64 SdbgSendData(WORD32 Device, WORD64 Command) {
	switch (sdbgctx->LastCommand) {
	case 0x01:
		SdbgSetLocation(Command);
		break;
	case 0x02:
		SdbgSetSize(Command);
		break;
	}
	return 0;
}

WORD64  SdbgGetData(WORD32 Device, WORD64 NullArg) {
	return 0;
}

WORD64 SdbgReset(WORD32 Device, WORD64 NullArg) {
	return 0;
}

WORD64 SdbgOff(WORD32 Device, WORD64 NullArg) {
	sdbgctx->Active = 0;
	return 0;
}

WORD64 SdbgOn(WORD32 Device, WORD64 NullArg) {
	sdbgctx->Active = 1;
	return 0;
}
