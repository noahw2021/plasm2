#pragma once
#include "../basetypes.h"
/*
decoder.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

// decoder, debugger, and disassembler

void decoder_init(void);
void decoder_shutdown(void);

byte decoderi_g1(void);
u64  decoderi_gx(byte How Much);

void decoder_go(byte Instruction);

typedef struct _dcctx {
	u64 SpeculativePointer; 
}dcctx_t;
extern dcctx_t* dctx;