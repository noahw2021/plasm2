//
//  decoder.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _decoder_h
#define _decoder_h

#include "../basetypes.h"

// decoder, debugger, and disassembler

void DecoderInit(void);
void DecoderShutdown(void);

BYTE DecoderRead1(void);
WORD64  DecoderReadX(BYTE HowMuch);

void DecoderGo(BYTE Instruction);

void DecoderPrint(const char* Format);

typedef struct _DCCTX {
	WORD64 SpeculativePointer;
}DC_CTX, *PDC_CTX;

extern PDC_CTX DcCtx;

#endif
