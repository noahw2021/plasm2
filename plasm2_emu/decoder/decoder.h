//
//  decoder.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../basetypes.h"

// decoder, debugger, and disassembler

void decoder_init(void);
void decoder_shutdown(void);

BYTE decoderi_g1(void);
WORD64  decoderi_gx(BYTE HowMuch);

void decoder_go(BYTE Instruction);

void decoder_print(const char* Format);

typedef struct _dcctx {
	WORD64 SpeculativePointer; 
}dcctx_t;
extern dcctx_t* dcctx;
