#pragma once
#include "../../basetypes.h"
/*
mmu.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void mmu_init(void);
void mmu_shutdown(void);

byte mmu_read1(u64 Address);
u64  mmu_read8(u64 Address);
u64  mmu_readx(u64 Address, byte BytesToRead);

void mmu_put64(u64 Address, u64 Value);
u64  mmu_get64(u64 Address);

u64 mmu_translate(u64 VirtualAddress);