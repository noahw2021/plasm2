//
//  basetypes.h
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#pragma once

typedef unsigned long long WORD64;
typedef unsigned long WORD32;
typedef unsigned short WORD16;
typedef unsigned char BYTE, _bool;

#define InRange(x, min, max) (x > min && x < max)

#define TRUE 1
#define FALSE 0
