; tst_videomode.asm

LDI r0, 1 ; device id
LDI r1, 2 ; draw line
LDI r2, 5629585434214410 ; line args (10,10), (20, 20)
LDI r3, 4294967295 ; black

PSH r3 ; push color to stack

; draw line
DSC r0, r1
DSD r0, r2

; infinite loop
Eh2:
LDI r4, Eh2
JMP r4

-c