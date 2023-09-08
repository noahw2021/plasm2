; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; render.asm

-b 10

_BiosRenderMain:
; r0 = i : 0
; r1 = baseadr : 2048
; r2 = 8 : 8
XOR r0, r0
LDI r1, 2976 ; 0xA00
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
RET

-b 16
-a 800
-f txt/bios.fon

-e
