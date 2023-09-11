; PLASM2 BIOS
; (c) Noah Wooten 2023, All Rights Reserved
; hdd.asm

_BiosGetHddCnt:
LDI r0, 2 ; HDD controller
LDI r1, 1 ; get drive cnt
DSC r0, r1
DGD r0, r1
MOV r0, r1 ; returns into r0
RET

; a0 = drive id
; a1 = ptr to read
; a2 = bytes to read
; a3 = where to put

_BiosHddRead:
POP r8 
POP r2 
POP r1 
POP r0 

LDI r3, 2 ; fdisk id
LDI r4, 0 ; SetActiveDrive

; SetActiveDrive
DSC r3, r4
DSD r3, r0

MOV r5, r2
DVI r5, 8
XOR r6, r6
LDI r4, 16 ; DriveSeek
DSC r3, r4
DSD r3, r1 ; ptr
LDI r4, 6; DriveRead

_BiosHddReadLoop0Check:
CMI r6, r5
NXL 
JMI _BiosHddReadLoop0Exec
JMI _BiosHddReadLoop0After

_BiosHddReadLoop0Exec:
DSC r3, r4
DGD r3, r7
STW r8, r7
ADI r8, 8
INC r6
JMI _BiosHddReadLoop0Check

_BiosHddReadLoop0After:
RET

-e
