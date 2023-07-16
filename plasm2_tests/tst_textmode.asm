; tst_textmode.asm
; plasm2
; plasm2_tests
; (c) Noah Wooten 2023, All Rights Reserved

-b 16 ; set to hex
-a 3A0 ; set to bios address

BiosMain:

; first we need to determine which device is the video adapter
LDI r0, 0 ; r0 = Device ID

; first stage of the loop
BiosMain_Loop0:
DSQ r0 ; get device status to stack
POP r1 ; pull this into r1 (r1 = device status)
LDI r2, 1100100A ; device status 'good'
CMP r1, r2
LDI r3, BiosMain_Loop1
NXE
LDI r3, BiosMain_Loop2

; device not ready
BiosMain_Loop1:

; device ready
BiosMain_Loop2:


