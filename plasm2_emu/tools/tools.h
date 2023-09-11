#pragma once
/*
tools.h
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

/*
There used to be another project called
'plasm2_tools' that contained a bios generator,
but I removed it as the functionality was implemented
into plasm2_asm. I hope to avoid a future situation so for
now this will reside in the emulator.
*/

void tools_main(void);

void toolsi_hddgen(void);
void toolsi_fontgen(void);
void toolsi_fontview(void);
void toolsi_bootloader(void);