; tst_videomode.asm

;-b 16
;-a 3A0
;-b 10

LDI r0, 1 ; device id
LDI r1, 2 ; draw line
LDI r2, 9007336694743056 ; line args (10,10), (20, 20)
LDI r3, 4294967295 ; black

PSH r3 ; push color to stack

; draw line
DSC r0, r1
DSD r0, r2

; infinite loop

LDI r5, 928

Eh2:
LDI r4, Eh2
ADD r4, r5
JMP r4

-p 4096
-c