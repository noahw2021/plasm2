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
#define PHYS_MEMSZ 1048576

// Critical System Malfunctions
#define CSM_IMPROPERSTACK 0x01 // Improper stack pointer with extra security (AH flag set)

enum {
	// Generic Instructions
	__MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers
	__LDI = 0x01, // LDI 01 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Load Immediate
	__JMP = 0x02, // JMP 02 (R:04,08 ___ADR)                  16 : Jump
	__NXC = 0x03, // NXC 03                                   08 : Next Execute Clear
	__NXE = 0x04, // NXE 04                                   08 : Next Execute If Equal
	__NXZ = 0x05, // NXZ 05                                   08 : Next Execute If Equal
	__NXG = 0x06, // NXG 06                                   08 : Next Execute If Greater
	__NXL = 0x07, // NXL 07                                   08 : Next Execute If Less
	__CLL = 0x08, // CLL 08 (R:04,08 ___ADR)                  16 : Call
	__RET = 0x09, // RET 09                                   08 : Return
	__IMR = 0x0A, // IMR 0A (I:16,16 _VALUE)                  24 : Immediate Return
	__SHF = 0x0B, // SHF 0B                                   08 : Set Halt Flags

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
	__PSH = 0x64, // PSH 64 (R:04,08 ___SRC)                  16 : Push to Stack
	__POP = 0x65, // POP 65 (R:04,08 __DEST)                  16 : Pop from Stock
	__PSR = 0x66, // PSR 66                                   08 : Push all Registers
	__POR = 0x67, // POR 67                                   08 : Pop all Registers
	__VME = 0x68, // VME 68                                   08 : Virtual Memory Enable
	__VMD = 0x69, // VMD 69                                   08 : Virtual Memory Disable 
	__VPC = 0x6A, // VPC 6A (R:04,04 __PHYS) (R:04,04 __SIZE) 16 : Virtual Page Create, Virtual Address Pushed, Permissions in r0
	__VPD = 0x6B, // VPD 6B (R:04,08 __VIRT)                  16 : Virtual Page Delete
	__VSI = 0x6C, // VSI 6C                                   08 : Virtual Security Increment
	__VSD = 0x6D, // VSD 6D                                   08 : Virtual Security Decrement
	__SPS = 0x6E, // SPS 6E (R:04,08 ___SRC)                  16 : Stack Pointer Set
	__SPG = 0x6F, // SPG 6F (R:04,08 __DEST)                  16 : Stack Pointer Get
	__VSS = 0x70, // VSS 70 (R:04,08 __LOCR)                  16 : Virtual Pagetable Start Set
	__VES = 0x71, // VES 71 (R:04,08 __ENPR)                  16 : Virtual Pagetable End Set

	// Device Instructions
	__DSQ = 0x80, // DSQ 80 (R:04,08 ___DEV)                  16 : Device Status Query
	__DSC = 0x81, // DSC 81 (R:04,04 ___DEV) (R:04,04 ___CMD) 16 : Device Send Command
	__DSD = 0x82, // DSD 82 (R:04,04 ___DEV) (R:04,04 __DATA) 16 : Device Send Data
	__DGD = 0x83, // DGD 83 (R:04,04 ___DEV) (R:04,04 __DEST) 16 : Device Get Data
	__DRS = 0x84, // DRS 84 (R:04,04 ___DEV)                  16 : Device Reset State
	__DPE = 0x85, // DPE 85 (R:04,08 ___DEV)                  16 : Device Power Enable
	__DPD = 0x86, // DPD 86 (R:04,08 ___DEV)                  16 : Device Power Disable
	__DGC = 0x87, // DGC 87 (R:04,08 __DEST)                  16 : Device Get Count

	// Interrupts
	__INT = 0x90, // INT 90 (R:04,08 __INTR)                  16 : Call Interrupt
	__HND = 0x91, // HND 91 (R:04,04 __INTR) (R:04,04 __VADR) 16 : Handle Interrupt, Security level on stack
	__IRT = 0x92, // IRT 92                                   08 : Interrupt Return
	__ENI = 0x93, // ENI 93                                   08 : Enable Interrupts
	__DSI = 0x94, // DSI 94                                   08 : Disable Interrupts
	__SMH = 0x95, // SMH 95 (R:04,08 HANDLR)                  16 : Set System Malfunction Handler
	__SIT = 0x96, // SIT 96 (R:04,08 IHNDLR)                  16 : Set Interrupt Table
};

void cpu_init(void);
void cpu_shutdown(void);

void cpu_clock(void);

typedef struct _cpuctx {
	u64 ClocksPerSecond;
	u64 LastClockTime;

	void* PhysicalMemory;
	u64 PhysicalMemorySize;
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
					byte VF : 1; // Virtual flag
					byte XF : 1; // Exception flag
					byte NF : 1; // Next Execute skip flag
					byte OF : 1; // Overflow flag
					byte SF : 1; // StackSkip flag
					byte CF : 1; // Call flag
					byte AF : 1; // Extra security flag
					byte IT : 1; // Interrupt table set
				}flags_s;
			};
			union {
				u64 security;
				struct {
					byte SecurityLevel : 5;
					u64 Reserved : 59;
				}security_s;
			};
			struct {
				u64 ps; // page start
				u64 pe; // page end
				u64 ral; // return address location, backup stack pointer
				u64 it; // interrupt table
			}pti;
		};
	};
}i[1];

void cpui_csm_set(u64 Handler);
void cpui_csm_msg(byte Code, u64 AddtData);