#include "../devices.h"
#include "../kb/kb.h"
#include "../../cpu/cpu.h"
#include "video.h"
#include <string.h>
#include <SDL.h>
/*
video_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

SDL_Window* Window;
SDL_Renderer* Renderer;
byte PauseDrawing;
videoctx_t* videoctx;

#pragma warning(disable: 6011 6387)

void videoii_loop(void);

void video_init(void) {
	videoctx = malloc(sizeof(videoctx_t));
	memset(videoctx, 0, sizeof(videoctx_t));
	
	videoctx->Status = DEVSTATUS_GOOD;

	PauseDrawing = 0;
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow("PLASM Emulator", 20, 20, 640, 480, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
}

void video_shutdown(void) {
	PauseDrawing = 1;
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void videoii_loop(void) {
	SDL_Event Event;
	byte Quit = 0;
	while (!Quit) {
		while (SDL_PollEvent(&Event)) {
			if (PauseDrawing)
				break;
			if (Event.type == SDL_QUIT)
				Quit = 1;
			if (Event.type == SDL_KEYDOWN)
				KeysDown[Event.key.keysym.scancode] = 1;
			if (Event.type == SDL_KEYUP)
				KeysDown[Event.key.keysym.scancode] = 0;
		}
		while (PauseDrawing)
			SDL_Delay(100);
	}
}