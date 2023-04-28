#include "link.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
link_init.c
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

linkctx_t* linkctx;

#pragma warning(disable: 6387)

void link_init(void) {
	linkctx = malloc(sizeof(linkctx_t));
	memset(linkctx, 0, sizeof(linkctx_t));

	return;
}

void link_shutdown(void) {
	free(linkctx);
	return;

}