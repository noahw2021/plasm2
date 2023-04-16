#pragma once
#include "../basetypes.h"
/*
cpu.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

// Current clock speed
#define BASE_CLOCK 1000

enum {
	// Generic Instructions
	__MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers
	__LDI = 0x01, // LDI 01 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Load Immediate
	__JMP = 0x02, // JMP : Jump
	__NXC = 0x03, // NXC : Next Execute Clear
	__NXE = 0x04, // NXE : Next Execute If Equal
	__NXZ = 0x05, // NXZ : Next Execute If Equal
	__NXG = 0x06, // NXG : Next Execute If Greater
	__NXL = 0x07, // NXL : Next Execute If Less
	__CLL = 0x08, // CLL : Call
	__RET = 0x09, // RET : Return
	__IMR = 0x0A, // IMR : Immediate Return
	__SHF = 0x0B, // SHF : Set Half Flags

	// Arithmetic Instructions
	__ADD = 0x20, // ADD 20 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Add
	__SUB = 0x21, // SUB 21 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Subtract
	__MUL = 0x22, // MUL 22 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Multiply
	__DIV = 0x23, // DIV 23 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Divide
	__MOD = 0x24, // MOD 24 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Modulo

	// Bitwise Instruction
	__AND = 0x40, // AND 40 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise AND
	__BOR = 0x41, // BOR 41 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise OR
	__XOR = 0x42, // XOR 42 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise XOR

	// Memory Instructions
	__LDW = 0x60, // LDW 60 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Word From Memory
	__LDB = 0x61, // LDB 61 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Byte From Memory
	__STW = 0x62, // STW 62 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory
	__STB = 0x63, // STB 63 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory
	__PSH = 0x64, // PSH : Push to Stack
	__POP = 0x65, // POP : Pop from Stock
	__PSR = 0x66, // PSR : Push all Registers
	__POR = 0x67, // POR : Pop all Registers
	__VME = 0x68, // VME : Virtual Memory Enable
	__VMD = 0x69, // VMD : Virtual Memory Disable
	__VPC = 0x6A, // VPC : Virtual Page Create
	__VPD = 0x6B, // VPD : Virtual Page Delete
	__VSI = 0x6C, // VSI : Virtual Security Increment
	__VSD = 0x6D, // VSD : Virtual Security Decrement
	__SPS = 0x6E, // SPS : Stack Pointer Set
	__SPG = 0x6F, // SPG : Stack Pointer Get
	__VSS = 0x70, // VSS : Virtual Pagetable Start Set
	__VES = 0x71, // VES : Virtual Pagetable End Set

	// Device Instructions
	__DSQ = 0x80, // DSQ : Device Status Query
	__DSC = 0x81, // DSC : Device Send Command
	__DSD = 0x82, // DSD : Device Send Data
	__DGD = 0x83, // DGD : Device Get Data
	__DRS = 0x84, // DRS : Device Reset State
	__DPE = 0x85, // DPE : Device Power Enable
	__DPD = 0x86, // DPD : Device Power Disable
	__DGC = 0x87, // DGC : Device Get Count

	// Interrupts
	__INT = 0x90, // INT : Call Interrupt
	__HND = 0x91, // HND : Handle Interrupt
	__IRT = 0x92, // IRT : Interrupt Return
	__ENI = 0x93, // ENI : Enable Interrupts
	__DSI = 0x94, // DSI : Disable Interrupts
};

void cpu_init(void);
void cpu_shutdown(void);

void cpu_clock(void);

typedef struct _cpuctx {
	u64 ClocksPerSecond;
	u64 LastClockTime;
}cpuctx_t;
extern cpuctx_t* cpuctx;

#define Instruction(Name) void Name(void)
#define ListInstruction(Name) [__##Name] = Name
#define DeclInstruction(Name) void Name(void);
extern void(*Instructions[256])(void);

struct {
	union {
		union {
			u64 rs_64[24];
			struct {
				u64 rs_gprs[16];
				u64 rs_spec[8];
			};
		};
		struct {
			u64 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;
			u64 ip;
			u64 sp;
			union {
				u64 flags;
				struct {
					byte GF : 1; // Greater flag
					byte EF : 1; // Equal flag
					byte ZF : 1; // Zero flag
					byte HF : 1; // Halt flag
					byte IF : 1; // Interrupt flag
				}flags_s;
			};
			union {
				u64 security;
				struct {
					byte __resevered;
				}security_s;
			};
			struct {
				u64 ps; // page start
				u64 pe; // page end
				u64 res[2]; // reserved
			}pti;
		};
	};
}i[1];