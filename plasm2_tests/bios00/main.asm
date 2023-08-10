; bios00
; (c) Noah Wooten 2023, All Rights Reserved
; PLASM2 Reference BIOS Implementation

-b 16

-a 00
LDI r0, main
JMP r0

-a 20

-i dvmgr.dvmgr_init.asm ; dvmgr/dvmgr_init.c
-i hdrv.hdrv_init.asm ; hdrv/hdrv_init.c
-i hdrv.hdrv_io.asm ; hdrv/hdrv_io.c
-i txto.asm ; txto.c

-r 3A0 ; the bios is mapped to 0x3A0

main:

CLL DiInit

CLL TxtInit
PSH StrVideoDriverLoaded
CLL TxtPrint

CLL HddInit
PSH StrFDCLoaded
CLL TxtPrint

CLL HddCount
LDI r1, 30 ; '0'
MOV r3, r0
ADD r3, r1
LDI r1, StrFDCCount
LDI r2, 12
ADD r1, r2
STB r1, r3

LDI r1, 0
CMP r0, r1
NXG
JMP main_Sub0 
JMP main_Sub1

main_Sub0:
PSH StrThey
CLL TxtPrint
DSI
HLT

main_Sub1:
PSH StrNoFD
CLL TxtPrint
DSI
HLT

StrVideoDriverLoaded:
-s "Video Driver Loaded\n"
StrFDCLoaded:
-s "Fixed Disk Controller Driver Loaded\n"
StrFDCCount:
-s "Fixed Disk Count: N"
StrNoFD:
-s "No fixed disks. We are hanging here.\n"
StrThey:
-s "They were found.\n"