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

u64 mmu_translate(u64 VirtualAddress, byte Reason) {
	if (i->flags_s.VF) {
		VirtualAddress &= (PHYS_MEMSZ - 1); // temporary
		return VirtualAddress;
	} else {
		VirtualAddress &= (PHYS_MEMSZ - 1);
		return VirtualAddress;
	}
}