//
//  link_init.c
//  plasm2_asm
//
//  Created by Noah Wooten on 4/21/23.
//
#include "link.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PLINK_CTX LinkCtx;

#pragma warning(disable: 6387)

void LinkInit(void) {
	LinkCtx = malloc(sizeof(LINK_CTX));
	memset(LinkCtx, 0, sizeof(LINK_CTX));

	return;
}

void LinkShutdown(void) {
	free(LinkCtx);
	return;

}
