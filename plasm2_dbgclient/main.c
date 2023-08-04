#include <Windows.h>
/*
main.c
plasm2
plasm2_dbgclient
(c) Noah Wooten 2023, All Rights Reserved
*/

/*
For complexity sake, this project is
currently Windows only. I need to progress
on others first, however I will come back to 
this.
*/

// Char Count 2 SiZe
#define CC2SZ(n) (sizeof(char) * (n + 1))

#pragma warning(disable: 6029 6031 28251 26451)

_bool
APIENTRY
wWinMain(
	HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPWSTR CmdLine,
	INT ShowMode
) {
	AllocConsole();
	SetConsoleTitleA("PLASM2 Debugger Console");

	HANDLE ThePipe = CreateFileA("\\\\.\\pipe\\Plasm2Pipe", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (ThePipe == INVALID_HANDLE_VALUE || ThePipe == ERROR_PIPE_BUSY) {
	GenericError:
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "Failed to open pipe.\n", 22, NULL, NULL);
		Sleep(15000);
		FreeConsole();
		return FALSE;
	}

	if (!WaitNamedPipeA("\\\\.\\pipe\\Plasm2Pipe", 15000))
		goto GenericError;

	BOOLEAN Loop = TRUE;
	LPWSTR TheBuffer = HeapAlloc(GetProcessHeap(), 0x00, CC2SZ(255)); // 255 is the max chars
	while (Loop) {
		BYTE IncomingMsgSize = 0x00; // in char cnt, not actual size
		if (!ReadFile(ThePipe, &IncomingMsgSize, 1, NULL, NULL)) {
			Loop = FALSE;
			break;
		}

		if (IncomingMsgSize != 0) {
			DWORD MakeW7Happy; // its important
			ReadFile(ThePipe, TheBuffer, CC2SZ(IncomingMsgSize), &MakeW7Happy, NULL);
			WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), TheBuffer, CC2SZ(IncomingMsgSize), NULL, NULL);
		}
		Sleep(1);
	}

	return TRUE;
}