#include "video.h"
#include "../emu.h"
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

typedef union _tmchar {
	byte Bytes[2];
	u16 UShort;

	struct {
		byte Character;
		union {
			byte Color;
			struct {
				byte r : 2;
				byte g : 2;
				byte b : 1;
				byte Blink : 1;
				byte Reserved : 1;
			};
		};
	};
}tmchar_t;
typedef tmchar_t TMLine[80];
typedef TMLine TMLines[50];
u16 LineHashes[50];
TMLines* WindowBuffer;

u16 videoif_genlinehash(TMLine* Line) {
	TMLine TheLine = *Line;

	u16 Hash = 0x0000;
	for (int i = 0; i < 80; i++) {
		Hash += TheLine[i].Character;
		Hash ^= TheLine[i].Character - (Hash & 0x1FFB);
	}

	return Hash;
}

typedef struct _linenug {
	byte StrLen;
	char* String;
	union {
		byte Color;
		struct {
			byte r : 2;
			byte g : 2;
			byte b : 1;
			byte Blink : 1;
			byte Reserved : 1;
		};
	};
}linenug_t; // line nugget

typedef struct _linenugs {
	linenug_t* Nuggets;
	u16 NuggetCount;
}linenugs_t;
linenugs_t* __pls_free; // horrible practice huh

void videof_shutdown(void) {
	for (int i = 0; i < 80; i++) {
		free(__pls_free->Nuggets[i].String);
		free(__pls_free->Nuggets[i]);
	}
	free(__pls_free); // i can assure this is much better than what wasn't published...

	return;
}

linenugs_t* videoif_getlinestr(TMLine* Line) {
	TMLine TheLine = *Line;
	
	static linenugs_t* Nuggets = NULL;
	if (!Nuggets) {
		Nuggets = malloc(sizeof(linenugs_t));
		Nuggets->NuggetCount = 0;
		for (int i = 0; i < 80; i++) {
			Nuggets->Nuggets[i] = malloc(sizeof(linenug_t));
			Nuggets->Nuggets[i].String = malloc(80);
		}

		__pls_free = Nuggets; // :p
	}

	byte LastColor = 0x00;

	for (int i = 0; i < 80; i++) {
		if (TheLine[i].Color != LastColor) {
			Nuggets->Nuggets = realloc(Nuggets->Nuggets, (sizeof(linenug_t * Nuggets->NuggetCount));
			Nuggets->Nuggets[Nuggets->NuggetCount].Color = TheLine[i].Color;
			Nuggets->Nuggets[Nuggets->NuggetCount].String[0] = TheLine[i].Character;
			Nuggets->Nuggets[i].StrLen = 1;
			Nuggets->NuggetCount++;
		} else {
			Nuggets->Nuggets[Nuggets->NuggetCount - 1].String[Nuggets->Nuggets[Nuggets->NuggetCount - 1].StrLen] = TheLine[i].Character;
			Nuggets->Nuggets[Nuggets->NuggetCount - 1].StrLen++;
		}
	}
}

void video_clock(void) {
	if (videoctx->TextMode) {
		for (int i = 0; i < 50; i++) {
			u16 LineHash = videoif_genlinehash(&videoctx->textmode->Lines[i]);
			if (LineHashes[i] != LineHash) {
				LineHashes[i] = LineHash;

				// draw line
				byte TrackingX = 0;
				linenugs_t* TheNuggets = videoif_getlinestr(&videoctx->textmode->Lines[i]);
				for (int f = 0; f < TheNuggets->NuggetCount; f++) {
					SDL_Color TheColor;
					TheColor.r = (TheNuggets->Nuggets[f].r * 64) & 0xFF;
					TheColor.g = (TheNuggets->Nuggets[f].g * 64) & 0xFF;
					TheColor.b = (TheNuggets->Nuggets[f].b * 64) & 0xFF;

					if (TheNuggets->Nuggets[f].Blink && !videoctx->BlinkOff) {
						SDL_Surface* Line = TTF_RenderText_Blended(SdlFont, TheNuggets->Nuggets[f].String, TheColor);
						SDL_Texture* TLine = SDL_CreateTextureFromSurface(Renderer, Line);

						PauseDrawing = 1;
						SDL_Rect DestinationRect = {(TrackingX * 8), (i * 16), Line->w, Line->h)};
						SDL_RenderCopy(Renderer, TLine, NULL, &DestinationRect);
						SDL_FreeSurface(Line);
						SDL_DestroyTexture(TLine);
						PauseDrawing = 0;
					}

					TrackingX += TheNuggets->Nuggets[f].StrLen;
				}
				
			} else {
				continue;
			}
		}
	}
	else {
		videof_clock();
	}
	return;
}

void videof_init(void) {
	TTF_Init();
	SdlFont = TTF_OpenFont("resources\\vgafon.ttf", 12);
	if (!SdlFont) {
		emu_register_fatal("Failed to load the textmode font.");
		return;
	}

	return;
}