#pragma once
#include "../basetypes.h"
/*
parse.h
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

void parse_init(void);
void parse_shutdown(void);

void parse_line(const char* Line);

typedef struct _parsectx {
	byte Started;

	int CurrentCtx;
	int CurrentLine;
}parsectx_t;
extern parsectx_t* parsectx;