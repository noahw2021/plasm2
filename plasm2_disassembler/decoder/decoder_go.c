//
//  decoder_go.c
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#include "decoder.h"
#include "../psin2/psin2.h"

BYTE DecoderGo(void) {
    if (DecoderCtx->VirtualIp >= DecoderCtx->DataLength)
        return 0;
    
    int InId = Psin2iGetInstructionByOpcode(
        DecoderRead1());
    
    DecoderPrint("%s ", Psin2iGetInstructionName(InId));
    
    int ArgumentCnt = Psin2iGetOperandCount(InId);
    switch (ArgumentCnt) {
        case 0:
            DecoderPrint("\n");
            break;
        case 1:
            if (Psin2iGetOperandType(InId, 0) == 0) { // R
                DecoderPrint("r%i\n", DecoderRead1());
            } else { // I
                DecoderPrint("%llX\n", DecoderReadX(
                    Psin2iGetPhysicalSize(InId, 0)));
            }
            break;
        case 2:
            for (int i = 0; i < 2; i++) {
                if (Psin2iGetOperandType(InId, i) == 0) { // R
                    if (Psin2iGetPhysicalSize(InId, i) == 4) {
                        union {
                            byte Byte;
                            struct {
                                byte r0 : 4;
                                byte r1 : 4;
                            };
                        }Registers;
                        Registers.Byte = DecoderRead1();
                        
                        DecoderPrint("r%i, r%i\n",
                            Registers.r0, Registers.r1);
                        break;
                    }
                    
                    DecoderPrint("r%i", DecoderRead1());
                } else { // I
                    DecoderPrint("%llX", DecoderReadX(
                        Psin2iGetPhysicalSize(InId, i)));
                }
                
                if (i == 0)
                    DecoderPrint(", ");
                else
                    DecoderPrint("\n");
            }
            break;
    }
    
    return 1;
}
