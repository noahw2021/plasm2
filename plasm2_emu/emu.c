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

void EmuPrint(FILE* File, char* String) {
    EmutexLock(EmuCtx->PrintMutex);
    
    for (int i = 0; i < EmuCtx->PrintEventCount; i++) {
        PEMU_PRINTF_EVENT ThisEvent = &EmuCtx->PrintEvents[i];
        if (ThisEvent->Executed) {
            ThisEvent->Executed = FALSE;
            ThisEvent->String = String;
            ThisEvent->TargetFile = File;
            return;
        }
    }
    
    if (!EmuCtx->PrintEvents) {
        EmuCtx->PrintEvents = malloc(sizeof(EMU_PRINTF_EVENT) * 100);
        EmuCtx->PrintEventCountMax = 100;
        memset(&EmuCtx->PrintEvents[EmuCtx->PrintEventCount],
            0, sizeof(EMU_PRINTF_EVENT) * 100);
    } else {
        if (EmuCtx->PrintEventCount >= EmuCtx->PrintEventCountMax) {
            EmuCtx->PrintEventCountMax += 100;
            EmuCtx->PrintEvents = realloc(EmuCtx->PrintEvents,
                sizeof(EMU_PRINTF_EVENT) * EmuCtx->PrintEventCountMax);
            memset(&EmuCtx->PrintEvents[EmuCtx->PrintEventCount],
                0, sizeof(EMU_PRINTF_EVENT));
        }
    }
    
    PEMU_PRINTF_EVENT LatestEvent =
        &EmuCtx->PrintEvents[EmuCtx->PrintEventCount];
    
    LatestEvent->Executed = FALSE;
    LatestEvent->String = String;
    LatestEvent->TargetFile = File;
    
    EmuCtx->PrintEventCount++;
    
    EmutexUnlock(EmuCtx->PrintMutex);
    return;
}

void EmuMainThreadLoop(void) {
    //return;
    
    EmutexLock(EmuCtx->PrintMutex);
    
    for (int i = 0; i < EmuCtx->PrintEventCount; i++) {
        PEMU_PRINTF_EVENT ThisPrint = &EmuCtx->PrintEvents[i];
        if (ThisPrint->Executed)
            continue;
        
        ThisPrint->Executed = TRUE;
        puts(ThisPrint->String);
    }
    
    EmutexUnlock(EmuCtx->PrintMutex);
    return;
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
