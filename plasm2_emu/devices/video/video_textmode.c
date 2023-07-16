#include "video.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
/*
video_textmode.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern byte PauseDrawing;

TTF_Font* SdlFont;

void video_clock(void) {
	
	

	return;
}

void videof_init(void) {
	TTF_Init();
	SdlFont = TTF_OpenFont("resources\\vgafon.ttf", 12);
	if (!SdlFont) {
		printf("")
	}
}