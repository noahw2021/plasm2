#include "basetypes.h"
#include "emu.h"
#include <stdlib.h>
#include <string.h>

/*
emu.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

// this will be very simple, we can combine into one file

typedef struct _emuctx {
	BOOL RequiresHalt;
	char LastReason[256];
}emuctx_t;
emuctx_t* emuctx;

void emu_init(void) {
	emuctx = malloc(sizeof(emuctx_t));
	memset(emuctx, 0, sizeof(emuctx_t));
	return;
}

void emu_shutdown(void) {
	free(emuctx);
	return;
}

void emu_halt(void) {
	strcpy(emuctx->LastReason, "Unspecified / Unknown Reason");
	emuctx->RequiresHalt = TRUE;
}
void emu_register_fatal(const char* Reason) {
	if (strlen(Reason) > 255)
		Reason[255] = 0x00;
	strcpy(emuctx->LastReason, Reason);
	emuctx->RequiresHalt = TRUE;
}

BOOL emu_aufhoren(char* ThePtr) {
	if (emuctx->RequiresHalt) {
		strcpy(ThePtr, emuctx->LastReason);
		return TRUE;
	}
	return FALSE;
}