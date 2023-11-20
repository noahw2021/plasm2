//
//  tools_fontgen.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "tools.h"
#include "../basetypes.h"
#include <SDL.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable: 6011)

#define SINGLE_GET(Single, Bit) (Single & (1LLU << Bit)) >> Bit
#define SINGLE_SET(Single, Bit) (Single |= (1LLU << Bit))
#define SINGLE_CLR(Single, Bit) (Single &= (~(1LLU << Bit)))

#define BIT_READ(Pair, Bit)  SINGLE_GET(Pair[Bit / 64], Bit % 64)
#define BIT_WRTE(Pair, Bit, State) SINGLE_CLR(Pair[Bit / 64], Bit % 64); SINGLE_SET(Pair[Bit / 64], Bit % 64)

int toolsi_renderthread(void* __a0) {
	return 0;
}

void toolsi_fontgen(void) {
	SDL_Init(SDL_INIT_VIDEO);
	
	char FileName[260];
	printf("PLASM2 Font Generator\n");
	printf("File name: ");
	fgets(FileName, 260, stdin);
	if (strstr(FileName, "\n"))
		((char*)strstr(FileName, "\n"))[0] = 0x00;
	
	int LastChar = 0;
	int CurrentChar = 0;
	int MousePos[2] = { 0, 0 };
	byte IsMouseDown[2] = { 0,0 };
	byte** Bitmap = malloc(sizeof(byte*) * 256);
	for (int i = 0; i < 256; i++) {
		Bitmap[i] = malloc(sizeof(byte) * 128);
		memset(Bitmap[i], 0, 128);
	}

	FILE* InFile = fopen(FileName, "rb");
	
	if (InFile) {
		fseek(InFile, 0, SEEK_END);
		u64 Dryer = ftell(InFile);
		if (Dryer != (256 * sizeof(u64) * 2)) {
			printf("Malforned font file.\n");
			fclose(InFile);
			for (int i = 0; i < 128; i++)
				free(Bitmap[i]);
			free(Bitmap);
			return;
		}

		fseek(InFile, 0, SEEK_SET);
		u64 InPair[2];

		for (int i = 0; i < 256; i++) {
			fread(InPair, sizeof(u64) * 2, 1, InFile);
			for (u64 b = 0; b < 128; b++)
				Bitmap[i][b] = BIT_READ(InPair, b);
		}
		fclose(InFile);
	}
	InFile = fopen(FileName, "wb");
	if (!InFile) {
		printf("Failed to open the file.\n");
		for (int i = 0; i < 128; i++)
			free(Bitmap[i]);
		free(Bitmap);
		return;
	}

	SDL_Window* FontWindow = SDL_CreateWindow("PLASM2 Font Generator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 257, 640, 0);
	SDL_Renderer* FontRenderer = SDL_CreateRenderer(FontWindow, -1, SDL_RENDERER_ACCELERATED);

	char Quit = 0;
	while (!Quit) {
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			if (Event.type == SDL_QUIT) {
				Quit = 1;
				break;
			}
			if (Event.type == SDL_MOUSEBUTTONDOWN) {
				if (Event.button.button == SDL_BUTTON_LEFT)
					IsMouseDown[0] = 1;
				else
					IsMouseDown[1] = 1;
			}
			if (Event.type == SDL_MOUSEBUTTONUP) {
				if (Event.button.button == SDL_BUTTON_LEFT)
					IsMouseDown[0] = 0;
				else
					IsMouseDown[1] = 0;
			}
			if (Event.type == SDL_MOUSEMOTION) {
				MousePos[0] = Event.motion.x;
				MousePos[1] = Event.motion.y;
			}

		}
		if (Quit)
			break;

		SDL_SetRenderDrawColor(FontRenderer, 0, 0, 0, 255);
		SDL_RenderClear(FontRenderer);

		SDL_SetRenderDrawColor(FontRenderer, 200, 200, 200, 255);
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 16; y++) {
				SDL_Rect Rectangle = { (x * 32) + 1, (y * 32) + 1, 31, 31 };
				if (Bitmap[CurrentChar][(y * 8) + x]) {
					SDL_SetRenderDrawColor(FontRenderer, 220, 220, 220, 255);
					SDL_RenderFillRect(FontRenderer, &Rectangle);
					SDL_SetRenderDrawColor(FontRenderer, 200, 200, 200, 255);
				}
				SDL_RenderDrawRect(FontRenderer, &Rectangle);
				if (InRange(MousePos[0], Rectangle.x, Rectangle.x + Rectangle.w)) {
					if (InRange(MousePos[1], Rectangle.y, Rectangle.y + Rectangle.h)) {
						if (IsMouseDown[0])
							Bitmap[CurrentChar][(y * 8) + x] = 1;
						if (IsMouseDown[1])
							Bitmap[CurrentChar][(y * 8) + x] = 0;
					}
				}
			}
		}
		for (int b = 0; b < 2; b++) {
			SDL_Rect Rectangle = { 1 + (1 * b) + (127 * b), 523, 128, 127 };
			SDL_SetRenderDrawColor(FontRenderer, 200, 200, 200, 255);
			SDL_RenderDrawRect(FontRenderer, &Rectangle);

			if (InRange(MousePos[0], Rectangle.x, Rectangle.x + Rectangle.w)) {
				if (InRange(MousePos[1], Rectangle.y, Rectangle.y + Rectangle.h) && IsMouseDown[0]) {
					if (b)
						CurrentChar++;
					else
						CurrentChar--;
					if (!InRange(CurrentChar, 0, 256))
						CurrentChar &= 0xFF;
				}
			}

			if (b) {
				SDL_RenderDrawLine(FontRenderer, Rectangle.x + 20, Rectangle.y + 20, Rectangle.x + 107, Rectangle.y + 64);
				SDL_RenderDrawLine(FontRenderer, Rectangle.x + 107, Rectangle.y + 64, Rectangle.x + 20, Rectangle.y + 97);
			} else {
				SDL_RenderDrawLine(FontRenderer, Rectangle.x + 20, Rectangle.y + 64, Rectangle.x + 107, Rectangle.y + 97);
				SDL_RenderDrawLine(FontRenderer, Rectangle.x + 20, Rectangle.y + 64, Rectangle.x + 107, Rectangle.y + 20);
			}
		}

		if (LastChar != CurrentChar) {
			printf("Current Char: 0x%02hX, '%c'\n", CurrentChar, CurrentChar);
			LastChar = CurrentChar;
			IsMouseDown[0] = 0;

			if (CurrentChar == 0xFF) {
				char Yn;
				printf("Are you finished? (y/n): ");
				Yn = fgetc(stdin);
				switch (Yn & (~0x20)) {
				case 'Y':
					Quit = 1;
					break;
				default:
					break;
				}
			}
		}

		SDL_RenderPresent(FontRenderer);
		SDL_Delay(10);
	}

	for (int i = 0; i < 256; i++) {
		u64 OutPairs[2] = { 0, 0 };
		for (int c = 0; c < 128; c++) {
			if (Bitmap[i][c]) {
				BIT_WRTE(OutPairs, c, 1);
			}
		}
		fwrite(OutPairs, sizeof(u64), 2, InFile);
	}	

	return;
}
