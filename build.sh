cd plasm2_emu
make
cd ..
cd plasm2_asm
make
cd ..
cd plasm2_disassembler
make
cd ..

cp build/emu/plasm2_emu64 build/plasm2_emu64
cp build/asm/plasm2_asm64 build/plasm2_asm64
cp build/disasm/plasm2_disasm64 build/plasm2_disasm64

echo Success! [maybe]
