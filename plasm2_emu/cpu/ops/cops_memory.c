#include "../cpu.h"
#include "../mmu/mmu.h"
/*
cops_memory.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void __LDW(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Address : 4;
		};
	}Inputs;
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read8(VirtualAddress);
	return;
}

void __LDB(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Address : 4;
		};
	}Inputs; 
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_READ);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	i->rs_gprs[Inputs.Destination] = mmu_read1(VirtualAddress);
	return;
}

void __STW(void) {
	union {
		byte Input;
		struct {
			byte Address : 4;
			byte Register : 4;
		};
	}Inputs;
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put8(VirtualAddress, i->rs_gprs[Inputs.Register]);
	return;
}

void __STB(void) {
	union {
		byte Input;
		struct {
			byte Address : 4;
			byte Register : 4;
		};
	}Inputs; 
	u64 VirtualAddress = mmu_translate(i->rs_gprs[Inputs.Address], REASON_WRTE);
	if (!VirtualAddress) {
		i->flags_s.XF = 1;
		return;
	}
	mmu_put1(VirtualAddress, i->rs_gprs[Inputs.Register]);
	return;
}

void __PSH(void) {
	byte Register = mmu_read1() & 0xF;
	mmu_push(i->rs_gprs[Register]);
	return;
}

void __POP(void) {
	byte Register = mmu_read1() & 0xF;
	i->rs_gprs[Register] = mmu_pop();
	return;
}

void __PSR(void) {
	for (int i = 0; i < 16; i++)
		mmu_push(i->rs_gprs[i]);
	return;
}

void __POR(void) {
	for (int i = 0; i < 16; i++)
		i->rs_gprs[i] = mmu_pop();
	return;
}

void __VME(void) {
	i->flags_s.VF = 1;
	return;
}

void __VMD(void) {
	i->flags_s.VF = 0;
	return;
}

void __VPC(void) {
	if (!i->flags_s.VF)
		return;
	if (i->flags_s.AF) {
		if (i->security_s.SecurityLevel > 1)
			return;
	}

}

void __VPD(void) {

}

void __VSI(void) {

}

void __VSD(void) {

}

void __SPS(void) {

}

void __SPG(void) {

}

void __VSS(void) {

}

void __VES(void) {

}
