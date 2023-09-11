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

CMI r0, 504C4D4254494D47
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
PSI r0 ; gFont

ADI r0, 8
LDW r1, r0
PSI r0 ; gRenderChar

ADI r0, 8
LDW r1, r0
PSI r0 ; gRenderStr

ADI r0, 8
LDW r1, r0
PSI r0 ; gHddCount

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
; call entry point

_BiosOsLoaderFail:
_BiosOsLoaderCall: