#include "sdbg.h"
#include "../emu.h"
#include "../cpu/cpu.h"
#include <stdlib.h>
#include <string.h>
/*
sdbg_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

sdbgctx_t* sdbgctx;

#ifdef _WIN32
#include <Windows.h>
#endif

void sdbg_init(void) {
	sdbgctx = malloc(sizeof(sdbgctx_t));
	memset(sdbgctx, 0, sizeof(sdbgctx_t));

#ifdef _WIN32
	HANDLE OutPipe = CreateNamedPipeW(
		L"\\\\.\\pipe\\Plasm2Pipe",
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_MESSAGE,
		PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		256 * sizeof(wchar_t),
		0,
		1,
		NULL
	);

	STARTUPINFOW StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	HANDLE SecondConsole = CreateProcessW(
		L"Plasm2Helper", 
		L"plasm2_dbgclient.exe", 
		NULL, 
		NULL, 
		TRUE, 
		CREATE_NEW_CONSOLE, 
		NULL, 
		NULL, 
		&StartupInfo, 
		&ProcessInfo
	);

	BOOLEAN CnntStatus = ConnectNamedPipe(OutPipe, NULL);
	if (!CnntStatus) {
		TerminateProcess(SecondConsole, FALSE);
		CloseHandle(OutPipe);
		emu_register_fatal("Failed to open 2nd console pipe.");
		return;
	}

	LPWSTR OutgoingMessage[31] = L"PLASM2 Connection Successful.\n";
	BYTE OutgoingMsgLength = wcslen(OutgoingMessage);
	WriteFile(OutPipe, &OutgoingMsgLength, 1, NULL, NULL);
	WriteFile(OutPipe, OutgoingMessage, 31, NULL, NULL);

	sdbgctx->SecondProcess = SecondConsole;
	sdbgctx->DebugPipe = OutPipe;
#endif

	sdbgctx->CollectionBufferIn = malloc(sizeof(char) * 256);
	sdbgctx->CollectionBufferOut = malloc(sizeof(char) * 256);

	return;
}

void sdbg_shutdown(void) {
#ifdef _WIN32
	TerminateProcess(sdbgctx->SecondProcess, FALSE);
	CloseHandle(sdbgctx->DebugPipe);
#endif

	free(sdbgctx);
}

void sdbg_clock(void) {
	if (sdbgctx->LastSend > cput_gettime()) {
		if (sdbgctx->ReadyOut) {
#ifdef _WIN32
			WriteFile(sdbgctx->DebugPipe, sdbgctx->CollectionSizeOut, 1, NULL, NULL);
			WriteFile(sdbgctx->DebugPipe, sdbgctx->CollectionBufferOut, sdbgctx->CollectionSizeOut, NULL, NULL);
#else
			wprintf("%ws", sdbgctx->CollectionBufferOut);
#endif
			sdbgctx->ReadyOut = 0;
		}
	}
}

void sdbg_collect(void) {
	devicesctx->Devices[devicesctx->DeviceCount].DeviceType = DEVTYPE_FDISKC;
	devicesctx->Devices[devicesctx->DeviceCount].DeviceModel = 1;
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceName, "PLASM2EMU Serialish Debugger");
	strcpy(devicesctx->Devices[devicesctx->DeviceCount].DeviceVendor, "noahw2021");
	devicesctx->Devices[devicesctx->DeviceCount].DeviceSerial = 4279;
	devicesctx->Devices[devicesctx->DeviceCount].VendorId = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.Active = 1;
	devicesctx->Devices[devicesctx->DeviceCount].Flags.On = 1;

	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[0] = sdbg_statusquery;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[1] = sdbg_sendcommand;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[2] = sdbg_senddata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[3] = sdbg_getdata;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[4] = sdbg_reset;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[5] = sdbg_off;
	devicesctx->Devices[devicesctx->DeviceCount].Callbacks[6] = sdbg_on;

	devicesctx->DeviceCount++;

	return;
}