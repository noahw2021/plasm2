//
//  mmu.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once
#include "../../basetypes.h"

#define REASON_EXEC 0x01
#define REASON_WRTE 0x02
#define REASON_READ 0x04

#define SIZE_WATCHDOG 0xAFFFFFFFFFFFFFFF

// init
void MmuInit(void);
void MmuShutdown(void);

// i/o
BYTE MmuRead1(WORD64 Address);
WORD64  MmuRead8(WORD64 Address);
WORD64  MmuReadX(WORD64 Address, BYTE BytesToRead);
void MmuPut8(WORD64 Address, WORD64 Value);
void MmuPut4(WORD64 Address, WORD32 Value);
void MmuPut1(WORD64 Address, BYTE Value);

// virtual
WORD64 MmuTranslate(WORD64 VirtualAddress, BYTE Reason, WORD64 MaxSize);
WORD64 MmuCreatePage(WORD64 PhysicalAddress, WORD64 Size, BYTE Permissions);
void MmuDeletePage(WORD64 VirtualAddress);
void MmuSetPageTableStart(WORD64 New);
void MmuSetPageTableEnd(WORD64 New);

// stack
void MmuPush(WORD64 Value);
WORD64 MmuPop(void);

typedef struct _MMU_CTX {
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
}MMU_CTX, *PMMU_CTX;
extern PMMU_CTX MmuCtx;
