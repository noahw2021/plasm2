; PLASM2 OS Bootloader
; (c) Noah Wooten 2023, All Rights Reserved
; main.asm

-b 16
-r 0

-a 15000
gRenderStr:
-a 15008
gRenderChar:
-a 15010
gFont:
-a 15018
gXYConsolePos:

-r 16000

OslMain:
LDW r0, gRenderStr
CLL r0
PSI _OslHelloStr
CLR

DSI
SHF

_OslHelloStr:
-s "OSL: Hello!\n\0"