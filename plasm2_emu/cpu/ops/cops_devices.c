//
//  cops_device.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "../cpu.h"
#include "../mmu/mmu.h"
#include "../../devices/devices.h"

void DSQ(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	u64 Status = devicesi_statusquery((u32)i->rs_gprs[Register] & 0xFFFFFFFF);
	mmu_push(Status);
	return;
}

void DSC(void) {
	union {
		byte Input;
		struct {
			byte Command : 4;
			byte Device : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	devicesi_sendcommand((u32)i->rs_gprs[Inputs.Device], i->rs_gprs[Inputs.Command]);
	return;
}

void DSD(void) {
	union {
		byte Input;
		struct {
			byte Data : 4;
			byte Device : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	devicesi_senddata((u32)i->rs_gprs[Inputs.Device], i->rs_gprs[Inputs.Data]);
	return;
}

void DGD(void) {
	union {
		byte Input;
		struct {
			byte Destination : 4;
			byte Device : 4;
		};
	}Inputs;
	Inputs.Input = mmu_read1(i->ip++);
	i->rs_gprs[Inputs.Destination] = devicesi_getdata((u32)i->rs_gprs[Inputs.Device]);
	return;
}

void DRS(void) {
	u64 Device = i->rs_gprs[mmu_read1(i->ip++) & 0xF];
	devicesi_reset((u32)Device);
	return;
}

void DPE(void) {
	u64 Device = i->rs_gprs[mmu_read1(i->ip++) & 0xF];
	devicesi_on((u32)Device);
	return;
}

void DPD(void) {
	u64 Device = i->rs_gprs[mmu_read1(i->ip++) & 0xF];
	devicesi_off((u32)Device);
	return;
}

void DGC(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	i->rs_gprs[Register] = devicesi_devcount();
	return;
}
