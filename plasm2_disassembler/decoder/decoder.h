//
//  decoder.h
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#ifndef decoder_h
#define decoder_h

#include "../basetypes.h"
#include <stdio.h>

typedef struct _SYMBOL_CTX *PSYMBOL_CTX;

typedef struct _DECODER_CTX {
    PSYMBOL_CTX SymbolInformation;
    BYTE* Data;
    WORD32 DataLength;
    WORD32 VirtualIp;
    
    FILE* OutputFile;
}DECODER_CTX, *PDECODER_CTX;
extern PDECODER_CTX DecoderCtx;

void DecoderInit(void);
void DecoderShutdown(void);

void DecoderSetSymbolFile(FILE* SymbolFile);
void DecoderSetData(BYTE* Data, WORD32 DataLength);
void DecoderSetOutputFile(FILE* OutFile);

BYTE DecoderGo(void);
void DecoderPrint(const char* Format, ...);

BYTE DecoderRead1(void);
WORD16 DecoderRead2(void);
WORD32 DecoderRead4(void);
WORD64 DecoderRead8(void);
WORD64 DecoderReadX(int Count);

#endif /* decoder_h */
