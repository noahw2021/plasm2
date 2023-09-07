; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; main.asm

-b 16
-r 3A0
-b 10

_BiosMain:
CLI _BiosRenderMain
CLR
JMI _BiosMainDraw

_BiosStr:
-s "Hello, world!"
-z 1 ; null term

_BiosMainDraw:
CLI _BiosRenderStr
PSH _BiosStr
CLR

DSI
SHF

-i txt/draw.asm
-i txt/render.asm
-i exit.asm