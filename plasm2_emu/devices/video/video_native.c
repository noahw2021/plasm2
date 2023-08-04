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

#define R(x) (x & 0xFF000000) >> 24
#define G(x) (x & 0x00FF0000) >> 16
#define B(x) (x & 0x0000FF00) >> 8
#define A(x) (x & 0x000000FF) >> 0

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

void videoi_copyrect(u16 x, u16 y, u16 w, u16 h, u64 ptr) {
	PauseDrawing = 1;
	SDL_Surface* Surface = SDL_CreateRGBSurface(0, w, h, 8, 0, 0, 0, 0);
	if (mmu_maxaddr(ptr, REASON_READ) < (w * h * 4)) {
		PauseDrawing = 0;
		cpui_csm_msg(CSM_PAGETOOSMALL, ptr);
		return;
	}

	memcpy(Surface->pixels, cpuctx->PhysicalMemory + mmu_translate(ptr, REASON_READ), w * h * 4); // pm usage good (reason: sanity check)
	SDL_Texture* TargetTexture = SDL_CreateTextureFromSurface(Renderer, Surface);
	SDL_FreeSurface(Surface);
	SDL_Rect DestRect = { x, y, w, h };
	SDL_RenderCopy(Renderer, TargetTexture, NULL, &DestRect);
	SDL_DestroyTexture(TargetTexture);
	PauseDrawing = 0;

	return;
}

u32  videoi_getwh(void) {
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