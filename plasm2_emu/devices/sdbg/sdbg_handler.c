#include "sdbg.h"
/*
sdbg_handler.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

u64  sdbg_statusquery(u32 Device) {
	return sdbgctx->DeviceStatus;
}
/*
00 Send
01 SetSendLoc
02 SetSendSz
*/
void sdbg_sendcommand(u32 Device, u64 Command) {
	switch (Command) {
	case 0x00: // Send
		sdbg_send();
		break;
	case 0x01: // Set Send Location
	case 0x02: // Set Send Size
		sdbgctx->AwaitingData = 1;
		sdbgctx->LastCommand = Command;
		break;
	}
}

void sdbg_senddata(u32 Device, u64 Command) {
	switch (sdbgctx->LastCommand) {
	case 0x01:
		sdbg_setsendloc(Command);
		break;
	case 0x02:
		sdbg_setsendsz(Command);
		break;
	}
	return 0;
}

u64  sdbg_getdata(u32 Device) {
	return 0;
}

void sdbg_reset(u32 Device) {
	return;
}

void sdbg_off(u32 Device) {
	sdbgctx->Active = 0;
	return;
}

void sdbg_on(u32 Device) {
	sdbgctx->Active = 1;
	return;
}