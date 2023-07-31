#include "token.h"
#include <stdlib.h>
#include <string.h>
/*
token_parse.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6308 6387 26451 28182)

void tokeni_add(tokens_t* Main, u16 Token, char* String) {
	if (!Main->Tokens)
		Main->Tokens = malloc(sizeof(*Main->Tokens));
	else
		Main->Tokens = realloc(Main->Tokens, sizeof(*Main->Tokens) * (Main->TokenCount + 1));
	
	Main->Tokens[Main->TokenCount].String = String;
	Main->Tokens[Main->TokenCount].Token = Token;
	
	Main->TokenCount++;
	return;
}

tokens_t* token_parseword(char* Word) {
	tokens_t* Return = malloc(sizeof(tokens_t));
	memset(Return, 0, sizeof(tokens_t));

	char* LongestWord = malloc(64);
	int ItRoot = 0, ItLocal = 0;
	byte RealBreak = 0;
	while (Word[ItRoot]) {
		LongestWord[ItLocal++] = Word[ItRoot++];

		for (int i = 0; i < TOKEN_CNT; i++) {
			if (!Tokens[i].HasString)
				continue;
			if (!strcmp(Tokens[i].RelatedString, Word)) {
				tokeni_add(Return, Tokens[i].Token, Word);
				ItLocal = 0;
				RealBreak = 1;
				break;
			}
		}

		if (RealBreak) {
			RealBreak = 0;
			break;
		}
	}

	free(LongestWord);

	return Return;
}