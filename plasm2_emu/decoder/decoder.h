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

byte decoderi_g1(void);
u64  decoderi_gx(byte HowMuch);

void decoder_go(byte Instruction);

void decoder_print(const char* Format);

typedef struct _dcctx {
	u64 SpeculativePointer; 
}dcctx_t;
extern dcctx_t* dcctx;
