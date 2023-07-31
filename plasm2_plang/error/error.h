#pragma once
#include "../basetypes.h"
/*
error.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#define ERR_SPEC_BEGIN  0x0000
#define ERR_SPEC_END    0xFFFF

void error_init(void);
void error_shutdown(void);

void error_send(u16 Code, int Line);