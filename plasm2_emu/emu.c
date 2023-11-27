//
//  emu.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "basetypes.h"
#include "emu.h"
#include <stdlib.h>
#include <string.h>

// this will be very simple, we can combine into one file

PEMU_CTX EmuCtx;

void EmuInit(void) {
	EmuCtx = malloc(sizeof(EMU_CTX));
	memset(EmuCtx, 0, sizeof(EMU_CTX));
	return;
}

void EmuShutdown(void) {
	free(EmuCtx);
	return;
}

void EmuHalt(void) {
	strcpy(EmuCtx->LastReason, "Unspecified / Unknown Reason");
	EmuCtx->RequiresHalt = TRUE;
}

void EmuRegisterFatal(const char* Reason) {
	if (strlen(Reason) > 255)
		((char*)(Reason))[255] = 0x00;
	strcpy(EmuCtx->LastReason, Reason);
	EmuCtx->RequiresHalt = TRUE;
}

_bool EmuCheckClock(char* ThePtr) {
    
    if (EmuCtx->RequiresHalt) {
		strcpy(ThePtr, EmuCtx->LastReason);
		return TRUE;
	}
	return FALSE;
}

void* EmutexCreate(void) {
    return SDL_CreateMutex();
}

void EmutexLock(void* Mutex) {
    SDL_LockMutex(Mutex);
    return;
}

void EmutexUnlock(void* Mutex) {
    SDL_UnlockMutex(Mutex);
    return;
}

void EmutexDestory(void* Mutex) {
    SDL_DestroyMutex(Mutex);
}
