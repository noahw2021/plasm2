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

// init
void mmu_init(void);
void mmu_shutdown(void);

// i/o
byte mmu_read1(u64 Address);
u64  mmu_read8(u64 Address);
u64  mmu_readx(u64 Address, byte BytesToRead);
void mmu_put8(u64 Address, u64 Value);
void mmu_put1(u64 Address, byte Value);

// virtual
u64 mmu_translate(u64 VirtualAddress, byte Reason);
u32 mmu_maxaddr(u64 VirtualAddress, byte Reason);
u64 mmu_createpage(u64 PhysicalAddress, u64 Size, byte Permissions);
void mmu_deletepage(u64 VirtualAddress);
void mmu_setptstart(u64 New);
void mmu_setptend(u64 New);

// stack
void mmu_push(u64 Value);
u64 mmu_pop(void);

typedef struct _mmuctx {
	u32 MaxPageCount;
	u32 PageCount;
	struct {
		u64 Physical;
		u64 Virtual;
		u32 Size;
		union {
			byte Permissions;
			struct {
				byte Execute : 1;
				byte Read : 1;
				byte Write : 1;
				byte Active : 1;
			};
		};
	}*Pages;
}mmuctx_t;
extern mmuctx_t* mmuctx;