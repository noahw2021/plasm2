#pragma once
#include "../basetypes.h"
//
//  link.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

void link_init(void);
void link_shutdown(void);

u64  link_getsymbol(const char* Name, u32 Offset);
void link_resolve(const char* Name, u64 Pointer);
void link_go(void);

typedef struct _linkctx {
	u64 SymbolCount;
	struct {
		u64 Resolution;
		char* SymbolName;
		byte Resolved;

		u64 LocationCount;
		u64* Locations;
	}*Symbols;
}linkctx_t;
extern linkctx_t* linkctx;
