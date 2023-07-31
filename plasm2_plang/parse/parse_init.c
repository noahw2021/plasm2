#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token/token.h"
/*
parse_init.c
plasm2
plasm2_plang
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6387)

parsectx_t* parsectx;

void parse_init(void) {
	parsectx = malloc(sizeof(parsectx_t));
	memset(parsectx, 0, sizeof(parsectx_t));

	parsectx->Started = 1;

	token_init();

	return;
}

void parse_shutdown(void) {
	token_shutdown();

	if (parsectx)
		free(parsectx);

	return;
}