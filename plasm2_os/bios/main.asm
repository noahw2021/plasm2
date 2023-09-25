; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; main.asm

-b 16
-r 3A0
-b 10

-i hdd/hdd.asm
-i hdd/osl.asm
-i txt/draw.asm
-i txt/render.asm

-b 10

_BiosMain:
CLI _BiosRenderMain
CLR
JMI _BiosMainDraw

_BiosStr:
-s "\nPLASM2 BIOS: Loading\n"
-z 1 ; null term
_BiosNoDrivesStr:
-s "PLASM2 BIOS: No drives found!\n"
-z 1
_BiosVersionString:
-s "PLASM2 BIOS v1.00, Build 51, 25 Sept 2023 1020 EST"
-z 1

-b 16
_BiosMainDraw:
CLI _BiosRenderStr
PSI _BiosVersionString
CLR

PSI _BiosTextColor
POP r0
LDI r1, 808000FF
STH r0, r1

CLI _BiosRenderStr
PSI _BiosStr
CLR

CLI _BiosGetHddCnt
CLR ; returns to r0
DBN
CMI r0, 0
NXE
JMI _BiosMainNoDrives
JMI _BiosMainOSLoader

_BiosMainNoDrives:
CLI _BiosRenderStr
PSI _BiosNoDrivesStr
CLR
XOR r0, r0
DSI
SHF

_BiosMainOSLoader:
CLI _BiosOSLoader
CLR

DSI
SHF

-i exit.asm
-e
