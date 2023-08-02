#include "fdisk.h"
#include <stdlib.h>
#include <string.h>
/*
fdisk_init.c
plasm2
plasm2_emu
(c) Noah Wooten 2023, All Rights Reserved
*/

#pragma warning(disable: 6011 6308 6387 26451)

fdiskctx_t* fdiskctx;

void fdisk_init(void) {
	fdiskctx = malloc(sizeof(fdiskctx_t));
	memset(fdiskctx, 0, sizeof(fdiskctx_t));
}

void fdisk_shutdown(void) {
	for (int i = 0; i < fdiskctx->DriveCount; i++) {
		fclose(fdiskctx->Drives[i].DrivePhysicalPointer);
		for (int m = 0; m < 4; m++) {
			if (fdiskctx->Drives[i].CurrentLoadedChunks[m])
				free(fdiskctx->Drives[i].CurrentLoadedChunks[m]);
		}
	}
	free(fdiskctx->Drives);
	free(fdiskctx);
}

BOOL fdisk_register(const char* DiskFile) {
	FILE* Tfp = fopen(DiskFile, "wb+");
	if (!Tfp)
		return 0;

	fdiskhdr_t* PotHeader = malloc(sizeof(fdiskhdr_t));
	fread(PotHeader, sizeof(fdiskhdr_t), 1, Tfp);
	
	if (PotHeader->Magic != FDISKHDR_MAGIC) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	u64 RunningTotal = 0x00;
	for (int i = 0; i < sizeof(PotHeader->DeviceVendor); i++)
		RunningTotal += PotHeader->DeviceVendor[i];
	RunningTotal += PotHeader->DeviceVendorId;
	RunningTotal += PotHeader->DeviceSerial;
	RunningTotal += PotHeader->DeviceModelNum;

	if (RunningTotal != PotHeader->PartsSum) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	fseek(Tfp, 0, SEEK_END);
	u64 FileSize;
#ifdef _WIN32
	FileSize = _ftelli64(Tfp);
#elif __linux__
#error Please test this
	FileSize = ftello(Tfp);
#else
#error Please test this
	FileSize = ftell(Tfp);
#endif

	if (FileSize != PotHeader->ExpectedPhysicalSize) {
		free(PotHeader);
		fclose(Tfp);
		return 0;
	}

	if (!fdiskctx->Drives)
		fdiskctx->Drives = malloc(sizeof(*fdiskctx->Drives));
	else
		fdiskctx->Drives = realloc(fdiskctx->Drives, sizeof(*fdiskctx->Drives) * (fdiskctx->DriveCount + 1));
	memset(&fdiskctx->Drives[fdiskctx->DriveCount], 0, sizeof(*fdiskctx->Drives));

	fdiskctx->Drives[fdiskctx->DriveCount].CurrentFilePointer = 0x00;
	fdiskctx->Drives[fdiskctx->DriveCount].DeviceSerial = PotHeader->DeviceSerial;
	memcpy(fdiskctx->Drives[fdiskctx->DriveCount].DeviceVendor, PotHeader->DeviceVendor, 16);
	fdiskctx->Drives[fdiskctx->DriveCount].DeviceVendorId = PotHeader->DeviceVendorId;
	fdiskctx->Drives[fdiskctx->DriveCount].DrivePhysicalPointer = Tfp;
	fdiskctx->Drives[fdiskctx->DriveCount].DrivePhysicalSize = FileSize;
	fdiskctx->Drives[fdiskctx->DriveCount].IsDriveActive = 1;
	fdiskctx->Drives[fdiskctx->DriveCount].IsDriveAwake = 1;

	fdiskctx->DriveCount++;

	return;
}