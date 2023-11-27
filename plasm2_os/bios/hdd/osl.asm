; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; osl.asm

-b 16
_BiosOSLoader:
CLI _BiosHddRead
PSI 0
PSI 0
PSI 80
PSI 14000
CLR

LDI r0, 14000
LDW r1, r0

CMI r1, 504C4D4254494D47
NXE
JMI _BiosOsLoaderSub0
JMI _BiosOsLoaderFail

_BiosOsLoaderSub0:
ADI r0, 8
LDW r1, r0
PSH r1 ; entry point

ADI r0, 8
LDW r3, r0 ; expected map point
MOV r11, r3 ; also expected, im lazy :D

ADI r0, 10 ; skip osid
LDW r1, r0
PSH r1 

ADI r0, 8
LDW r1, r0
PSH r1 ; gRenderChar

ADI r0, 8
LDW r1, r0
PSH r1 ; gRenderStr

ADI r0, 8
LDW r1, r0
PSH r1 ; gXYPointer

ADI r0, 8
LDW r1, r0
PSH r1 ; RawDiskPtr

ADI r0, 8
LDW r1, r0
PSH r1 ; RawDiskSize

POP r0 ; raw disk size

POP r1 ; raw disk ptr
ADI r1, 80 ; account for boot loader header

CLI _BiosHddRead
POP r0
LDI r1, 1080FFFF
STH r0, r1
PSI 0
PSH r1 ; disk ptr
PSH r0 ; disk size
PSH r3 ; map pnt
CLR

; assign variables
POP r0 ; gXYPointer
POP r1 ; gRenderStr
POP r2 ; gRenderChar
POP r3 ; gFont

PSI _BiosTextPosX
POP r4
STW r0, r4
PSI _BiosRenderChar
POP r4
STW r2, r4
PSI _BiosRenderStr
POP r4
STW r1, r4
PSI _BiosFont
POP r4
STW r3, r4


; call entry point
POP r0; entry point
ADD r0, r11 ; +=expected map point
JMP r0

_BiosOsLoaderBadBootImageStr:
-s "$PLASM2 BIOS: Invalid Boot Image\n\0"


_BiosOsLoaderFail:
PSI _BiosTextColor
POP r0
LDI r1, 1080FFFF
STH r0, r1
LDI r0, _BiosOsLoaderBadBootImageStr
INC r0
CLI _BiosRenderStr
PSH r0
CLR
DSI
SHF