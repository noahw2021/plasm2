//
//  cpu.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../basetypes.h"
#include <time.h>

// Current clock speed
#define BASE_CLOCK 1000
#define PHYS_MEMSZ 10485760

// Critical System Malfunctions
#define CSM_IMPROPERSTACK 0x01 // Improper stack pointer with extra security (AH flag set)
#define CSM_PTFAILNOTSIZE 0x02 // Page table fail not big enough
#define CSM_PAGETOOSMALL  0x03 // Trying to read too much from a page
#define CSM_XPAGETOOSMALL 0x04 // Trying to execute too much from a page

#define SYSARG_INTTMNS 10000 // number of nanoseconds between 'clock' interrupt

// System Interrupts
#define SYSINT_CLOCK 0x00 // called every SYSARG_INTTMNS ns

// System Rings (i->security_s.SecurityLevel), 0-31
/*
0: System Ring: Kernel
1: System Drivers Ring: Kernel-mode Drivers
2: Elevated User Ring: User-mode Drivers, executive applications
3: Administrator Ring: Administrator permission applications
4: Application Ring: Regular permission application
5+: Operating system discretion
*/

enum {
	// Generic Instructions
	__MOV = 0x00, // MOV 00 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Move Registers
	__LDI = 0x01, // LDI 01 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Load Immediate
	__JMP = 0x02, // JMP 02 (R:04,08 ___ADR)                  16 : Jump
	__NXC = 0x03, // NXC 03                                   08 : Next Execute Clear
	__NXE = 0x04, // NXE 04                                   08 : Next Execute If Equal
	__NXZ = 0x05, // NXZ 05                                   08 : Next Execute If Zero
	__NXG = 0x06, // NXG 06                                   08 : Next Execute If Greater
	__NXL = 0x07, // NXL 07                                   08 : Next Execute If Less
	__CLL = 0x08, // CLL 08 (R:04,08 ___ADR)                  16 : Call
	__RET = 0x09, // RET 09                                   08 : Return
	__IMR = 0x0B, // IMR 0B (I:16,16 _VALUE)                  24 : Immediate Return
	__SHF = 0x0C, // SHF 0C                                   08 : Set Halt Flags
	__CMP = 0x0E, // CMP 0E (R:04,04 ___OP1) (R:04,04 ___OP2) 16 : Compare
	__JMI = 0x0F, // JMI 0F (I:64,64 __ADDR)                  72 : Jump Immediate
	__CLI = 0x10, // CLI 10 (I:64,64 __ADDR)                  72 : Call Immediate
	__CMI = 0x11, // CMI 11 (R:04,08 _CMPA0) (I:64,64 _CMPA1) 80 : Compare Against Immediate
    __CLR = 0x12, // CLR 12                                   08 : Call Execute
    
	// Arithmetic Instructions
	__ADD = 0x20, // ADD 20 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Add
	__SUB = 0x21, // SUB 21 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Subtract
	__MUL = 0x22, // MUL 22 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Multiply
	__DIV = 0x23, // DIV 23 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Divide
	__MOD = 0x24, // MOD 24 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Modulo
	__INC = 0x25, // INC 25 (R:04,08 __DEST)                  16 : Increment Register
	__DEC = 0x26, // DEC 26 (R:04,08 __DEST)                  16 : Decrement Register
	__ADI = 0x27, // ADI 27 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Add Immediate
	__SBI = 0x28, // SBI 28 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Subtract Immediate
	__MLI = 0x29, // MLI 29 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Multiply Immediate
	__DVI = 0x2A, // DVI 2A (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Divide Immediate
	__MDI = 0x2B, // MDI 2B (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Modulo Immediate

	// Bitwise Instruction
	__AND = 0x40, // AND 40 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise AND
	__BOR = 0x41, // BOR 41 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise OR
	__XOR = 0x42, // XOR 42 (R:04,04 __DEST) (R:04,04 ___SRC) 16 : Bitwise XOR
	__BSL = 0x43, // BSL 43 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Left
	__BSR = 0x44, // BSR 44 (R:04,04 __DEST) (R:04,04 SFTCNT) 16 : Bitshift Right
	__NOT = 0x45, // NOT 45 (R:04,08 __DEST)                  16 : Bitwise NOT
	__ANI = 0x46, // ANI 46 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise AND w/ Immediate
	__ORI = 0x47, // ORI 47 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise OR w/ Immediate
	__XOI = 0x48, // XOI 48 (R:04,08 __DEST) (I:64,64 ___SRC) 80 : Bitwise XOR w/ Immediate
	__BLI = 0x49, // BLI 49 (R:04,08 __DEST) (I:06,08 ___SRC) 24 : Bitshift Left w/ Immediate
	__BRI = 0x4A, // BRI 4A (R:04,08 __DEST) (I:06,08 ___SRC) 24 : Bitshift Right w/ Immediate
    
	// Memory Instructions
	__LDW = 0x60, // LDW 60 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Word From Memory
	__LDB = 0x61, // LDB 61 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Load Byte From Memory
	__STW = 0x62, // STW 62 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Word To Memory
	__STB = 0x63, // STB 63 (R:04,04 __DEST) (R:04,04 ___PTR) 16 : Store Byte To Memory
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
	__LWS = 0x72, // LWS 72 (R:04,08 __DEST) (I:64,64 __ADDR) 80 : Load Word Immediate (2nd Value Imm)
	__SWS = 0x73, // SWS 73 (R:04,08 __DEST) (I:64,64 _VALUE) 80 : Store Word Immediate (2nd Value Imm)
	__LDH = 0x74, // LDH 74 (R:04,04 __DEST) (R:04,04____PTR) 16 : Load HalfWord (32bits) From Memory
	__STH = 0x75, // STH 75 (R:04,04 __DEST) (R:04,04____PTR) 16 : Stor HalfWord (32bits) Into Memory
    __PSI = 0x76, // PSI 76 (I:64,64 __IMMD)                  72 : Push Immediate (64bits)

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
	u64 SystemSeconds;
	u64 SystemNanoSeconds;
	time_t SystemBoot; // silly and quick 'timer'

	byte* PhysicalMemory; // PM usage good
	u64 PhysicalMemorySize;
}cpuctx_t;
extern cpuctx_t* cpuctx;

#define Instruction(Name) void Name(void)
#define ListInstruction(Name) [__##Name] = Name
#define DeclInstruction(Name) void Name(void);
extern void(*Instructions[256])(void);

#define REGCOUNT_GPRS  16
#define REGCOUNT_SPEC  16
#define REGCOUNT_TOTAL REGCOUNT_GPRS + REGCOUNT_SPEC

typedef struct _PLASM2_CTX {
	union {
		union {
			u64 rs_64[REGCOUNT_TOTAL];
			struct {
				u64 rs_gprs[REGCOUNT_GPRS];
				u64 rs_spec[REGCOUNT_SPEC];
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
					byte TF : 1; // Interrupt table set
					byte MF : 1; // Memory Guard Flag
					byte LF : 1; // Less than flag
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
				u64 vsp; // virtual trailing arm
				u64 csm; // csm handler
				u64 dvptr; // device map pointer
				u64 spb; // stack pointer upper bound
				u64 slb; // stack lower bound
				u64 pml; // page max location
				u64 reserved[2];
			}pti;
		};
	};
}PLASM2_CTX, *PPLASM2_CTX;
extern PPLASM2_CTX i;

void cpui_csm_set(u64 Handler);
void cpui_csm_msg(byte Code, u64 AddtData);

// addresses are physical
void cpui_inst_jmp(u64 Address);
void cpui_inst_cll(u64 Address);
void cpui_inst_ret(void);
void cpui_inst_int(byte Interrupt);
void cpui_inst_clr(void);

// cpu advanced functions

/*
these functions need to be expanded on,
but for now cput_gettime will return MS,
but in the future I plan on a nanosecond
timer, but cput_gettime will always return
the time in MS.
*/

u64  cput_gettime(void); // get current ms since system start
void cput_pwr_sleep(void);
void cput_pwr_shutdown(void);
void cput_pwr_restart(void);
void cput_pwr_awake(void);
