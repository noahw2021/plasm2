#pragma once
/*
plasm2.h
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char byte;

#define REASON_EXEC 0x01
#define REASON_WRTE 0x02
#define REASON_READ 0x04

// Memory Intrinsics 
void __push(u64 Value);
u64  __pop(void);

// Virtual Memory Intrinsics
void __vme(void);
void __vmd(void);
u64  __vpc(u64 Physical, u64 Size, byte Permissions);
void __vpd(u64 Virtual);
void __vsi(void);
void __vsd(void);
void __vss(u64 Ptr);
void __ves(u64 Ptr);

// Device Intrinsics
u32  __dsq(int Device);
void __dsc(int Device, u64 Command);
void __dsd(int Device, u64 Command);
u64  __dgd(int Device);
void __drs(int Device);
void __dpe(int Device);
void __dpd(int Device);
int  __dgc(void);

// Interrupt Intrinsics
void __int(byte Interrupt);
void __hnd(byte Interrupt, u64 Function, byte SecLevel);
void __irt(void);
void __eni(void);
void __dsi(void);
void __smh(u64 Handler);
void __sit(u64 Table);

// General Intrinsics
void __hlt(void);