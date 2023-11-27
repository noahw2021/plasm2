; tst_videomode.asm
; (c) Noah Wooten 2023, All Rights Reserved

-b 16

-a 3000
BiosFontRenderTarget:

-a 3A0
BiosMain:
; Font Pre-render
; r0 = Char count
; r1 = Char[x] #0
; r2 = Char[x] #1
; r3 = memory
LDI r0, 0
LDI r3, BiosFontRenderTarget
JMI BiosMainLoop0Check

BiosMainLoop0Check:
CMI r0, 127
NXL
JMI BiosMainLoop0Exec
JMI BiosMainSub1

BiosMainLoop0Exec:


BiosMainSub1:


BiosFont:
-f bios7.fon