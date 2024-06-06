/*
	Stargate 3DS
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "io_sg3d.h"
#include "libtwl_card.h"

static inline void SG3D_ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void SG3D_WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

static inline u32 SG3D_SendCommand(const u64 command)
{
	card_romSetCmd(command);
	card_romStartXfer(SG3D_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void SG3D_SDReadSector(u32 sector, void *buffer)
{
	// wait until data is ready
	// request should return 0 when ready to access
	while(SG3D_SendCommand(SG3D_CMD_SD_READ_REQUEST(sector)));

	SG3D_ReadCardData(SG3D_CMD_SD_READ_DATA, SG3D_CTRL_READ_512B, buffer, 128);
}

void SG3D_SDWriteSector(u32 sector, const void *buffer)
{
	SG3D_WriteCardData(SG3D_CMD_SD_WRITE_START(sector), SG3D_CTRL_WRITE_512B, buffer, 128);

	// Wait until write finishes
	// status should return 0 when done
	while(SG3D_SendCommand(SG3D_CMD_SD_WRITE_STAT(sector)));
}
