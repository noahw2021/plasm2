#pragma once
/*
basetypes.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#define FLAG_ASSEMBLY 0x01
#define FLAG_COMPLETE 0x02

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char byte;

#define InRange(x, min, max) (x > min && x < max)