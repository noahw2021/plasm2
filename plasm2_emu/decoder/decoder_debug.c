#include "decoder.h"
#include "../cpu/mmu/mmu.h"
#include "../psin2/psin2.h"

/*
decoder_debug.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

byte decoderi_g1(void) {
	byte Return = mmu_read1(dcctx->SpeculativePointer);
	dcctx->SpeculativePointer += 1;
	return Return;
}

u64  decoderi_gx(byte HowMuch) {
	u64 Returns = mmu_readx(dcctx->SpeculativePointer, HowMuch);
	dcctx->SpeculativePointer += HowMuch;
	return Returns;
}

void decoder_go(byte Instruction) {
	int Psin2Id = psin2i_getinstructionbycd(Instruction);	
	/*
	if debugger is disabled, the cpu does no opcode checking by default (yet)
	as of 7/21/23. this may be implemented in the future, but for now there will
	be no error checking within the debugger. this may seem bad, but it should 
	encourage me to get on error checking within the cpu, as this debugger may
	be slow and shouldn't be functioning all the time. thus, we will not do
	any checking and pretend everything is all good, all the time. until the system
	burns.
	*/

	BOOL TwoArgsOneByte = TRUE;
	BOOL IsOperandRegister[2] = { FALSE, FALSE };
	u64  OperandValues[2] = { 0, 0 };

	for (int i = 0; i < psin2i_getoperandcnt(Psin2Id); i++) {
		if (psin2i_getphyssize(i) != 4)
			TwoArgsOneByte = FALSE;
	}

	if (TwoArgsOneByte) {
		union {
			byte Raw;
			struct {
				BYTE r0 : 4;
				BYTE r1 : 4;
			};
		}InputData;
		InputData.Raw = decoderi_g1();
		IsOperandRegister[0] = TRUE;
		IsOperandRegister[1] = TRUE;

		OperandValues[0] = InputData.r0;
		OperandValues[1] = InputData.r1;
	} else {
		for (int i = 0; i < psin2i_getoperandcnt(i); i++) {

		}
	}
}