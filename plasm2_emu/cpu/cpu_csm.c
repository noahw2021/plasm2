#include "cpu.h"
#include "mmu/mmu.h"
#include <stdlib.h>
#include <string.h>
/*
cpu_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void cpui_csm_set(u64 Handler) {
	if (i->security_s.SecurityLevel < 2)
		i->pti.csm = Handler;
	else
		i->flags_s.XF = 1;
	return;
}
void cpui_csm_msg(byte Code, u64 AddtData) {
	if (i->security_s.SecurityLevel >= 3)
		i->flags_s.XF = 1;
	for (int c = 0; c < REGCOUNT_SPEC; c++)
		mmu_push(i->rs_spec[c]);
	mmu_push(Code);
	mmu_push(AddtData);

	return;
}