#define _CRT_SECURE_NO_WARNINGS
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include "token/token.h"
#include "../error/error.h"
/*
parse_line.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6308 6386 6386 6387)

void parse_line(const char* Line) {
	char** Words = malloc(sizeof(char*) * 64);
	int WordCounts = 0, CurrentMax = 64;

	char* DelimitedBySpace = strtok(Line, " ");
	while (DelimitedBySpace) {
		if (WordCounts > (CurrentMax - 1)) {
			CurrentMax += 64;
			Words = realloc(Words, (sizeof(char*) * CurrentMax));
		}
		Words[WordCounts] = malloc(strlen(DelimitedBySpace) + 1);
		strcpy(Words[WordCounts], DelimitedBySpace);
		WordCounts++;
		DelimitedBySpace = strtok(NULL, " ");
	}

	if (!WordCounts) {
		free(Words);
		return;
	}

	tokens_t* Primary = token_parseword(Words[0]);
	for (int i = 1; i < WordCounts; i++) {
		tokens_t* New = token_parseword(Words[i]);
		token_cat(Primary, New);
		free(New->Tokens);
		free(New);
	}

	for (int i = 0; i < Primary->TokenCount; i++) {
		if (InRange(Primary->Tokens[i].Token, TOKEN_TYPE_FIRST, TOKEN_TYPE_LAST)) {
			if (Primary->Tokens[i].Token == TOKEN_TYPE_VOID && Primary->Tokens[i + 1].Token != TOKEN_MODI_POINTER) {
				
			}
		}
	}
}