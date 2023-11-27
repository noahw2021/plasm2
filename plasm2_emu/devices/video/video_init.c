//
//  video_init.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../devices.h"
#include "../kb/kb.h"
#include "../../cpu/cpu.h"
#include "../../emu.h"
#include "video.h"
#include <string.h>
#include <SDL.h>

SDL_Window* Window;
SDL_Renderer* Renderer;
BYTE PauseDrawing;
PVIDEO_CTX VideoCtx;
SDL_Thread* LoopThread;

#pragma warning(disable: 6011 6387)

void VideoLoop(void);

void VideoInit(void) {
	VideoCtx = malloc(sizeof(VIDEO_CTX));
	memset(VideoCtx, 0, sizeof(VIDEO_CTX));
	
	VideoCtx->Status = DEVSTATUS_GOOD;

	PauseDrawing = 0;
    VideoLoop();
}

void VideoShutdown(void) {
	PauseDrawing = 1;
	SDL_DestroyRenderer(Renderer); 
	SDL_DestroyWindow(Window);
	SDL_DetachThread(LoopThread);
	SDL_Quit();
}

void VideoLoop(void) {
	SDL_Event Event;
	BYTE Quit = 0;
    
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("PLASM Emulator", 20, 20, 640, 480, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    
	while (!Quit) {
		while (SDL_PollEvent(&Event)) {
			if (PauseDrawing)
				break;
			if (Event.type == SDL_QUIT)
				Quit = 1;
			if (Event.type == SDL_KEYDOWN && !KbCtx->HoldUp) {
				KbiiMarkKeyState(Event.key.keysym.scancode, 1);
				KbCtx->NotifyDown = Event.key.keysym.scancode;
			}
			if (Event.type == SDL_KEYUP && !KbCtx->HoldUp) {
				KbiiMarkKeyState(Event.key.keysym.scancode, 0);
				KbCtx->NotifyUp = Event.key.keysym.scancode;
			}
			
		}
		if (!PauseDrawing)
			SDL_RenderPresent(Renderer);
		SDL_Delay(16);
	}
    
    return;
}
