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
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	WORD64 Status = DevicesiStatusQuery((WORD32)ECtx->GPRs[Register] & 
        0xFFFFFFFF);
	MmuPush(Status);
	return;
}

void DSC(void) {
	union {
		BYTE Input;
		struct {
			BYTE Command : 4;
			BYTE Device : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(ECtx->ip++);
	DevicesiSendCommand((WORD32)ECtx->GPRs[Inputs.Device], 
        ECtx->GPRs[Inputs.Command]);
	return;
}

void DSD(void) {
	union {
		BYTE Input;
		struct {
			BYTE Data : 4;
			BYTE Device : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(ECtx->ip++);
	DevicesiSendData((WORD32)ECtx->GPRs[Inputs.Device], 
        ECtx->GPRs[Inputs.Data]);
	return;
}

void DGD(void) {
	union {
		BYTE Input;
		struct {
			BYTE Destination : 4;
			BYTE Device : 4;
		};
	}Inputs;
	Inputs.Input = MmuRead1(ECtx->ip++);
	ECtx->GPRs[Inputs.Destination] = 
        DevicesiGetData((WORD32)ECtx->GPRs[Inputs.Device]);
	return;
}

void DRS(void) {
	WORD64 Device = ECtx->GPRs[MmuRead1(ECtx->ip++) & 0xF];
	DevicesiReset((WORD32)Device);
	return;
}

void DPE(void) {
	WORD64 Device = ECtx->GPRs[MmuRead1(ECtx->ip++) & 0xF];
	DevicesiOn((WORD32)Device);
	return;
}

void DPD(void) {
	WORD64 Device = ECtx->GPRs[MmuRead1(ECtx->ip++) & 0xF];
	DevicesiOff((WORD32)Device);
	return;
}

void DGC(void) {
	BYTE Register = MmuRead1(ECtx->ip++) & 0xF;
	ECtx->GPRs[Register] = DevicesiDevCount();
	return;
}
