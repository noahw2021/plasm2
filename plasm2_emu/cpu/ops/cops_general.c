#include "../cpu.h"
#include "../mmu/mmu.h"
#include "../emu.h"
/*
cops_general.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void MOV(void) {
	union {
		byte Byte;
		struct {
			byte Source : 4;
			byte Dest : 4;
		};
	}Input;
	Input.Byte = mmu_read1(i->ip++);
	i->rs_gprs[Input.Dest] = i->rs_gprs[Input.Source];
	return;
}

void LDI(void) {
	byte Destination = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	i->rs_gprs[Destination] = Immediate;
	return;
}

void JMP(void) {
	byte Address = mmu_read1(i->ip++) & 0xF;
	u64 PhysicalAddress = mmu_translate(i->rs_gprs[Address], REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (PhysicalAddress)
		cpui_inst_jmp(PhysicalAddress);
	else
		i->flags_s.XF = 1;
	return;
}

void NXC(void) {
	i->flags_s.NF = 0;
	return;
}

void NXE(void) {
	i->flags_s.NF = !i->flags_s.EF;
	i->flags_s.EF = 0;
	return;
}

void NXZ(void) {
	i->flags_s.NF = !i->flags_s.ZF;
	i->flags_s.ZF = 0;
	return;
}

void NXG(void) {
	i->flags_s.NF = !i->flags_s.GF;
	i->flags_s.GF = 0;
	return;
}

void NXL(void) {
	i->flags_s.NF = !i->flags_s.LF;
	i->flags_s.LF = 0;
	return;
}

void CLL(void) {
	byte Register = mmu_read1(i->ip++) & 0xF;
	u64 Address = i->rs_gprs[Register];
	u64 PhysicalAddress = mmu_translate(Address, REASON_EXEC | REASON_READ, SIZE_WATCHDOG);
	if (PhysicalAddress)
		cpui_inst_cll(PhysicalAddress);
	else
		i->flags_s.XF = 1;
	return;
}

void RET(void) {
	cpui_inst_ret();
}

void IMR(void) {
	u16 ReturnValue = (u16)mmu_readx(i->ip, 2);
	i->ip += 2;
	i->r0 = ReturnValue;
	RET(); // reuse
	return;
}

void SHF(void) {
	i->flags_s.HF = 1;
	return;
}

void CMP(void) { // __CMP = 0x0C, // CMP 0C (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare
	i->flags_s.EF = 0;
	i->flags_s.GF = 0;
	i->flags_s.LF = 0;

	union {
		byte Byte;
		struct {
			byte r2 : 4;
			byte r1 : 4;
		};
	}Input;
	Input.Byte = mmu_read1(i->ip++);

	if (i->rs_gprs[Input.r1] > i->rs_gprs[Input.r2])
		i->flags_s.GF = 1;
	if (i->rs_gprs[Input.r1] == i->rs_gprs[Input.r2])
		i->flags_s.EF = 1;
	if (i->rs_gprs[Input.r2] < i->rs_gprs[Input.r2])
		i->flags_s.LF = 1;
	
	return;
}

void JMI(void) {
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	u64 Translated = mmu_translate(Immediate, REASON_READ | REASON_EXEC, SIZE_WATCHDOG);
	if (Translated)
		cpui_inst_jmp(Translated);
	else
		i->flags_s.XF = 1;
	return;
}

void CLI(void) {
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;
	u64 Translated = Immediate;
	if (Translated)
		cpui_inst_cll(Translated);
	else
		i->flags_s.XF = 1;
	return;
}

void CMI(void) {
	i->flags_s.EF = 0;
	i->flags_s.GF = 0;
	i->flags_s.LF = 0;

	byte Register = mmu_read1(i->ip++) & 0xF;
	u64 Immediate = mmu_read8(i->ip);
	i->ip += 8;

	if (i->rs_gprs[Register] > Immediate)
		i->flags_s.GF = 1;
	if (i->rs_gprs[Register] < Immediate)
		i->flags_s.LF = 1;
	if (i->rs_gprs[Register] == Immediate)
		i->flags_s.EF = 1;

	return;
}

void CLR(void) {
	cpui_inst_clr();
}

void DBN(void) {
	emuctx->DebuggerEnabled = 1;
}

void DBF(void) {
	emuctx->DebuggerEnabled = 0;
}

void DBB(void) {
	cpui_inst_break();
}