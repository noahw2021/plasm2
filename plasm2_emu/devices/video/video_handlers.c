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

#define GET16_HIHI(x) (x & 0xFFFF000000000000) >> 48
#define GET16_HILO(x) (x & 0x0000FFFF00000000) >> 32
#define GET16_LOHI(x) (x & 0x00000000FFFF0000) >> 16
#define GET16_LOLO(x) (x & 0x000000000000FFFF) >> 00

u64  video_statusquery(u64 Device) {
	return DEVSTATUS_GOOD;
}

void video_sendcommand(u64 Device, u64 Command) {
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
		videoctx->AwaitingData = 1;
		videoctx->PendingDataSend = 0;
		break;
	default:
		videoctx->Status = DEVSTAUTS_INVL; // invalid command
		return;
	}

	videoctx->DestinationCommand = Command;
	kbctx->Status = DEVSTATUS_GOOD;
	return;
}

void video_senddata(u64 Device, u64 Data) {
	if (!videoctx->AwaitingData)
		return;
	u32 color;
	switch (videoctx->DestinationCommand) {
	case 0x01:
		videoi_settextbuffer(Data);
		break;
	case 0x02:
		Color = mmu_pop();
		videoi_drawline(GET16_HIHI(Data), GET16_HILO(Data), GET16_LOHI(Data), GET16_LOLO(Data), Color);
		break;
	case 0x03:
		Color = mmu_pop();
		videoi_drawrect(GET16_HIHI(Data), GET16_HILO(Data), GET16_LOHI(Data), GET16_LOLO(Data), Color);
		break;
	case 0x04:
		Color = mmu_pop();
		videoi_drawfill(GET16_HIHI(Data), GET16_HILO(Data), GET16_LOHI(Data), GET16_LOLO(Data), Color);
		break;
	case 0x05:
		Color = mmu_pop();
		videoi_drawrect(GET16_HIHI(Data), GET16_HILO(Data), GET16_LOHI(Data), GET16_LOLO(Data), Color);
		break;
	case 0x06:
		Color = mmu_pop(); // aka pointer here
		videoi_copyrect(GET16_HIHI(Data), GET16_HILO(Data), GET16_LOHI(Data), GET16_LOLO(Data), Color);
		break;
	case 0x07:
		break;
	}
}

u64  video_getdata(u64 Device) {
	if (videoctx->PendingDataSend)
		return videoctx->Outgoing;
	return 0;
}

void video_reset(u64 Device) {
	videoi_drawrect(0, 0, videoctx->w, videoctx->h, 0x000000FF);
}

void video_off(u64 Device) {
	if (i->security_s.SecurityLevel < 2)
		video_shutdown();
	return;
}
void video_on(u64 Device) {
	if (i->security_s.SecurityLevel < 2)
		video_init();
	return;
}

void video_clock(void) { 
	return;
}