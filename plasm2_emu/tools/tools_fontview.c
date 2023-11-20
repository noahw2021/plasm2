//
//  tools_fontview.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "tools.h"
#include "../basetypes.h"
#include <SDL.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable: 6011 6387)
#define SINGLE_GET(Single, Bit) (Single & (1LLU << Bit)) >> Bit
#define BIT_READ(Pair, Bit)  SINGLE_GET(Pair[Bit / 64], Bit % 64)

void toolsi_fontview(void) {
	printf("PLASM2 Font Viewer\n");
	char InputBuffer[260];
	printf("Please input the font name: ");
	fgets(InputBuffer, 260, stdin);
	if (strstr(InputBuffer, "\n"))
		((char*)strstr(InputBuffer, "\n"))[0] = 0x00;

	FILE* InFile = fopen(InputBuffer, "rb");
	if (!InFile) {
		printf("Failed to get file.\n");
		return;
	}
	fseek(InFile, 0, SEEK_END);
	u32 FileSize = ftell(InFile);
	if (FileSize != (sizeof(u64) * 2 * 256)) {
		fclose(InFile);
		printf("Malformed font file.\n");
		return;
	}
	
	byte** Bitmap = malloc(sizeof(byte*) * 256);
	for (int i = 0; i < 256; i++) {
		u64 InMaps[2] = { 0, 0 };
		fseek(InFile, (2 * i * sizeof(u64)), SEEK_SET);
		fread(InMaps, sizeof(u64), 2, InFile);

		Bitmap[i] = malloc(128);
		for (int b = 0; b < 128; b++)
			Bitmap[i][b] = BIT_READ(InMaps, b);
	}
	fclose(InFile);

	SDL_Window* Window = SDL_CreateWindow("PLASM2 Font Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 257, 690, 0);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	int MousePos[2] = { 0, 0 };
	byte ClickState[2] = { 0,0 };
	int CurrentChar = 0x00;

	byte Break = 0;
	while (!Break) {
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			if (Event.type == SDL_QUIT) {
				Break = 1;
				break;
			}

			if (Event.type == SDL_MOUSEMOTION) {
				MousePos[0] = Event.motion.x;
				MousePos[1] = Event.motion.y;
			}
			
			if (Event.type == SDL_MOUSEBUTTONDOWN)
				ClickState[(Event.button.button == SDL_BUTTON_LEFT) ? 0 : 1] = 1;

			if (Event.type == SDL_MOUSEBUTTONUP)
				ClickState[(Event.button.button == SDL_BUTTON_LEFT) ? 0 : 1] = 0;
		}

		if (Break)
			break;
	
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);

		SDL_SetRenderDrawColor(Renderer, 200, 200, 200, 255);
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 16; y++) {
				SDL_Rect Rectangle = { (x * 32) + 1, (y * 32) + 1, 31, 31 };
				if (Bitmap[CurrentChar][(y * 8) + x]) {
					SDL_SetRenderDrawColor(Renderer, 220, 220, 220, 255);
					SDL_RenderFillRect(Renderer, &Rectangle);
				}
				SDL_SetRenderDrawColor(Renderer, 200, 200, 200, 255);
				SDL_RenderDrawRect(Renderer, &Rectangle);
			}
		}
		for (int b = 0; b < 2; b++) {
			SDL_Rect Rectangle = { 1 + (2 * b) + (127 * b), 523, 128 - (2 * b), 127  };
			SDL_SetRenderDrawColor(Renderer, 200, 200, 200, 255);
			SDL_RenderDrawRect(Renderer, &Rectangle);

			if (InRange(MousePos[0], Rectangle.x, Rectangle.x + Rectangle.w)) {
				if (InRange(MousePos[1], Rectangle.y, Rectangle.y + Rectangle.h) && ClickState[0]) {
					if (b)
						CurrentChar++;
					else
						CurrentChar--;
					if (!InRange(CurrentChar, 0, 256))
						CurrentChar &= 0xFF;

					ClickState[0] = 0;
				}
			}

			if (b) {
				SDL_RenderDrawLine(Renderer, Rectangle.x + 20, Rectangle.y + 20, Rectangle.x + 107, Rectangle.y + 64);
				SDL_RenderDrawLine(Renderer, Rectangle.x + 107, Rectangle.y + 64, Rectangle.x + 20, Rectangle.y + 97);
			}
			else {
				SDL_RenderDrawLine(Renderer, Rectangle.x + 20, Rectangle.y + 64, Rectangle.x + 107, Rectangle.y + 97);
				SDL_RenderDrawLine(Renderer, Rectangle.x + 20, Rectangle.y + 64, Rectangle.x + 107, Rectangle.y + 20);
			}
		}

		for (int i = 0; i < 15; i++) {
			byte YkChar = ((CurrentChar - 7) + i) % 256;
			SDL_Rect Rectangle = { 1 + (17 * i), 655, 16 + (i == 14), 32 };
			if (YkChar == CurrentChar)
				SDL_SetRenderDrawColor(Renderer, 155, 255, 150, 255);
			else
				SDL_SetRenderDrawColor(Renderer, 200, 200, 200, 200);
			SDL_RenderDrawRect(Renderer, &Rectangle);

			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 16; y++) {
					if (Bitmap[YkChar][(y * 8) + x]) {
						SDL_Rect ThisRect = { Rectangle.x + (x * 2), Rectangle.y + (y * 2), 2, 2 };
						SDL_RenderFillRect(Renderer, &ThisRect);
					}
				}
			}

			if (InRange(MousePos[0], Rectangle.x, Rectangle.x + Rectangle.w)) {
				if (InRange(MousePos[1], Rectangle.y, Rectangle.y + Rectangle.h) && ClickState[0]) {
					CurrentChar = YkChar;
				}
			}
		}

		SDL_RenderPresent(Renderer);
		SDL_Delay(10);
	}

	for (int i = 0; i < 256; i++)
		free(Bitmap[i]);
	free(Bitmap);

	return;
}
