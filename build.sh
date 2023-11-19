cd plasm2_emu
make
cd ..
cd plasm2_asm
make
cd ..

cp build/emu/plasm2_emu64 build/plasm2_emu64
cp build/asm/plasm2_asm64 build/plasm2_asm64

echo Success! [maybe]
