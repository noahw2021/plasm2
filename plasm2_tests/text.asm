; PLASM2 Video Tests
; (c) Noah Wooten 2023, All Rights Reserved
; text.asm

; Test video-mode text renderer

-b 16
-a 00
-r 3A0

; 
;
;
;
;
;
;

-b 10

_BiosMain:
; r0 = i : 0
; r1 = baseadr : 2048
; r2 = 8 : 8
XOR r0, r0
LDI r1, 2976 ; 0xBA0
LDI r5, 16384 ; 0x4000
LDI r6, 4294967295 ; 0xFFFFFFFF
LDI r7, 0 ; 0x00000000

_BiosLoop0Check:
CMI r0, 256
NXL
JMI _BiosLoop0Exec
JMI _BiosLoop0After

_BiosLoop0Exec:
XOR r3, r3
LDW r2, r1

_BiosLoop1Check:
CMI r3, 64
NXL
JMI _BiosLoop1Exec
JMI _BiosLoop1After

_BiosLoop1Exec:
MOV r4, r2
ANI r4, 1
CMI r4, 1
NXE
JMI _BiosLoop1Sub1
JMI _BiosLoop1Sub2

_BiosLoop1Sub0:
BRI r2, 1
INC r3
JMI _BiosLoop1Check

_BiosLoop1After:
ADI r1, 8
INC r0
JMI _BiosLoop0Check

_BiosLoop1Sub1:
STH r5, r6
ADI r5, 4
JMI _BiosLoop1Sub0

_BiosLoop1Sub2:
STH r5, r7
ADI r5, 4
JMI _BiosLoop1Sub0

_BiosLoop0After:
LDI r0, 65 ; 'A'
CLI _BiosRenderChar
PSH r0
CLR

DSI
SHF

_BiosTextPosX:
-z 4
_BiosTextPosY:
-z 4

; Expects an immediate char pushed to stack
_BiosRenderChar:
POP r1
LDI r2, 16384
MLI r1, 512
ADD r2, r1
LDI r3, _BiosTextPosX
LDW r4, r3
MOV r6, r4
LDI r3, _BiosTextPosY
LDW r5, r3
MOV r7, r5
BLI r4, 48
BLI r5, 32
BOR r4, r5
LDI r3, 1048584
BOR r4, r3
LDI r0, 0
LDI r1, 5
PSH r2
DSC r0, r1
DSD r0, r4
ADI r6, 8
ADI r7, 16
LDI r3, _BiosTextPosX
STW r3, r6
LDI r3, _BiosTextPosY
STW r3, r7
RET

_BiosRenderStr:


-b 16
-a 800
_BiosFon:
-f biosd7.fon
-a 1000

-c