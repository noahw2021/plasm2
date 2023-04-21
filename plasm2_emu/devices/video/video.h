#pragma once
#include "../devices.h"
/*
video.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void video_init(void);
void video_shutdown(void);

u64  video_statusquery(u32 Device);
void video_sendcommand(u32 Device, u64 Command);
void video_senddata(u32 Device, u64 Command);
u64  video_getdata(u32 Device);
void video_reset(u32 Device);
void video_off(u32 Device);
void video_on(u32 Device);

/*
defaults:

text buffer, 50x50x2, x*y*(char + color)
default colors R:2, G:2, B:2, Blink:1, Reserved:1

*/

/*
commands

GetTextBuffer 00 : Returns text buffer pointer to R0
SetTextBuffer 01 : Adjusts the text buffer position, Argument
DrawLine      02 : Draws a line v
DrawRect      03 : Draws a rect (color from stack)
DrawFill      04 : Draws a filled rect ^
CopyRect      05 : Copies a rectangular bitmap (32bit color), ptr on stack
GetWh         06 : Get width (hi) and height (lo)
SuggestWh     07 : Suggest a width (hi32) and height (lo32)
*/

u64 videoi_gettextbuffer(void);
void videoi_settextbuffer(u64 NewOffset);
void videoi_drawline(u16 x1, u16 y1, u16 x2, u16 y2, u32 color); // color pulled from stack
void videoi_drawrect(u16 x, u16 y, u16 w, u16 h, u32 color);     // ^
void videoi_drawfill(u16 x, u16 y, u16 w, u16 h, u32 color);     // ^
void videoi_copyrect(u16 x, u16 y, u16 w, u16 h, u64 ptr); // ptr pulled from stack. size = (w * h * 4)
u32  videoi_getwh(void);
void videoi_suggestwh(u16 w, u16 h);

typedef struct _videoctx {
	byte TextMode;
	byte SizeLocked;
	u64 Status;
	byte AwaitingData;
	byte DestinationCommand;
	byte PendingDataSend;
	u64 Outgoing;
	int w;
	int h;
	struct {
		union {
			byte RawBytes[2];
			u16 RawU16;
			struct {
				char Character;
				byte R : 2;
				byte G : 2;
				byte B : 2;
				byte Blink : 1;
				byte Reserved : 1;
			};
		};
	}*textmode;
	u64 TextmodePointer;
}videoctx_t;
extern videoctx_t* videoctx;