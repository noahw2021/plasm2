#pragma once
#include "../basetypes.h"
/*
link.h
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void link_init(void);
void link_shutdown(void);

u64  link_getsymbol(const char* Name);
void link_resolve(const char* Name, u64 Pointer);
void link_go(void);

typedef struct _linkctx {
	u64 SymbolCount;
	struct {
		u64 Resolution;
		char* SymbolName;
		byte Resolved;

		u64 LocationCount;
		struct {
			u64 Location;
		}*Locations;
	}*Symbols;
}linkctx_t;
extern linkctx_t* linkctx;