//
//  basetypes.h
//  plasm2_disassembler
//
//  Created by Noah Wooten on 11/20/23.
//

#ifndef basetypes_h
#define basetypes_h

typedef unsigned long long WORD64;
typedef unsigned long WORD32;
typedef unsigned short WORD16;
typedef unsigned char byte, BYTE, _bool;

#ifndef TRUE
#define TRUE
#endif
#ifndef FALSE
#define FALSE
#endif

#define InRange(x, min, max) (x >= min && x <= max)

#endif /* basetypes_h */
