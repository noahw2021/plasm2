#define _CRT_SECURE_NO_WARNINGS
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include "token/token.h"
#include "../error/error.h"
#include "../cg/cg.h"
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

	byte FunctionType;
	char* PotFuncName;

	for (int i = 0; i < Primary->TokenCount; i++) {
		// variable declaration
		if (InRange(Primary->Tokens[i].Token, TOKEN_TYPE_FIRST, TOKEN_TYPE_LAST)) { // variable declaration
			if (Primary->Tokens[i].Token == TOKEN_TYPE_VOID && Primary->Tokens[i + 1].Token != TOKEN_MODI_POINTER) {
				error_send(ERR_SNTX_INVTYPE, parsectx->CurrentLine);
				goto FreeAll;
			}

			if (Primary->Tokens[i + 2].Token == TOKEN_MODI_OPARENT || Primary->Tokens[i + 3].Token == TOKEN_MODI_OPARENT)
				goto FunctionDeclaration;

			if (Primary->Tokens[i + 1].Token == TOKEN_MODI_POINTER) {
				byte Type = 0x00;
				switch (Primary->Tokens[i].Token) {
				case TOKEN_TYPE_BYTE:
					Type = CG_TYPE_PBYTE;
					break;
				case TOKEN_TYPE_STR:
					Type = CG_TYPE_PSTR;
					break;
				case TOKEN_TYPE_U16:
					Type = CG_TYPE_PU16;
					break;
				case TOKEN_TYPE_U32:
					Type = CG_TYPE_PU32;
					break;
				case TOKEN_TYPE_U64:
					Type = CG_TYPE_PU64;
					break;
				case TOKEN_TYPE_VOID:
					Type = CG_TYPE_PVOID;
					break;
				}

				if (Primary->Tokens[i + 2].Token != TOKEN_INTL_UNKOWN) {
					error_send(ERR_SNTX_VNEXPCT, parsectx->CurrentLine);
					goto FreeAll;
				}

				cg_decvar(Type, parsectx->CurrentCtx, Primary->Tokens[i + 2].String);

				i += 3;
				if (Primary->Tokens[i].Token != TOKEN_MODI_SEMICLN) {
					error_send(ERR_SNTX_SCEXPCT, parsectx->CurrentLine);
					goto FreeAll;
				}
			} else {
				byte Type = 0x00;
				switch (Primary->Tokens[i].Token) {
				case TOKEN_TYPE_BYTE:
					Type = CG_TYPE_BYTE;
					break;
				case TOKEN_TYPE_STR:
					Type = CG_TYPE_STR;
					break;
				case TOKEN_TYPE_U16:
					Type = CG_TYPE_U16;
					break;
				case TOKEN_TYPE_U32:
					Type = CG_TYPE_U32;
					break;
				case TOKEN_TYPE_U64:
					Type = CG_TYPE_U64;
					break;
				case TOKEN_TYPE_VOID:
					Type = CG_TYPE_VOID;
					break;
				}

				if (Primary->Tokens[i + 1].Token != TOKEN_INTL_UNKOWN) {
					error_send(ERR_SNTX_VNEXPCT, parsectx->CurrentLine);
					goto FreeAll;
				}

				cg_decvar(Type, parsectx->CurrentCtx, Primary->Tokens[i + 1].String);

				i += 2;
				if (Primary->Tokens[i].Token != TOKEN_MODI_SEMICLN) {
					error_send(ERR_SNTX_SCEXPCT, parsectx->CurrentLine);
					goto FreeAll;
				}
			}
		}
		// function declaration
	FunctionDeclaration:
		FunctionType = Primary->Tokens[i].Token;
		switch (FunctionType) {
		case TOKEN_TYPE_BYTE:
			FunctionType = CG_TYPE_BYTE;
			break;
		case TOKEN_TYPE_STR:
			FunctionType = CG_TYPE_STR;
			break;
		case TOKEN_TYPE_U16:
			FunctionType = CG_TYPE_U16;
			break;
		case TOKEN_TYPE_U32:
			FunctionType = CG_TYPE_U32;
			break;
		case TOKEN_TYPE_U64:
			FunctionType = CG_TYPE_U64;
			break;
		case TOKEN_TYPE_VOID:
			FunctionType = CG_TYPE_VOID;
			break;
		}
		if (Primary->Tokens[i + 1].Token == TOKEN_MODI_POINTER) {
			FunctionType |= 0x10; // pointer mask
			i++;
		}
		i++;
		if (Primary->Tokens[i].Token != TOKEN_INTL_UNKOWN) {
			error_send(ERR_SNTX_FNEXPCT, parsectx->CurrentLine);
			goto FreeAll;
		}
		PotFuncName = Primary->Tokens[i++].String;
		
		if (Primary->Tokens[i].Token != TOKEN_MODI_OPARENT) {
			error_send(ERR_SNTX_EXPBOAL, parsectx->CurrentLine);
			goto FreeAll;
		}


		// function call
		// variable manipulation
	}

FreeAll:
}