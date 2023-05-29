#include <stdio.h>
#include "ui/ui.h"

/*
main.c
plasm2
plasm2_tools
(c) Noah Wooten 2023, All Rights Reserved
*/

int __argc;
char** __argv;

int main(int argc, char** argv) {
	__argc = argc;
	__argv = argv;

	if (argc == 1) // launch interactive mode
		ui_interactive();
	

	return 0;
}