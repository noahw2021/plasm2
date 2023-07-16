; tst_textmode.asm
; plasm2
; plasm2_tests
; (c) Noah Wooten 2023, All Rights Reserved

-b 16 ; set to hex
-a 3A0 ; set to bios address

-a 10000
BiosTextModeData:
-a 3A0

; we know in a test environment, video adatper is device 1
BiosMain:

LDI r0, 1 ; device id
LDI r1, BiosTextModeData
LDI r2, 1 ; command

; send command and data
DSC r0, r2
DSD r0, r1

; write to memory
LDI r3, D757D747D74CF759
STW r1, r3

BiosLoop:
LDI r4, BiosLoop
JMP r4