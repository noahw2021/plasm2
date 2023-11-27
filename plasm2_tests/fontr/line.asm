-a 0
-b 16
-r 3A0
-b 10

XOR r4, r4
Main:
LDI r0, 1 ; device id
LDI r1, 2 ; draw line
LDI r2, 9007336694743056 ; line args (10,10), (20, 20)
LDI r3, 4294967295 ; black

PSH r3

; draw line

CMI r4, 1
NXE
JMI _BiosHalt
INC r4

DSC r0, r1
DSD r0, r2

JMI Main

_BiosHalt:
DSI
SHF

-p 4096
-c