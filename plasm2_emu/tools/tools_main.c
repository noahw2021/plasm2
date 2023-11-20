//
//  tools_main.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//

#include "tools.h"
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 6031)

void ToolsMain(void) {
	printf("PLASM2EMU Tools: \n\n");
	printf("1) HDD Generator\n");
	printf("2) Font Generator\n");
	printf("3) Font Viewer\n");
    printf("4) Boot Generator\n");
	printf("5) Quit\n");

	char Inputs[3];
	fgets(Inputs, 3, stdin);
	int Input = atoi(Inputs);
	
	switch (Input) {
	case 1:
		ToolsiHddGen();
		break;
	case 2:
		ToolsiFontGen();
		break;
	case 3:
		ToolsiFontViewer();
		break;
    case 4:
        ToolsiBootloader();
        break;
	case 5:
		break;
	default:
		break;
	}

	return;
}
