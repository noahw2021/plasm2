#include "token.h"
/*
token_data.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

tokenpair_t Tokens[] = {
	{TOKEN_TYPE_U64, 1, "u64"},
	{TOKEN_TYPE_U32, 1, "u32"},
	{TOKEN_TYPE_U16, 1, "u16"},
	{TOKEN_TYPE_BYTE, 1, "byte"},
	{TOKEN_TYPE_STR, 1, "str"},
	{TOKEN_TYPE_VOID, 1, "void"},

	{TOKEN_MODI_POINTER, 1, "*"},
	{TOKEN_MODI_SEMICLN, 1, ";"},
	{TOKEN_MODI_STRQOT, 1, "\""},
	{TOKEN_MODI_CHARQOT, 1, "'"},
	{TOKEN_MODI_OPARENT, 1, "("},
	{TOKEN_MODI_CPARENT, 1, ")"},
	{TOKEN_MODI_OBRAKET, 1, "{"},
	{TOKEN_MODI_CBRAKET, 1, "}"},
	{TOKEN_MODI_OHBRKT, 1, "["},
	{TOKEN_MODI_CHBRKT, 1, "]"},
	{TOKEN_MODI_FSLASH, 1, "/"},
	{TOKEN_MODI_BSLASH, 1, "\\"},
	{TOKEN_MODI_PIPE, 1, "|"},
	{TOKEN_MODI_EXLCLAM, 1, "!"},
	{TOKEN_MODI_PERCENT, 1, "%"},
	{TOKEN_MODI_CARROT, 1, "^"},
	{TOKEN_MODI_AMPRSND, 1, "&"},
	{TOKEN_MODI_MINUS, 1, "-"},
	{TOKEN_MODI_PLUS, 1, "+"},
	{TOKEN_MODI_QUESTON, 1, "?"},
	{TOKEN_MODI_LFTPS, 1, "<"},
	{TOKEN_MODI_RFTPS, 1, ">"},
	{TOKEN_MODI_TILDA, 1, "~"},
};