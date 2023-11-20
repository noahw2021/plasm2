#pragma once
#include "../basetypes.h"
//
//  link.h
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//

void LinkInit(void);
void LinkShutdown(void);

WORD64 LinkGetSymbol(const char* Name, WORD32 Offset);
void LinkResolve(const char* Name, WORD64 Pointer);
void LinkGo(void);

typedef struct _LINK_CTX {
	WORD64 SymbolCount;
	struct {
		WORD64 Resolution;
		char* SymbolName;
		BYTE Resolved;

		WORD64 LocationCount;
		WORD64* Locations;
	}*Symbols;
}LINK_CTX, *PLINK_CTX;
extern PLINK_CTX LinkCtx;
