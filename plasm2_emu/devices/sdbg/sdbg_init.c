#include "sdbg.h"
#include <stdlib.h>
#include <string.h>
/*
sdbg_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

sdbgctx_t* sdbgctx;

void sdbg_init(void) {
	sdbgctx = malloc(sizeof(sdbgctx_t));
	memset(sdbgctx, 0, sizeof(sdbgctx_t));


}

void sdbg_shutdown(void) {

}

void sdbg_clock(void) {

}

void sdbg_collect(void) {

}