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

_BiosHddRead:

-e
