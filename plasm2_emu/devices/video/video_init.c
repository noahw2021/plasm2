#include "../devices.h"
#include "../kb/kb.h"
#include "../../cpu/cpu.h"
#include "video.h"
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

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
SDL_Thread* LoopThread;


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
	LoopThread = SDL_CreateThread(videoii_loop, "Plasm2EmuSDLLoop", NULL);

	videof_init();

	strcpy(&devicesctx->Devices[devicesctx->DeviceCount].DeviceModel, "Standard VSA");
	strcpy(&devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "Display Adapter");
	strcpy(&devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "plasm2 Vendor");

	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 0x1000200030004001;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_VIDEO;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 0x0010020030040567;

	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = video_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = video_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = video_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = video_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = video_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = video_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = video_on;

	devicesctx->DeviceCount++;
}

void video_shutdown(void) {
	videof_shutdown();
	PauseDrawing = 1;
	SDL_DestroyRenderer(Renderer); 
	SDL_DestroyWindow(Window);
	SDL_DetachThread(LoopThread);
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
			if (Event.type == SDL_KEYDOWN && !kbctx->HoldUp) {
				kbii_markkey(Event.key.keysym.scancode, 1);
				kbctx->NotifyDown = Event.key.keysym.scancode;
			}
			if (Event.type == SDL_KEYUP && !kbctx->HoldUp) {
				kbii_markkey(Event.key.keysym.scancode, 0);
				kbctx->NotifyUp = Event.key.keysym.scancode;
			}
			
		}
		while (PauseDrawing)
			SDL_Delay(16);
		
		SDL_RenderPresent(Renderer);
	}
}