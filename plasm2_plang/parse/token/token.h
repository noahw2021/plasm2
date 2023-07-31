#pragma once
#include "../../basetypes.h"
/*
token.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#define TOKEN_SPEC_BEGIN   0x0000

#define TOKEN_TYPE_FIRST   0x0100
#define TOKEN_TYPE_U64     0x0101 // u64
#define TOKEN_TYPE_U32     0x0102 // u32
#define TOKEN_TYPE_U16     0x0103 // u16
#define TOKEN_TYPE_BYTE    0x0104 // byte
#define TOKEN_TYPE_STR     0x0105 // str
#define TOKEN_TYPE_VOID    0x0106 // void

#define TOKEN_MODI_FIRST   0x0150
#define TOKEN_MODI_POINTER 0x0151 // *
#define TOKEN_MODI_SEMICLN 0x0152 // ;
#define TOKEN_MODI_STRQOT  0x0153 // "
#define TOKEN_MODI_CHARQOT 0x0154 // '
#define TOKEN_MODI_OPARENT 0x0155 // (
#define TOKEN_MODI_CPARENT 0x0156 // )
#define TOKEN_MODI_OBRAKET 0x0157 // {
#define TOKEN_MODI_CBRAKET 0x0158 // }
#define TOKEN_MODI_OHBRKT  0x0159 // [
#define TOKEN_MODI_CHBRKT  0x015A // ]
#define TOKEN_MODI_FSLASH  0x015B // /
#define TOKEN_MODI_BSLASH  0x015C // YOU KNOW
#define TOKEN_MODI_PIPE    0x015D // |
#define TOKEN_MODI_EXLCLAM 0x015E // !
#define TOKEN_MODI_PERCENT 0x015F // %
#define TOKEN_MODI_CARROT  0x0160 // ^
#define TOKEN_MODI_AMPRSND 0x0161 // &
#define TOKEN_MODI_ASTRISK 0x0162 // *
#define TOKEN_MODI_MINUS   0x0163 // -
#define TOKEN_MODI_PLUS    0x0164 // +
#define TOKEN_MODI_QUESTON 0x0165 // ?
#define TOKEN_MODI_LFTPS   0x0166 // <
#define TOKEN_MODI_RFTPS   0x0167 // >
#define TOKEN_MODI_TILDA   0x0168 // ~

#define TOKEN_SPEC_END     0xFFFF 

typedef struct _tokenctx {

}tokenctx_t;
extern tokenctx_t* tokenctx;

typedef struct _tokens {
	u32 TokenCount;
	u16* Tokens;
}tokens_t;

void token_init(void);
void token_shutdown(void);

void token_parseword(char* Word);