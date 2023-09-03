#include "decoder.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
decoder_print.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

void decoder_print(const char* Format) {
	printf("[DBG]: ");
	
	int i = 0;
	while (Format[i]) {
		if (Format[i] == -18) {
			i++;
			continue;
		}

		fputc(Format[i], stdout);
		i++;
	}

	fputc('\n', stdout);

	return;
}