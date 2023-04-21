#include "../devices.h"
#include "../../cpu/cpu.h"
#include "kb.h"
#include <string.h>
/*
kb_handlers.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

u64  kb_statusquery(u32 Device) {
	return kbctx->Status;
}

void kb_sendcommand(u32 Device, u64 Command) {
	switch (Command) {
	case 0x00: // needs data block
	case 0x01:
		kbctx->AwaitingData = 1;
		break;
	case 0x02: // doesnt
		kbctx->DataQueued = 1;
		kbctx->OutputQueue = kbi_getkeymapptr();
		break;
	default:
		kbctx->Status = DEVSTAUTS_INVL;
		return;
	}
	kbctx->AwaitingCommand = Command;
	kbctx->Status = DEVSTATUS_GOOD;
	return;
}

void kb_senddata(u32 Device, u64 Data) {
	if (kbctx->AwaitingData) {
		if (kbctx->AwaitingCommand == 0x00)
			kbi_setkeydownint(Data);
		else
			kbi_setkeyupint(Data);
		kbctx->AwaitingCommand = 0;
		kbctx->AwaitingData = 0;
	}
	kbctx->Status = DEVSTATUS_GOOD;
}

u64  kb_getdata(u32 Device) {
	if (kbctx->DataQueued) {
		kbctx->DataQueued = 0;
		return kbctx->OutputQueue;
	}
	return 0;
}

void kb_reset(u32 Device) {
	for (int i = 0; i < 4; i++)
		KeysDown[i] = 0;
	return;
}

void kb_off(u32 Device) {
	kbctx->HoldUp = 1;
	return;
}

void kb_on(u32 Device) {
	kbctx->HoldUp = 0;
	return;
}