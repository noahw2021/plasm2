#include <stdio.h>
#include "ui/ui.h"

/*
main.c
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

typedef unsigned char byte;
typedef unsigned short u16;

typedef union _tmchar {
	byte Bytes[2];
	u16 UShort;

	struct {
		byte Character;
		union {
			byte Color;
			struct {
				byte r : 2;
				byte g : 2;
				byte b : 1;
				byte Blink : 1;
				byte Reserved : 1;
			};
		};
	};
}tmchar_t;

int main(int argc, char** argv) {

	//if (argc == 1) // launch interactive mode
		//ui_interactive();
	
	tmchar_t TestChar;
	TestChar.Character = 'N';
	TestChar.r = 3;
	TestChar.g = 1;
	TestChar.b = 1;
	TestChar.Blink = 1;

	printf("%04hX\n", TestChar.UShort);

	return 0;
}