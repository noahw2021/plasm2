//
//  decoder_read.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include "decoder.h"

BYTE DecoderRead1(void) {
    return (BYTE)DecoderReadX(1);
}

WORD16 DecoderRead2(void) {
    return (WORD16)DecoderReadX(2);
}

WORD32 DecoderRead4(void) {
    return (WORD32)DecoderReadX(4);
}

WORD64 DecoderRead8(void) {
    return (BYTE)DecoderReadX(8);
}
WORD64 DecoderReadX(int Count) {
    union {
        WORD64 Word64;
        BYTE Word8[8];
    }Data;
    
    int Count2 = Count;
    if (Count2 > 8)
        Count2 = 8;
    
    for (int i = 0; i < Count2; i++)
        Data.Word8[i] = DecoderCtx->Data[DecoderCtx->VirtualIp++];
    
    return Data.Word64;
}
