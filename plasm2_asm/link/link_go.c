//
//  link_go.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "link.h"
#include "../cg/cg.h"
#include "../symbols/symbols.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern FILE* PrimaryOutput;
#pragma warning(disable: 6001 6386 6387)

void LinkGo(void) {
	for (int s = 0; s < LinkCtx->SymbolCount; s++) {
		if (!LinkCtx->Symbols[s].Resolved) {
			char* ErrorMsg = malloc(512);
			sprintf(ErrorMsg, "[E5000]: Unresolved symbol '%s'.", LinkCtx->Symbols[s].SymbolName);
			CgeError(0xFFFF, ErrorMsg);
			free(ErrorMsg);
		}

        WORD32 VariableCount = 0, FunctionCount = 0;
		for (int l = 0; l < LinkCtx->Symbols[s].LocationCount; l++) {
			fseek(PrimaryOutput, (WORD32)LinkCtx->Symbols[s].Locations[l], SEEK_SET);
			// linkctx->Symbols[s].Resolution; // += cgctx->ReferencePtr; WHY??
			fwrite(&LinkCtx->Symbols[s].Resolution, 8, 1, PrimaryOutput);
            
            BYTE ThisOpcode = LinkCtx->Symbols[s].LocationOpcodes[l];
            
            switch (ThisOpcode) {
                case 0x02:
                case 0x08:
                case 0x0F:
                case 0x11:
                case 0x91:
                    FunctionCount++;
                    break;
                default:
                    VariableCount++;
                    break;
            }
		}
        
        WORD64 BestEndingSize = 0xFFFFFFFFFFFFFFFF;
        for (int s2 = 0; s2 < LinkCtx->SymbolCount; s2++) {
            if (LinkCtx->Symbols[s2].Resolution > LinkCtx->Symbols[s].Resolution) {
                WORD64 Difference = LinkCtx->Symbols[s2].Resolution - 
                    LinkCtx->Symbols[s].Resolution;
                
                if (Difference < BestEndingSize)
                    BestEndingSize = Difference;
            }
        }
        
        if (BestEndingSize == 0xFFFFFFFFFFFFFFFF)
            BestEndingSize = CgCtx->HighestPosition - LinkCtx->Symbols[s].Resolution;
        
        WORD64 SymbolTypePrefix = 0;
        if (FunctionCount > (VariableCount / 4) ||
            FunctionCount == VariableCount
        ) { // Function
            SymbolsCreateFunction(LinkCtx->Symbols[s].Resolution, BestEndingSize,
                LinkCtx->Symbols[s].SymbolName);
        } else if ((FunctionCount + VariableCount) == 0) { // Unused
            SymbolsCreateUnused(LinkCtx->Symbols[s].Resolution, BestEndingSize,
                LinkCtx->Symbols[s].SymbolName);
        } else { // Variable
            SymbolsCreateVariable(LinkCtx->Symbols[s].Resolution, BestEndingSize,
                LinkCtx->Symbols[s].SymbolName);
        }
        
        
        
        //  @TODO why are we not freeing this??? - nw 11/21/23
		//if (LinkCtx->Symbols[s].Locations)
			//free(linkctx->Symbols[s].Locations);
		LinkCtx->Symbols[s].LocationCount = 0;
	}

	return;
}
