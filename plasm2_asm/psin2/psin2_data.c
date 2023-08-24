#pragma once
#include "psin2.h"
/*
psin2_data.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void psin2_load(void) {
	psin2_parse("__LDI = 0x01, // LDI 01 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Load Immediate");
	psin2_parse("__JMP = 0x02, // JMP 02 (R:04,08 ___ADR)                  16 : Jump");
	psin2_parse("__NXC = 0x03, // NXC 03                                   08 : Next Execute Clear");
	psin2_parse("__NXE = 0x04, // NXE 04                                   08 : Next Execute If Equal");
	psin2_parse("__NXZ = 0x05, // NXZ 05                                   08 : Next Execute If Zero");
	psin2_parse("__NXG = 0x06, // NXG 06                                   08 : Next Execute If Greater");
	psin2_parse("__NXL = 0x07, // NXL 07                                   08 : Next Execute If Less");
	psin2_parse("__CLL = 0x08, // CLL 08 (R:04,08 ___ADR)                  16 : Call");
	psin2_parse("__RET = 0x09, // RET 09                                   08 : Return");
	psin2_parse("__IMR = 0x0A, // IMR 0A (I:16,16 _VALUE)                  24 : Immediate Return");
	psin2_parse("__SHF = 0x0B, // SHF 0B                                   08 : Set Halt Flags");
	psin2_parse("__CMP = 0x0C, // CMP 0C (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare");
	psin2_parse("__CMP = 0x0C, // CMP 0C (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare");
	psin2_parse("__JMI = 0x0D, // JMI 0D (I:64,64 __ADDR)                  72 : Jump Immediate");
	psin2_parse("__CLI = 0x0E, // CLI 0E (I:64,64 __ADDR)                  72 : Call Immediate");
	psin2_parse("__CMI = 0x0F, // CMI 0F (R:04,08 _CMPA0) (I:64,64 _CMPA1) 80 : Compare Against Immediate");

	psin2_parse("// Arithmetic Instructions");
	psin2_parse("__ADD = 0x20, // ADD 20 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Add");
	psin2_parse("__SUB = 0x21, // SUB 21 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Subtract");
	psin2_parse("__MUL = 0x22, // MUL 22 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Multiply");
	psin2_parse("__DIV = 0x23, // DIV 23 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Divide");
	psin2_parse("__MOD = 0x24, // MOD 24 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Modulo");
	psin2_parse("__INC = 0x25, // INC 25 (R:04,08 __DEST)                  16 : Increment Register");
	psin2_parse("__DEC = 0x26, // DEC 26 (R:04,08 __DEST)                  16 : Decrement Register");
	psin2_parse("__ADI = 0x27, // ADI 27 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Add Immediate");
	psin2_parse("__SBI = 0x28, // SBI 28 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Subtract Immediate");
	psin2_parse("__MLI = 0x29, // MLI 29 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Multiply Immediate");
	psin2_parse("__DVI = 0x2A, // DVI 2A (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Divide Immediate");
	psin2_parse("__MDI = 0x2B, // MDI 2B (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Modulo Immediate");

	psin2_parse("// Bitwise Instruction");
	psin2_parse("__AND = 0x40, // AND 40 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise AND");
	psin2_parse("__BOR = 0x41, // BOR 41 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise OR");
	psin2_parse("__XOR = 0x42, // XOR 42 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise XOR");
	psin2_parse("__BSL = 0x43, // BSL 43 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Left");
	psin2_parse("__BSR = 0x44, // BSL 44 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Right");
	psin2_parse("__NOT = 0x45, // NOT 45 (R:04,08 __DEST)                  16 : Bitwise NOT");
	psin2_parse("__ANI = 0x46, // ANI 46 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise AND w/ Immediate");
	psin2_parse("__ORI = 0x47, // ORI 47 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise OR w/ Immediate");
	psin2_parse("__XOI = 0x48, // XOI 48 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise XOR w/ Immediate");
	psin2_parse("__BLI = 0x49, // BLI 49 (R:04,08 __DEST) (I:06,08 ___SRC) 24 : Bitshift Left w/ Immediate");
	psin2_parse("__BRI = 0x4A, // BRI 4A (R:04,08 __DEST) (I:06,08 ___SRC) 24 : Bitshift Right w/ Immediate");

	psin2_parse("// Memory Instructions");
	psin2_parse("__LDW = 0x60, // LDW 60 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Word From Memory");
	psin2_parse("__LDB = 0x61, // LDB 61 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Byte From Memory");
	psin2_parse("__STW = 0x62, // STW 62 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory");
	psin2_parse("__STB = 0x63, // STB 63 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory");
	psin2_parse("__PSH = 0x64, // PSH 64 (R:04,08 ___SRC)                  16 : Push to Stack");
	psin2_parse("__POP = 0x65, // POP 65 (R:04,08 __DEST)                  16 : Pop from Stock");
	psin2_parse("__PSR = 0x66, // PSR 66                                   08 : Push all Registers");
	psin2_parse("__POR = 0x67, // POR 67                                   08 : Pop all Registers");
	psin2_parse("__VME = 0x68, // VME 68                                   08 : Virtual Memory Enable");
	psin2_parse("__VMD = 0x69, // VMD 69                                   08 : Virtual Memory Disable ");
	psin2_parse("__VPC = 0x6A, // VPC 6A (R:04,04 __PHYS) (R:04,04 __SIZE) 16 : Virtual Page Create, Virtual Address Pushed, Permissions in r0");
	psin2_parse("__VPD = 0x6B, // VPD 6B (R:04,08 __VIRT)                  16 : Virtual Page Delete");
	psin2_parse("__VSI = 0x6C, // VSI 6C                                   08 : Virtual Security Increment");
	psin2_parse("__VSD = 0x6D, // VSD 6D                                   08 : Virtual Security Decrement");
	psin2_parse("__SPS = 0x6E, // SPS 6E (R:04,08 ___SRC)                  16 : Stack Pointer Set");
	psin2_parse("__SPG = 0x6F, // SPG 6F (R:04,08 __DEST)                  16 : Stack Pointer Get");
	psin2_parse("__VSS = 0x70, // VSS 70 (R:04,08 __LOCR)                  16 : Virtual Pagetable Start Set");
	psin2_parse("__VES = 0x71, // VES 71 (R:04,08 __ENPR)                  16 : Virtual Pagetable End Set");
	psin2_parse("__LWS = 0x72, // LWS 72 (R:04,08 __DEST) (I:64,64 __ADDR) 80 : Load Word Immediate (2nd Value Imm)");
	psin2_parse("__SWS = 0x73, // SWS 73 (R:04,08 __DEST) (I:64,64 _VALUE) 80 : Store Word Immediate (2nd Value Imm)");
	psin2_parse("__LWF = 0x74, // LWF 74 (I:64,64 __ADDR) (R:04,08 ___SRC) 80 : Load Word Immediate (1st Value Imm)");
	psin2_parse("__SWF = 0x75, // SWF 75 (I:64,64 __ADDR) (R:04,08 ___SRC) 80 : Store Word Immediate (1st Value Imm)");
	psin2_parse("__LDH = 0x74, // LDH 74 (R:04,04 __DEST) (R:04,04____PTR) 16 : Load HalfWord (32bits) From Memory");
	psin2_parse("__STH = 0x75, // STH 75 (R:04,04 __DEST) (R:04,04____PTR) 16 : Stor HalfWord (32bits) Into Memory");

	psin2_parse("// Device Instructions");
	psin2_parse("__DSQ = 0x80, // DSQ 80 (R:04,08 ___DEV)                  16 : Device Status Query");
	psin2_parse("__DSC = 0x81, // DSC 81 (R:04,04 ___DEV) (R:04,04 ___CMD) 16 : Device Send Command");
	psin2_parse("__DSD = 0x82, // DSD 82 (R:04,04 ___DEV) (R:04,04 __DATA) 16 : Device Send Data");
	psin2_parse("__DGD = 0x83, // DGD 83 (R:04,04 ___DEV) (R:04,04 __DEST) 16 : Device Get Data");
	psin2_parse("__DRS = 0x84, // DRS 84 (R:04,04 ___DEV)                  16 : Device Reset State");
	psin2_parse("__DPE = 0x85, // DPE 85 (R:04,08 ___DEV)                  16 : Device Power Enable");
	psin2_parse("__DPD = 0x86, // DPD 86 (R:04,08 ___DEV)                  16 : Device Power Disable");
	psin2_parse("__DGC = 0x87, // DGC 87 (R:04,08 __DEST)                  16 : Device Get Count");

	psin2_parse("// Interrupts");
	psin2_parse("__INT = 0x90, // INT 90 (R:04,08 __INTR)                  16 : Call Interrupt");
	psin2_parse("__HND = 0x91, // HND 91 (R:04,04 __INTR) (R:04,04 __VADR) 16 : Handle Interrupt, Security level on stack");
	psin2_parse("__IRT = 0x92, // IRT 92                                   08 : Interrupt Return");
	psin2_parse("__ENI = 0x93, // ENI 93                                   08 : Enable Interrupts");
	psin2_parse("__DSI = 0x94, // DSI 94                                   08 : Disable Interrupts");
	psin2_parse("__SMH = 0x95, // SMH 95 (R:04,08 HANDLR)                  16 : Set System Malfunction Handler");
	psin2_parse("__SIT = 0x96, // SIT 96 (R:04,08 IHNDLR)                  16 : Set Interrupt Table");

	return;
}