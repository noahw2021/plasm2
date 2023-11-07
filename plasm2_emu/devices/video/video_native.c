#include "../devices.h"
#include "../kb/kb.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "video.h"
#include <string.h>
#include <SDL.h>
/*
video_native.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern byte PauseDrawing;

#pragma warning(disable: 26451)

#define R(x) (Uint8)((u32)(x & 0xFF000000) >> 24)
#define G(x) (Uint8)((u32)(x & 0x00FF0000) >> 16)
#define B(x) (Uint8)((u32)(x & 0x0000FF00) >> 8)
#define A(x) (Uint8)((u32)(x & 0x000000FF) >> 0)

u64 videoi_gettextbuffer(void) {
	return 0; // deprecated
}

void videoi_settextbuffer(u64 NewOffset) {
	return; // deprecated
}

void videoi_drawline(u16 x1, u16 y1, u16 x2, u16 y2, u32 color) {
	PauseDrawing = 1;
	SDL_SetRenderDrawColor(Renderer, R(color), G(color), B(color), A(color));
	SDL_RenderDrawLine(Renderer, x1, y1, x2, y2);
	PauseDrawing = 0;

	return;
}

void videoi_drawrect(u16 x, u16 y, u16 w, u16 h, u32 color) {
	PauseDrawing = 1;
	SDL_SetRenderDrawColor(Renderer, R(color), G(color), B(color), A(color));
	SDL_Rect DestRect;
	DestRect.x = x;
	DestRect.y = y;
	DestRect.h = h;
	DestRect.w = w;
	SDL_RenderDrawRect(Renderer, &DestRect);
	PauseDrawing = 0;

	return;
}

void videoi_drawfill(u16 x, u16 y, u16 w, u16 h, u32 color) {
	PauseDrawing = 1;
	SDL_SetRenderDrawColor(Renderer, R(color), G(color), B(color), A(color));
	SDL_Rect DestRect;
	DestRect.x = x;
	DestRect.y = y;
	DestRect.h = h;
	DestRect.w = w;
	SDL_RenderFillRect(Renderer, &DestRect);
	PauseDrawing = 0;

	return;
}

#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff

void videoi_copyrect(u16 x, u16 y, u16 w, u16 h, u64 ptr) {
	PauseDrawing = 1;
	u64 VAdr = mmu_translate(ptr, REASON_READ, w * h * 4);
	if (!VAdr) {
		cpui_csm_msg(CSM_PAGETOOSMALL, ptr);
	}
	SDL_Surface* Surface = SDL_CreateRGBSurfaceFrom((cpuctx->PhysicalMemory + VAdr), w, h, 32, 32 * w, RMASK, GMASK, BMASK, AMASK);
	SDL_Texture* TargetTexture = SDL_CreateTextureFromSurface(Renderer, Surface);
	SDL_FreeSurface(Surface);
	SDL_Rect DestRect = { x, y, w, h };
	SDL_RenderCopy(Renderer, TargetTexture, NULL, &DestRect);
	SDL_DestroyTexture(TargetTexture);
	PauseDrawing = 0;

	return;
}

u64  videoi_getwh(void) {
	return (videoctx->w << 16) | (videoctx->h);
}

void videoi_suggestwh(u16 w, u16 h) {
	if (!videoctx->SizeLocked) {
		videoctx->w = w;
		videoctx->h = h;
		SDL_SetWindowSize(Window, w, h);
	}

	return;
}