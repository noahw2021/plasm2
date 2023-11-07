; PLASM2 Video Tests
; (c) Noah Wooten 2023, All Rights Reserved
; text.asm

; Test video-mode text renderer

-b 16
-a 00
-r 3A0

;for (int i = 0; i < 128; i++) {
;		u64 Segs[2];
;		for (int s = 0; s < 2; s++)
;			Segs[s] = *(byte*)(0x3000 + (s * 8));
;		for (int b = 0; b < 128; b++) {
;			if (Segs[b / 64] & (1 << b) >> b)
;				*(u32*)(0x4000 + (i * 8 * 16 * 4) + (b * 4)) = 0xFFFFFFFF;
;		}	
;	}
;}

BiosMain:
; r0 = i
; r1 = seg0
; r2 = seg1
; r3 = s
; r4 = b
; r5 = manip addr
; r6 = temp
; r7 = temp 2
XOR r0, r0
JMI BiosMainLoop0Check

BiosMainLoop0Check:
CMI r0, 80
NXL
JMI BiosMainLoop0Exec
JMI BiosMainLoop0After

BiosMainLoop0Exec:
XOR r3, r3
JMI BiosMainLoop0ExecLoop0Check
XOR r4, r4


BiosMainLoop0ExecLoop0Check:
CMI r3, 2
NXL
JMI BiosMainLoop0ExecLoop0Exec
JMI BiosMainLoop0ExecLoop0After

BiosMainLoop0ExecLoop0Exec:
CMI r3, 0
NXE
JMI BiosMainLoop0ExecLoop0ExecSub0
JMI BiosMainLoop0ExecLoop0ExecSub1

BiosMainLoop0ExecLoop0ExecSub0:
LDI r5, FontFile
MOV r6, r5
MLI r6, 8
ADD r5, r6
LDW r1, r5
JMI BiosMainLoop0ExecLoop0ExecSub2

BiosMainLoop0ExecLoop0ExecSub1:
LDI r5, FontFile
MOV r6, r5
MLI r6, 8
ADD r5, r6
LDW r2, r5
JMI BiosMainLoop0ExecLoop0ExecSub2

BiosMainLoop0ExecLoop0ExecSub2:
INC r3
JMI BiosMainLoop0ExecLoop0Check

BiosMainLoop0ExecLoop0After:
JMI BiosMainLoop0ExecLoop1Check

BiosMainLoop0ExecLoop1Check:
CMI r4, 80
NXL
JMI BiosMainLoop0ExecLoop1Exec
JMI BiosMainLoop0ExecLoop1After

BiosMainLoop0ExecLoop1Exec:
XOR r6, r6
CMI r4, 40
NXL
JMI BiosMainLoop0ExecLoop1ExecSub0
JMI BiosMainLoop0ExecLoop1ExecSub1

BiosMainLoop0ExecLoop1ExecSub0:
XOR r7, r7
INC r7
BSL r7, r4
MOV r8, r1
AND r8, r7
BSR r8, r4
CMI r8, 0
NXZ
JMI BiosMainLoop0ExecLoop1ExecSub3
JMI BiosMainLoop0ExecLoop1ExecSub4

BiosMainLoop0ExecLoop1ExecSub1:
XOR r7, r7
INC r7
BSL r7, r4
MOV r8, r2
AND r8, r7
BSR r8, r4
CMI r8, 0
NXZ
JMI BiosMainLoop0ExecLoop1ExecSub3
JMI BiosMainLoop0ExecLoop1ExecSub4

BiosMainLoop0ExecLoop1ExecSub2:
INC r4
JMI BiosMainLoop0ExecLoop1Check

BiosMainLoop0ExecLoop1ExecSub3: ; *(u32*)(0x4000 + (i * 8 * 16 * 4) + (b * 4)) = 0xFFFFFFFF;
LDI r5, 4000
MOV r6, r0
MLI r6, 200 ; (5 * 16 * 4)
ADD r5, r6
MOV r6, r4
MLI r6, 4
ADD r5, r6
LDI r6, FFFFFFFF
STH r5, r6
JMI BiosMainLoop0ExecLoop1ExecSub2

BiosMainLoop0ExecLoop1ExecSub4: ; *(u32*)(0x4000 + (i * 8 * 16 * 4) + (b * 4)) = 0x00000000;
LDI r5, 4000
MOV r6, r0
MLI r6, 200 ; (5 * 16 * 4)
ADD r5, r6
MOV r6, r4
MLI r6, 4
ADD r5, r6
LDI r6, 00000000
STH r5, r6
JMI BiosMainLoop0ExecLoop1ExecSub2

BiosMainLoop0ExecLoop1After:
INC r0
JMI BiosMainLoop0Check

BiosMainLoop0After:
; Video
LDI r0, 0 ; video = devid 0 
LDI r1, 05 ; CopyRect
LDI r2, 0800100008001000 ; glyph pos and size
LDI r3, TextRender
LDI r4, 65 ; 'A'
MLI r4, 200 ; * 'A' * 512 (512 bytes = 1 glyph size)
ADD r3, r4
DSC r0, r1
DSD r0, r3

-a 800
-f bios7.fon

-a 1000

