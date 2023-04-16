#include "../cpu.h"
#include "../mmu/mmu.h"
#include "../../devices/devices.h"
/*
cops_devices.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void DSQ(void) {
	byte Register = mmu_read1() & 0xF;
	devicesi_statusquery(i->rs_gprs[Register] & 0xFFFFFFFF);
	return;
}

void DSC(void) {
	union {
		byte Input;
		struct {
			byte Device : 4;
			byte Command : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1();
	devicesi_sendcommand(i->rs_gprs[Inputs.Device], i->rs_gprs[Inputs.Command]);
	return;
}

void DSD(void) {
	union {
		byte Input;
		struct {
			byte Device : 4;
			byte Data : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1();
	devicesi_senddata(i->rs_gprs[Inputs.Device], i->rs_gprs[Inputs.Data]);
	return;
}

void DGD(void) {
	union {
		byte Input;
		struct {
			byte Device : 4;
			byte Destination : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1();
	i->rs_gprs[Inputs.Destination] = devicesi_getdata(i->rs_gprs[Inputs.Device]);
	return;
}

void DRS(void) {
	u32 Device = i->rs_gprs[mmu_read1() & 0xF];
	devicesi_reset(Device);
	return;
}

void DPE(void) {
	u32 Device = i->rs_gprs[mmu_read1() & 0xF];
	devicesi_on(Device);
	return;
}

void DPD(void) {
	u32 Device = i->rs_gprs[mmu_read1() & 0xF];
	devicesi_off(Device);
	return;
}

void DGC(void) {
	byte Register = mmu_read1() & 0xF;
	i->rs_gprs[Register] = devicesi_devcount();
	return;
}
