-b 16
-a 3A0
BlProgMain:
LDI r0, 42069
LDI r1, 27351
ADD r0, r1
LDI r2, 69420
-a 400
BlProgMain_Success0:
LDI r5, 69
DSI
SHF
-a 3C0
CMP r0, r2
LDI r3, BlProgMain_Failure0
NXE
LDI r3, BlProgMain_Success0
-a 40C
BlProgMain_Failure0:
LDI r5, 420
BlProgMain_Failure1:
LDI r6, BlProgMain_Failure1
LDI r7, C
ADD r6, r7
JMP r6