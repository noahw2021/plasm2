; bios00
; (c) Noah Wooten 2023, All Rights Reserved
; PLASM2 Reference BIOS Implementation

HddReadBlock:
DGC r0
LDI r1, 0
LDI r2, 0
LDI r8, 10000006

LDI r3, 0

HddReadBlock_Loop0Check:
CMP r3, r0
NXL 
JMP HddReadBlock_Loop0Exec
JMP HddReadBlock_Sub0

HddReadBlock_Loop0Exec:
MOV r5, r3
LDI r6, A0
MUL r5, r6
LDW r4, r5
AND r4, FFFFFFFF00000000
LDI r7, 20
BSR r2, r7
CMP r4, r8
NXE
JMP HddReadBlock_Sub1
INC r3
LDI r4, HddReadBlock_Loop0Check
JMP r4

HddReadBlock_Sub0:
POP r0
POP r1
POP r2
POP r3
POP r4 ; i should add a to pop and ignore x
IMR 0

HddReadBlock_Sub1:
MOV r1, r5
MOV r2, r3
LDI r3, HddReadBlock_Sub2
JMP r3

HddReadBlock_Sub2:
; r0 = Drive
; r1 = BasePtr
; r2 = Size
; r3 = Destinaton
; r4 = DeviceId
MOV r4, r2
POP r0
POP r1
POP r2
POP r3
POP r4

LDI r5, 0
DSC r4, r5
DSD r4, r0
LDI r5, F
DSC r4, r5

LDI r5, 10
DSC r4, r5
DSD r4, r1

; r5 = Size / 8
; r6 = i
MOV r5, r2 