//
//  basetypes.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char byte, _bool;

#define InRange(x, min, max) (x > min && x < max)

#define TRUE 1
#define FALSE 0
