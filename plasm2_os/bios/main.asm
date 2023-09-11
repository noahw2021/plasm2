; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; main.asm

-b 16
-r 3A0
-b 10

-i hdd/hdd.asm
-i txt/draw.asm
-i txt/render.asm

-b 10

_BiosMain:
CLI _BiosRenderMain
CLR
JMI _BiosMainDraw

_BiosStr:
-s "PLASM2 BIOS: gfy and play csgo dumbass "
-z 1 ; null term
_BiosNoDrivesStr:
-s "PLASM2 BIOS: also tell alan hes an idiot"
-z 1
_BiosDrivesFoundStr:
-s "PLASM2 BIOS: Drives found, but the moth don't care"

-b 16
_BiosMainDraw:
CLI _BiosRenderStr
PSI _BiosStr
CLR

CLI _BiosGetHddCnt
CLR ; returns to r0
CMI r0, 0
NXE
JMI _BiosMainNoDrives
JMI _BiosMainOSLoader

_BiosMainNoDrives:
CLI _BiosRenderStr
PSI _BiosNoDrivesStr
CLR
XOR r0, r0
JMI _BiosClockTest0Check

_BiosClockTest0Check:
CMI r0, 10000000
NXL
JMI _BiosClockTest0Exec
JMI _BiosClockTest0After

_BiosClockTest0Exec:
INC r0
JMI _BiosClockTest0Check

_BiosClockTest0After:
DSI
SHF

_BiosMainOSLoader:
CLI _BiosRenderStr
PSI _BiosDrivesFoundStr
CLR
DSI
SHF

-i exit.asm