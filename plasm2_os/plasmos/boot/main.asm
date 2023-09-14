; PLASM2 OS Bootloader
; (c) Noah Wooten 2023, All Rights Reserved
; main.asm

-b 16

-r 15000

gRenderStr: ; 0x00
-z 8
gRenderChar: ;0x08
-z 8
gFont: ; 0x10
-z 8
gXYConsolePos: ;0x18
-z 8

OslMain: ; 0x20
LDI r1, gRenderStr
LDW r0, r1
CLL r0
PSI _OslHelloStr
CLR

DSI
SHF

_OslHelloStr:
-s "OSL: Hello!\n\0"

-b 16
-p 4000
-c
-e
