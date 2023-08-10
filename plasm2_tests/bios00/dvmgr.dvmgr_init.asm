; bios00
; (c) Noah Wooten 2023, All Rights Reserved
; PLASM2 Reference BIOS Implementation

-a 800
-z 4 ; reserve 4 bytes at 0x800
-a 20

DiInit:

DGC r0
LDI r1, 0
LDI r3, 10000004

DiInit_Loop0Check:
CMP r1, r0
NXL 
JMP DiInit_Loop0Exec
JMP DiInit_Sub0

DiInit_Loop0Exec:
MOV r5, r1
LDI r6, A0
MUL r5, r6
LDW r2, r5
AND r2, FFFFFFFF00000000
LDI r4, 20
BSR r2, r4
CMP r2, r3
NXE
JMP DiInit_Loop0Exec_Sub0
INC r1
LDI r2, DiInit_Loop0Check
JMP r2

DiInit_Loop0Exec_Sub0:
LDI r0, 800
LDW r2, r0
LDI r3, 00000000FFFFFFFF
AND r2, r3
LDI r4, 32
BSL r1, r4
OR r2, r1
STW r2, r0
JMP DiInit_Sub0

DiInit_Sub0:
RET