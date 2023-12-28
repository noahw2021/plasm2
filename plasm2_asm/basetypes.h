//
//  basetypes.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

#ifndef _basetypes_h
#define _basetypes_h

typedef unsigned long long WORD64;
typedef unsigned long WORD32;
typedef unsigned short WORD16;
typedef unsigned char byte, BYTE;

#define InRange(x, min, max) (x >= min && x <= max)

#endif
