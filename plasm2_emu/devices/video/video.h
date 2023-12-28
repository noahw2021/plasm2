//
//  video.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _video_h
#define _video_h

#include "../devices.h"

void VideoInit(void);
void VideoShutdown(void);
void VideoCollect(void);
void VideoClock(void);

WORD64 VideoStatusQuery(WORD32 Device, WORD64 NullArg);
WORD64 VideoSendCommand(WORD32 Device, WORD64 Command);
WORD64 VideoSendData(WORD32 Device, WORD64 Command);
WORD64 VideoGetData(WORD32 Device, WORD64 NullArg);
WORD64 VideoReset(WORD32 Device, WORD64 NullArg);
WORD64 VideoOff(WORD32 Device, WORD64 NullArg);
WORD64 VideoOn(WORD32 Device, WORD64 NullArg);

/*
defaults:

text buffer, 50x50x2, x*y*(char + color)
default colors R:2, G:2, B:2, Blink:1, Reserved:1

*/

/*
commands

GetTextBuffer 00 : Returns text buffer pointer to R0 (deprecated)
SetTextBuffer 01 : Adjusts the text buffer position, Argument (deprecated)
DrawLine      02 : Draws a line v
DrawRect      03 : Draws a rect (color from stack)
DrawFill      04 : Draws a filled rect ^
CopyRect      05 : Copies a rectangular bitmap (32bit color), ptr on stack
GetWh         06 : Get width (hi) and height (lo)
SuggestWh     07 : Suggest a width (hi32) and height (lo32)
*/

WORD64 VideoiGetTextBuffer(void); // deprecated
void VideoiSetTextBuffer(WORD64 NewOffset); // deprecated
void VideoiDrawLine(WORD16 x1, WORD16 y1, WORD16 x2, WORD16 y2, WORD32 color); // color pulled from stack
void VideoiDrawRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color);     // ^
void VideoiDrawFill(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color);     // ^
void VideoiCopyRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD64 ptr); // ptr pulled from stack. size = (w * h * 4)
WORD64  VideoiGetWidthHeight(void);
void VideoiSuggestSize(WORD16 w, WORD16 h);

typedef struct _VIDEO_CTX {
	BYTE SizeLocked;
	WORD64 Status;
	BYTE AwaitingData;
	BYTE DestinationCommand;
	BYTE PendingDataSend;
	WORD64 Outgoing;
	int w;
	int h;
}VIDEO_CTX, *PVIDEO_CTX;

extern PVIDEO_CTX VideoCtx;

#endif
