#pragma once
#include "../../basetypes.h"
/*
mmu.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#define REASON_EXEC 0x01
#define REASON_WRTE 0x02
#define REASON_READ 0x04

#define SIZE_WATCHDOG 0xAFFFFFFFFFFFFFFF

// init
void mmu_init(void);
void mmu_shutdown(void);

// i/o
BYTE mmu_read1(WORD64 Address);
WORD64  mmu_read8(WORD64 Address);
WORD64  mmu_readx(WORD64 Address, BYTE BytesToRead);
void mmu_put8(WORD64 Address, WORD64 Value);
void mmu_put4(WORD64 Address, WORD32 Value);
void mmu_put1(WORD64 Address, BYTE Value);

// virtual
WORD64 mmu_translate(WORD64 VirtualAddress, BYTE Reason, WORD64 MaxSize);
WORD64 mmu_createpage(WORD64 PhysicalAddress, WORD64 Size, BYTE Permissions);
void mmu_deletepage(WORD64 VirtualAddress);
void mmu_setptstart(WORD64 New);
void mmu_setptend(WORD64 New);

// stack
void mmu_push(WORD64 Value);
WORD64 mmu_pop(void);

typedef struct _mmuctx {
	WORD64 MaxPageCount;
	WORD64 PageCount;
	struct {
		WORD64 Physical;
		WORD64 Virtual;
		WORD64 Size;
		union {
			BYTE Permissions;
			struct {
				BYTE Execute : 1;
				BYTE Read : 1;
				BYTE Write : 1;
				BYTE Active : 1;
			};
		};
	}*Pages;
}mmuctx_t;
extern mmuctx_t* mmuctx;
