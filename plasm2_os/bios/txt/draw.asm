; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; draw.asm

-b 10

_BiosTextPosX:
-z 4
_BiosTextPosY:
-z 4

; Expects an immediate char pushed to stack
_BiosRenderChar:
POP r1
DEC r1
CMI r1, 9
NXE
JMI _BiosRenderCharSub0
LDI r2, 16384
MLI r1, 512
ADD r2, r1
LDI r3, _BiosTextPosX
LDH r4, r3
MOV r6, r4
LDI r3, _BiosTextPosY
LDH r5, r3
MOV r7, r5
BLI r4, 48
BLI r5, 32
BOR r4, r5
LDI r3, 524304
BOR r4, r3
LDI r0, 0
LDI r1, 5
PSH r2
DSC r0, r1
DSD r0, r4
ADI r6, 8
CMI r6, 640
NXE
JMI _BiosRenderCharSub0
JMI _BiosRenderCharSub2

_BiosRenderCharSub2:
LDI r3, _BiosTextPosX
STH r3, r6
JMI _BiosRenderCharSub1

_BiosRenderCharSub0:
;DBB
ADI r7, 16
LDI r3, _BiosTextPosY
STH r3, r7
LDI r6, 0
LDI r3, _BiosTextPosX
STH r3, r6
JMI _BiosRenderCharSub1

_BiosRenderCharSub1:
RET

; Expects a pointer string pushed to stack
_BiosRenderStr:
POP r0
JMI _BiosRenderStrLoop0Check

_BiosRenderStrLoop0Check:
LDB r1, r0
CMI r1, 0
NXE
JMI _BiosRenderStrLoop0After
JMI _BiosRenderStrLoop0Exec

_BiosRenderStrLoop0Exec:
PSH r1
PSH r0
CLI _BiosRenderChar
PSH r1
CLR
POP r0
POP r1
INC r0
JMI _BiosRenderStrLoop0Check

_BiosRenderStrLoop0After:
RET

-e
