#include <stdio.h>
#include "basetypes.h"
#include "cpu/cpu.h"
/*
main.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

int main(int argc, char** argv) {
	cpu_init();

	while (1) {
		cpu_clock();
		if (i->flags_s.HF && !i->flags_s.IF)
			break;
	}

	return 0;
}