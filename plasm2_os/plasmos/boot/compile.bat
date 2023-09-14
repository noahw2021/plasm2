@echo off
title PLASMOS Compiler

..\..\..\x64\Debug\plasm2_asm -f=main.asm -o=bldr.bin -q


..\..\..\x64\Debug\plasm2_emu.exe --tools-bootldr --out=iimg0.bin --data=bldr.bin --loadhdr
..\..\..\x64\Debug\plasm2_emu.exe --tools-hddgen --out=iimg0.hdd --size=16384000 --mountbin --mountpath=img0.bin --mountpoint=0x00

echo Success!!