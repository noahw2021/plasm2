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

void mmu_init(void);
void mmu_shutdown(void);

byte mmu_read1(u64 Address);
u64  mmu_read8(u64 Address);
u64  mmu_readx(u64 Address, byte BytesToRead);

void mmu_put8(u64 Address, u64 Value);
void mmu_put1(u64 Address, byte Value);

u64 mmu_translate(u64 VirtualAddress, byte Reason);

void mmu_push(u64 Value);
u64 mmu_pop(void);

typedef struct _mmuctx {
	u32 PageCount;
	struct {
		u64 Physical;
		u64 Virtual;
		union {
			byte Permissions;
			struct {
				byte Execute : 1;
				byte Read : 1;
				byte Write : 1;
			};
		};
	}*Pages;
}mmuctx_t;
extern mmuctx_t* mmuctx;