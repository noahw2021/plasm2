#include "../devices.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "video.h"
#include "../kb/kb.h"
#include <string.h>
/*
video_handlers.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#define GET16_HIHI(x) (u64)((x & 0xFFFF000000000000) >> 48)
#define GET16_HILO(x) (u64)((x & 0x0000FFFF00000000) >> 32)
#define GET16_LOHI(x) (u64)((x & 0x00000000FFFF0000) >> 16)
#define GET16_LOLO(x) (u64)((x & 0x000000000000FFFF) >> 00)

u64  video_statusquery(u32 Device, u64 NullArg) {
	return DEVSTATUS_GOOD;
}

u64 video_sendcommand(u32 Device, u64 Command) {
	switch (Command) {
	case 0x06:
	case 0x00:
		videoctx->AwaitingData = 0;
		videoctx->PendingDataSend = 1;
		if (Command == 0x06)
			videoctx->Outgoing = videoi_getwh();
		else
			videoctx->Outgoing = videoi_gettextbuffer();
		break;
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x07:
		videoctx->DestinationCommand = Command;
		videoctx->AwaitingData = 1;
		videoctx->PendingDataSend = 0;
		break;
	default:
		videoctx->Status = DEVSTAUTS_INVL; // invalid command
		return 0;
	}

	videoctx->DestinationCommand = (byte)Command;
	kbctx->Status = DEVSTATUS_GOOD;
	return 0;
}

u64 video_senddata(u32 Device, u64 Data) {
	if (!videoctx->AwaitingData)
		return 0;
	u32 Color;
	u64 Ptr;
	switch (videoctx->DestinationCommand) {
	case 0x01:
		videoi_settextbuffer(Data);
		break;
	case 0x02:
		Color = (u32)mmu_pop();
		videoi_drawline((u16)GET16_HIHI(Data), (u16)GET16_HILO(Data), (u16)GET16_LOHI(Data), (u16)GET16_LOLO(Data), Color);
		break;
	case 0x03:
		Color = (u32)mmu_pop();
		videoi_drawrect((u16)GET16_HIHI(Data), (u16)GET16_HILO(Data), (u16)GET16_LOHI(Data), (u16)GET16_LOLO(Data), Color);
		break;
	case 0x04:
		Color = (u32)mmu_pop();
		videoi_drawfill((u16)GET16_HIHI(Data), (u16)GET16_HILO(Data), (u16)GET16_LOHI(Data), (u16)GET16_LOLO(Data), Color);
		break;
	case 0x05:
		Ptr = mmu_pop(); // aka pointer here
		videoi_copyrect((u16)GET16_HIHI(Data), (u16)GET16_HILO(Data), (u16)GET16_LOHI(Data), (u16)GET16_LOLO(Data), Ptr);
		break;
	case 0x06: // wip
		break;
	}
	return 0;
}

u64  video_getdata(u32 Device, u64 NullArg) {
	if (videoctx->PendingDataSend)
		return videoctx->Outgoing;
	return 0;
}

u64 video_reset(u32 Device, u64 NullArg) {
	videoi_drawrect(0, 0, videoctx->w, videoctx->h, 0x000000FF);
	return 0;
}

u64 video_off(u32 Device, u64 NullArg) {
	if (i->security_s.SecurityLevel < 2)
		video_shutdown();
	return 0;
}
u64 video_on(u32 Device, u64 NullArg) {
	if (i->security_s.SecurityLevel < 2)
		video_init();
	return 0;
}

void video_clock(void) { 
	return;
}