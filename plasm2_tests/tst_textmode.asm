; tst_textmode.asm
; plasm2
; plasm2_tests
; (c) Noah Wooten 2023, All Rights Reserved

-b 16 ; set to hex
-a 3A0 ; set to bios address

BiosMain:

; first we need to determine which device is the video adapter
LDI r0, 0 ; r0 = Device ID

BiosMain_Loop0:
