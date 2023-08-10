; bios00
; (c) Noah Wooten 2023, All Rights Reserved
; PLASM2 Reference BIOS Implementation

HddInit:
RET

HddGetCount:
DGC r0
LDI r1, 0
LDI r2, 0
LDI r8, 10000006

LDI r3, 0

HddGetCount_Loop0Check:
CMP r3, r0
NXL 
JMP HddGetCount_Loop0Exec
JMP HddGetCount_Sub0

HddGetCount_Loop0Exec:
MOV r5, r3
LDI r6, A0
MUL r5, r6
LDW r4, r5
AND r4, FFFFFFFF00000000
LDI r7, 20
BSR r2, r7
CMP r4, r8
NXE
JMP HddGetCount_Sub1
INC r3
LDI r4, HddGetCount_Loop0Check
JMP r4

HddGetCount_Sub0:
IMR 0

HddGetCount_Sub1:
MOV r1, r5
MOV r2, r3
LDI r3, HddGetCount_Sub2
JMP r3

HddGetCount_Sub2:
LDI r3, 01
DSC r2, r3
DGD r2, r0
RET