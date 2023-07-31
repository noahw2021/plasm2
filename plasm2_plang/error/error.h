#pragma once
#include "../basetypes.h"
/*
error.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#define ERR_SPEC_BEGIN   0x0000

#define ERR_SNTX_FIRST   0x1000
#define ERR_SNTX_INVTYPE 0x1001 // Invalid type
#define ERR_SNTX_SCEXPCT 0x1002 // Semicolon expected
#define ERR_SNTX_VNEXPCT 0x1003 // Variable name expected
#define ERR_SNTX_FNEXPCT 0x1004 // Function name expected
#define ERR_SNTX_EXPBOAL 0x1005 // Expected begin of argument list
#define ERR_SNTX_EXPEOAL 0x1006 // Expected end of argument list

#define ERR_SPEC_END     0xFFFF

void error_init(void);
void error_shutdown(void);

void error_send(u16 Code, int Line);