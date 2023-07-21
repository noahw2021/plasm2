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

void decoder_print(const char* Format, ...) {
	va_list ArgList;
	va_start(ArgList, Format);
	
	char* Prepend = malloc(sizeof("[DBG]: ?") + strlen(Format));
	strcpy(Prepend, "[DBG]: ");
	strcat(Prepend, Format);

	vprintf(Prepend, ArgList);
	return;
}