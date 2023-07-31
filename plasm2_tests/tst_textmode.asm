; tst_textmode.asm
; plasm2
; plasm2_tests
; (c) Noah Wooten 2023, All Rights Reserved

-b 16 ; set to hex


LDI r0, 1 ; device id
LDI r1, 10000 ; bios text mode arena
LDI r2, 1 ; command

DSC r0, r2
DSD r0, r1

LDI r8, TestString
LDW r6, r8
LDI r7, 10000
STW r7, r6

-a 400
TestString:
-s "irql"

-a 2E

LDI r5, 3A0
BiosLoop:
LDI r4, BiosLoop
ADD r4, r5
JMP r4

-p 1000
-c