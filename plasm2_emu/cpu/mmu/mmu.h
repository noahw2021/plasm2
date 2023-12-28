//
//  mmu.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _mmu_h
#define _mmu_h

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

typedef union _MMU_PAGE_FLAGS {
    WORD64 Permissions;
    struct {
        WORD64 Secure : 1;
        WORD64 Selector : 1; // 0 = W, 1 = X
        WORD64 Read : 1;
        WORD64 Active : 1;
        
        WORD64 Reserved : 60;
    };
}MMU_PAGE_FLAGS, *PMMU_PAGE_FLAGS;

typedef struct _MMU_PAGE {
    WORD64 Physical;
    WORD64 Virtual;
    WORD64 Size;
    MMU_PAGE_FLAGS Flags;
}MMU_PAGE, *PMMU_PAGE;

typedef struct _MMU_CTX {
	WORD64 MaxPageCount;
	WORD64 PageCount;
    PMMU_PAGE Pages;
}MMU_CTX, *PMMU_CTX;
extern PMMU_CTX MmuCtx;

#endif
