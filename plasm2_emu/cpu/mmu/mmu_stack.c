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

void mmu_push(WORD64 Value) {
	while(InRange(i->sp, i->pti.ral, i->pti.ral + 16))
		i->sp += 1;
	WORD64* Stack = (WORD64*)((BYTE*)cpuctx->PhysicalMemory + mmu_translate(i->sp, REASON_WRTE, 8)); // PM usage good: (reason: virtual memory)
	Stack[1] = Value;
	if ((i->sp + 8) <= i->pti.spb)
		i->sp += 8;
    
    
	return;
}

WORD64 mmu_pop(void) {
	WORD64* Stack = (WORD64*)((BYTE*)cpuctx->PhysicalMemory + mmu_translate(i->sp, REASON_READ, 8)); // PM usage good: (reason: virtual memory)
	WORD64 Return = Stack[0];
	if ((i->sp - 8) >= i->pti.slb)
		i->sp -= 8;

	return Return;
}
