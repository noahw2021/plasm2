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
	while(InRange(i->sp, i->ControlRegisters.ReturnAddressLocation, i->ControlRegisters.ReturnAddressLocation + 16))
		i->sp += 1;
	WORD64* Stack = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory + MmuTranslate(i->sp, REASON_WRTE, 8)); // PM usage good: (reason: virtual memory)
	Stack[1] = Value;
	if ((i->sp + 8) <= i->ControlRegisters.StackPointerUpperBound)
		i->sp += 8;
    
    
	return;
}

WORD64 MmuPop(void) {
	WORD64* Stack = (WORD64*)((BYTE*)CpuCtx->PhysicalMemory + MmuTranslate(i->sp, REASON_READ, 8)); // PM usage good: (reason: virtual memory)
	WORD64 Return = Stack[0];
	if ((i->sp - 8) >= i->ControlRegisters.StackPointerLowerBound)
		i->sp -= 8;

	return Return;
}
