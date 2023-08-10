/*
main.c
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "plasm2.h"
#include "dvmgr/dvmgr.h"
#include "hdrv/hdrv.h"
#include "txto.h"

void main(void) {
    // Load devices
    DiInit();

    TxtInit();
    TxtPrint("Video Driver Loaded\n");

    // HDD Data
    HddInit();
    TxtPrint("Fixed Disk Controller Driver Loaded\n");
    
    int TheHddCount = HddCount();
    char ToAscii = TheHddCount + '0';
    char* OutString = "Fixed Disk Count: N";
    OutString[18] = ToAscii;

    if (TheHddCount > 0) {

    } else {
        TxtPrint("No fixed disks. We are hanging here.\n");
        __dsi();
        __hlt();
    }
}