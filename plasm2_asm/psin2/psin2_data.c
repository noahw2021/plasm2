#include "psin2.h"
//
//  psin2_data.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

void Psin2Load(void) {
	Psin2Parse("__MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers");
	Psin2Parse("__LDI = 0x01, // LDI 01 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Load Immediate");
	Psin2Parse("__JMP = 0x02, // JMP 02 (R:04,08 ___ADR)                  16 : Jump");
	Psin2Parse("__NXC = 0x03, // NXC 03                                   08 : Next Execute Clear");
	Psin2Parse("__NXE = 0x04, // NXE 04                                   08 : Next Execute If Equal");
	Psin2Parse("__NXZ = 0x05, // NXZ 05                                   08 : Next Execute If Zero");
	Psin2Parse("__NXG = 0x06, // NXG 06                                   08 : Next Execute If Greater");
	Psin2Parse("__NXL = 0x07, // NXL 07                                   08 : Next Execute If Less");
	Psin2Parse("__CLL = 0x08, // CLL 08 (R:04,08 ___ADR)                  16 : Call");
	Psin2Parse("__RET = 0x09, // RET 09                                   08 : Return");
	Psin2Parse("__IMR = 0x0B, // IMR 0B (I:16,16 _VALUE)                  24 : Immediate Return");
	Psin2Parse("__SHF = 0x0C, // SHF 0C                                   08 : Set Halt Flags");
	Psin2Parse("__CMP = 0x0E, // CMP 0E (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare");
	Psin2Parse("__JMI = 0x0F, // JMI 0F (I:64,64 __ADDR)                  72 : Jump Immediate");
	Psin2Parse("__CLI = 0x10, // CLI 10 (I:64,64 __ADDR)                  72 : Call Immediate");
	Psin2Parse("__CMI = 0x11, // CMI 11 (R:04,08 _CMPA0) (I:64,64 _CMPA1) 80 : Compare Against Immediate");
    Psin2Parse("__CLR = 0x12, // CLR 12                                   08 : Call Execute");

	Psin2Parse("// Arithmetic Instructions");
	Psin2Parse("__ADD = 0x20, // ADD 20 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Add");
	Psin2Parse("__SUB = 0x21, // SUB 21 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Subtract");
	Psin2Parse("__MUL = 0x22, // MUL 22 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Multiply");
	Psin2Parse("__DIV = 0x23, // DIV 23 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Divide");
	Psin2Parse("__MOD = 0x24, // MOD 24 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Modulo");
	Psin2Parse("__INC = 0x25, // INC 25 (R:04,08 __DEST)                  16 : Increment Register");
	Psin2Parse("__DEC = 0x26, // DEC 26 (R:04,08 __DEST)                  16 : Decrement Register");
	Psin2Parse("__ADI = 0x27, // ADI 27 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Add Immediate");
	Psin2Parse("__SBI = 0x28, // SBI 28 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Subtract Immediate");
	Psin2Parse("__MLI = 0x29, // MLI 29 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Multiply Immediate");
	Psin2Parse("__DVI = 0x2A, // DVI 2A (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Divide Immediate");
	Psin2Parse("__MDI = 0x2B, // MDI 2B (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Modulo Immediate");

	Psin2Parse("// Bitwise Instruction");
	Psin2Parse("__AND = 0x40, // AND 40 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise AND");
	Psin2Parse("__BOR = 0x41, // BOR 41 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise OR");
	Psin2Parse("__XOR = 0x42, // XOR 42 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise XOR");
	Psin2Parse("__BSL = 0x43, // BSL 43 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Left");
	Psin2Parse("__BSR = 0x44, // BSR 44 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Right");
	Psin2Parse("__NOT = 0x45, // NOT 45 (R:04,08 __DEST)                  16 : Bitwise NOT");
	Psin2Parse("__ANI = 0x46, // ANI 46 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise AND w/ Immediate");
	Psin2Parse("__ORI = 0x47, // ORI 47 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise OR w/ Immediate");
	Psin2Parse("__XOI = 0x48, // XOI 48 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise XOR w/ Immediate");
	Psin2Parse("__BLI = 0x49, // BLI 49 (R:04,08 __DEST) (I:08,08 ___SRC) 24 : Bitshift Left w/ Immediate");
	Psin2Parse("__BRI = 0x4A, // BRI 4A (R:04,08 __DEST) (I:08,08 ___SRC) 24 : Bitshift Right w/ Immediate");

	Psin2Parse("// Memory Instructions");
	Psin2Parse("__LDW = 0x60, // LDW 60 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Word From Memory");
	Psin2Parse("__LDB = 0x61, // LDB 61 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Byte From Memory");
	Psin2Parse("__STW = 0x62, // STW 62 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory");
	Psin2Parse("__STB = 0x63, // STB 63 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory");
	Psin2Parse("__PSH = 0x64, // PSH 64 (R:04,08 ___SRC)                  16 : Push to Stack");
	Psin2Parse("__POP = 0x65, // POP 65 (R:04,08 __DEST)                  16 : Pop from Stock");
	Psin2Parse("__PSR = 0x66, // PSR 66                                   08 : Push all Registers");
	Psin2Parse("__POR = 0x67, // POR 67                                   08 : Pop all Registers");
	Psin2Parse("__VME = 0x68, // VME 68                                   08 : Virtual Memory Enable");
	Psin2Parse("__VMD = 0x69, // VMD 69                                   08 : Virtual Memory Disable ");
	Psin2Parse("__VPC = 0x6A, // VPC 6A (R:04,04 __PHYS) (R:04,04 __SIZE) 16 : Virtual Page Create, Virtual Address Pushed, Permissions in r0");
	Psin2Parse("__VPD = 0x6B, // VPD 6B (R:04,08 __VIRT)                  16 : Virtual Page Delete");
	Psin2Parse("__VSI = 0x6C, // VSI 6C                                   08 : Virtual Security Increment");
	Psin2Parse("__VSD = 0x6D, // VSD 6D                                   08 : Virtual Security Decrement");
	Psin2Parse("__SPS = 0x6E, // SPS 6E (R:04,08 ___SRC)                  16 : Stack Pointer Set");
	Psin2Parse("__SPG = 0x6F, // SPG 6F (R:04,08 __DEST)                  16 : Stack Pointer Get");
	Psin2Parse("__VSS = 0x70, // VSS 70 (R:04,08 __LOCR)                  16 : Virtual Pagetable Start Set");
	Psin2Parse("__VES = 0x71, // VES 71 (R:04,08 __ENPR)                  16 : Virtual Pagetable End Set");
	Psin2Parse("__LWS = 0x72, // LWS 72 (R:04,08 __DEST) (I:64,64 __ADDR) 80 : Load Word Immediate (2nd Value Imm)");
	Psin2Parse("__SWS = 0x73, // SWS 73 (R:04,08 __DEST) (I:64,64 _VALUE) 80 : Store Word Immediate (2nd Value Imm)");
	Psin2Parse("__LDH = 0x74, // LDH 74 (R:04,04 __DEST) (R:04,04____PTR) 16 : Load HalfWord (32bits) From Memory");
	Psin2Parse("__STH = 0x75, // STH 75 (R:04,04 __DEST) (R:04,04____PTR) 16 : Stor HalfWord (32bits) Into Memory");
    Psin2Parse("__PPW = 0x76, // PPW 76                                   08 : Stack Pop and Waste (does not store value)");
    Psin2Parse("__PSI = 0x77, // PSI 77 (I:64,64 __IMMD)                  72 : Push Immediate (64bits)");
    
    Psin2Parse("// Device Instructions");
	Psin2Parse("__DSQ = 0x80, // DSQ 80 (R:04,08 ___DEV)                  16 : Device Status Query");
	Psin2Parse("__DSC = 0x81, // DSC 81 (R:04,04 ___DEV) (R:04,04 ___CMD) 16 : Device Send Command");
	Psin2Parse("__DSD = 0x82, // DSD 82 (R:04,04 ___DEV) (R:04,04 __DATA) 16 : Device Send Data");
	Psin2Parse("__DGD = 0x83, // DGD 83 (R:04,04 ___DEV) (R:04,04 __DEST) 16 : Device Get Data");
	Psin2Parse("__DRS = 0x84, // DRS 84 (R:04,04 ___DEV)                  16 : Device Reset State");
	Psin2Parse("__DPE = 0x85, // DPE 85 (R:04,08 ___DEV)                  16 : Device Power Enable");
	Psin2Parse("__DPD = 0x86, // DPD 86 (R:04,08 ___DEV)                  16 : Device Power Disable");
	Psin2Parse("__DGC = 0x87, // DGC 87 (R:04,08 __DEST)                  16 : Device Get Count");

	Psin2Parse("// Interrupts");
	Psin2Parse("__INT = 0x90, // INT 90 (R:04,08 __INTR)                  16 : Call Interrupt");
	Psin2Parse("__HND = 0x91, // HND 91 (R:04,04 __INTR) (R:04,04 __VADR) 16 : Handle Interrupt, Security level on stack");
	Psin2Parse("__IRT = 0x92, // IRT 92                                   08 : Interrupt Return");
	Psin2Parse("__ENI = 0x93, // ENI 93                                   08 : Enable Interrupts");
	Psin2Parse("__DSI = 0x94, // DSI 94                                   08 : Disable Interrupts");
	Psin2Parse("__SMH = 0x95, // SMH 95 (R:04,08 HANDLR)                  16 : Set System Malfunction Handler");
	Psin2Parse("__SIT = 0x96, // SIT 96 (R:04,08 IHNDLR)                  16 : Set Interrupt Table");

	return;
}
