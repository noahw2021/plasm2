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
u32  video_devcount(void);

/*
defaults:

text buffer, 50x50x2, x*y*(char + color)
default colors R:2, G:2, B:2, Blink:1, 

/

/*
commands

GetTextBuffer 00 : Returns text buffer pointer to R0
SetTextBuffer 01 : Adjusts the text buffer position
*/

u64 videoi_gettextbuffer(void);
void videoi_settextbuffer(u64 Offset);
void videoi_drawline(u32 x1, u32 y1, u32 x2, u32 y2, u32 color);
void videoi_drawrect(u32 x, u32 y, u32 w, u32 h, u32 color);
void videoi_drawfill(u32 x, u32 y, u32 w, u32 h, u32 color);
u64  videoi_getfb(void);
u32  videoi_getwh(void);
void videoi_suggestwh(u16 w, u16 h);