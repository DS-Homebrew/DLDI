/*
	Ace3DS+
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "ioa3p.h"
#include "libtwl_card.h"

static inline void ioA3PReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void ioA3PWriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

static inline u32 ioA3PSendCommand(const u64 command)
{
	card_romSetCmd(command);
	card_romStartXfer(IOA3P_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void ioA3PSDReadSector(u32 sector, void *buffer)
{
	// wait until data is ready
	// request should return 0 when ready to access
	while(ioA3PSendCommand(IOA3P_CMD_SD_READ_REQUEST(sector)));

	ioA3PReadCardData(IOA3P_CMD_SD_READ_DATA, IOA3P_CTRL_READ_512B, buffer, 128);
}

void ioA3PSDWriteSector(u32 sector, const void *buffer)
{
	ioA3PWriteCardData(IOA3P_CMD_SD_WRITE_START(sector), IOA3P_CTRL_WRITE_512B, buffer, 128);

	// Wait until write finishes
	// status should return 0 when done
	while(ioA3PSendCommand(IOA3P_CMD_SD_WRITE_STAT(sector)));
}
