//
//  mmu_stack.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

void MmuPush(WORD64 Value) {
	while (InRange(ECtx->sp, ECtx->ControlRegisters.ReturnAddressLocation,
        ECtx->ControlRegisters.ReturnAddressLocation + 16)
    ) {
        ECtx->sp++;
    }
		
	WORD64* Stack = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory + 
        MmuTranslate(ECtx->sp, REASON_WRTE, 8));
    // PM usage good: (reason: virtual memory)
	Stack[1] = Value;
	if ((ECtx->sp + 8) <= ECtx->ControlRegisters.StackPointerUpperBound)
		ECtx->sp += 8;
    
    
	return;
}

WORD64 MmuPop(void) {
	WORD64* Stack = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory + 
        MmuTranslate(ECtx->sp, REASON_READ, 8));
    // PM usage good: (reason: virtual memory)
	WORD64 Return = Stack[0];
	if ((ECtx->sp - 8) >= ECtx->ControlRegisters.StackPointerLowerBound)
		ECtx->sp -= 8;

	return Return;
}
