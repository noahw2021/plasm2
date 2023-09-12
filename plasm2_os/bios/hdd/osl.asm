; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; osl.asm

-b 16
_BiosOSLoader:
DBF
CLI _BiosHddRead
PSI 0
PSI 0
PSI 80
PSI 14000
CLR
DBN

LDI r0, 14000
LDW r1, r0

CMI r1, 504C4D4254494D47
NXE
JMI _BiosOsLoaderSub0
JMI _BiosOsLoaderFail

_BiosOsLoaderSub0:
ADI r0, 8
LDW r1, r0
PSI r1 ; entry point

ADI r0, 8
LDW r1, r3

ADI r0, 10 ; skip osid
LDW r1, r0
PSI r0 

ADI r0, 8
LDW r1, r0
PSI r0 ; gRenderChar

ADI r0, 8
LDW r1, r0
PSI r0 ; gRenderStr

ADI r0, 8
LDW r1, r0
PSI r0 ; gXYPointer

ADI r0, 8
LDW r1, r0
PSI r0 ; RawDiskPtr

ADI r0, 8
LDW r1, r0
PSI r0 ; RawDiskSize

POP r0 ; raw disk size
POP r1 ; raw disk ptr

CLI _BiosHddRead
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

LDI r4, _BiosTextPosX
STW r0, r4
LDI r4, _BiosRenderChar
STW r2, r4
LDI r4, _BiosRenderStr
STW r1, r4
LDI r4, _BiosFont
STW r3, r4

; call entry point
POP r0; entry point
JMP r0

_BiosOsLoaderBadBootImageStr:
-s "$PLASM2 BIOS: Invalid Boot Image\n\0"

_BiosOsLoaderFail:
LDI r0, _BiosOsLoaderBadBootImageStr
INC r0
CLI _BiosRenderStr
PSH r0
CLR
DSI
SHF