#include "../cpu.h"
#include "mmu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
mmu_virt.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void mmu_push(u64 Value) {
	while(InRange(i->sp, i->pti.ral, i->pti.ral + 16))
		i->sp += 1;
	u64* Stack = (u64*)((byte*)cpuctx->PhysicalMemory + i->sp);
	Stack[1] = Value;
	if ((i->sp + 8) < i->pti.spb)
		i->sp += 8;
	return;
}
u64 mmu_pop(void) {
	u64* Stack = (u64*)((byte*)cpuctx->PhysicalMemory + i->sp);
	u64 Return = Stack[0];
	if ((i->sp - 8) > i->pti.slb)
		i->sp -= 8;
	return Return;
}