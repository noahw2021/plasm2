#pragma once
/*
basetypes.h
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char byte;

#define InRange(x, min, max) (x > min && x < max)