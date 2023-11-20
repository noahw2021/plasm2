//
//  video_handlers.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "video.h"
#include "../kb/kb.h"
#include <string.h>

#define GET16_HIHI(x) (WORD64)((x & 0xFFFF000000000000) >> 48)
#define GET16_HILO(x) (WORD64)((x & 0x0000FFFF00000000) >> 32)
#define GET16_LOHI(x) (WORD64)((x & 0x00000000FFFF0000) >> 16)
#define GET16_LOLO(x) (WORD64)((x & 0x000000000000FFFF) >> 00)

WORD64  VideoStatusQuery(WORD32 Device, WORD64 NullArg) {
	return DEVSTATUS_GOOD;
}

WORD64 VideoSendCommand(WORD32 Device, WORD64 Command) {
	switch (Command) {
	case 0x06:
	case 0x00:
		VideoCtx->AwaitingData = 0;
		VideoCtx->PendingDataSend = 1;
		if (Command == 0x06)
			VideoCtx->Outgoing = VideoiGetWidthHeight();
		else
			VideoCtx->Outgoing = VideoiGetTextBuffer();
		break;
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x07:
		VideoCtx->DestinationCommand = Command;
		VideoCtx->AwaitingData = 1;
		VideoCtx->PendingDataSend = 0;
		break;
	default:
		VideoCtx->Status = DEVSTAUTS_INVL; // invalid command
		return 0;
	}

	VideoCtx->DestinationCommand = (BYTE)Command;
	KbCtx->Status = DEVSTATUS_GOOD;
	return 0;
}

WORD64 VideoSendData(WORD32 Device, WORD64 Data) {
	if (!VideoCtx->AwaitingData)
		return 0;
	WORD32 Color;
	WORD64 Ptr;
	switch (VideoCtx->DestinationCommand) {
	case 0x01:
		VideoiSetTextBuffer(Data);
		break;
	case 0x02:
		Color = (WORD32)mmu_pop();
		VideoiDrawLine((WORD16)GET16_HIHI(Data), (WORD16)GET16_HILO(Data), (WORD16)GET16_LOHI(Data), (WORD16)GET16_LOLO(Data), Color);
		break;
	case 0x03:
		Color = (WORD32)mmu_pop();
		VideoiDrawRect((WORD16)GET16_HIHI(Data), (WORD16)GET16_HILO(Data), (WORD16)GET16_LOHI(Data), (WORD16)GET16_LOLO(Data), Color);
		break;
	case 0x04:
		Color = (WORD32)mmu_pop();
		VideoiDrawFill((WORD16)GET16_HIHI(Data), (WORD16)GET16_HILO(Data), (WORD16)GET16_LOHI(Data), (WORD16)GET16_LOLO(Data), Color);
		break;
	case 0x05:
		Ptr = mmu_pop(); // aka pointer here
		VideoiCopyRect((WORD16)GET16_HIHI(Data), (WORD16)GET16_HILO(Data), (WORD16)GET16_LOHI(Data), (WORD16)GET16_LOLO(Data), Ptr);
		break;
	case 0x06: // wip
		break;
	}
	return 0;
}

WORD64  VideoGetData(WORD32 Device, WORD64 NullArg) {
	if (VideoCtx->PendingDataSend)
		return VideoCtx->Outgoing;
	return 0;
}

WORD64 VideoReset(WORD32 Device, WORD64 NullArg) {
	VideoiDrawRect(0, 0, VideoCtx->w, VideoCtx->h, 0x000000FF);
	return 0;
}

WORD64 VideoOff(WORD32 Device, WORD64 NullArg) {
	if (i->security_s.SecurityLevel < 2)
		VideoShutdown();
	return 0;
}
WORD64 VideoOn(WORD32 Device, WORD64 NullArg) {
	if (i->security_s.SecurityLevel < 2)
		VideoInit();
	return 0;
}

void VideoClock(void) { 
	return;
}
