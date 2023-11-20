//
//  video_native.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "../kb/kb.h"
#include "../../cpu/cpu.h"
#include "../../cpu/mmu/mmu.h"
#include "video.h"
#include <string.h>
#include <SDL.h>

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern BYTE PauseDrawing;

#pragma warning(disable: 26451)

#define R(x) (Uint8)((WORD32)(x & 0xFF000000) >> 24)
#define G(x) (Uint8)((WORD32)(x & 0x00FF0000) >> 16)
#define B(x) (Uint8)((WORD32)(x & 0x0000FF00) >> 8)
#define A(x) (Uint8)((WORD32)(x & 0x000000FF) >> 0)

WORD64 VideoiGetTextBuffer(void) {
	return 0; // deprecated
}

void VideoiSetTextBuffer(WORD64 NewOffset) {
	return; // deprecated
}

void VideoiDrawLine(WORD16 x1, WORD16 y1, WORD16 x2, WORD16 y2, WORD32 color) {
	PauseDrawing = 1;
	SDL_SetRenderDrawColor(Renderer, R(color), G(color), B(color), A(color));
	SDL_RenderDrawLine(Renderer, x1, y1, x2, y2);
	PauseDrawing = 0;

	return;
}

void VideoiDrawRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
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

void VideoiDrawFill(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD32 color) {
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

void VideoiCopyRect(WORD16 x, WORD16 y, WORD16 w, WORD16 h, WORD64 ptr) {
	PauseDrawing = 1;
	WORD64 VAdr = mmu_translate(ptr, REASON_READ, w * h * 4);
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

WORD64  VideoiGetWidthHeight(void) {
	return (VideoCtx->w << 16) | (VideoCtx->h);
}

void VideoiSuggestSize(WORD16 w, WORD16 h) {
	if (!VideoCtx->SizeLocked) {
		VideoCtx->w = w;
		VideoCtx->h = h;
		SDL_SetWindowSize(Window, w, h);
	}

	return;
}
