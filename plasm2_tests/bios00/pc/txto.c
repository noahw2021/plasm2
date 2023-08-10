/*
txto.c
bios00
(c) Noah Wooten 2023, All Rights Reserved
*/

#include "txto.h"

int SerialDevice;

void TxtInit(void) {
    return;
}

void TxtPrint(const char* Text) {
    int StrLen = 0;
    while (Text[StrLen])
        StrLen++;
    
    __dsc(SerialDevice, 0x01); // SetSendLoc
    __dsd(SerialDevice, Text);
    __dsc(SerialDevice, 0x02); // SetSendSz
    __dsd(SerialDevice, StrLen);
    __dsd(SerialDevice, 0x00); // Send

    return;
}