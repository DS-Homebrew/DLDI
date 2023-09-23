/*
	M3 DS Real
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "iom3.h"
#include "libtwl_card.h"

static inline void ioM3ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void ioM3WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

u32 ioM3SendCommand(const u64 command)
{
	u32 ret;
	ioM3ReadCardData(command, IOM3_CTRL_GENERAL, &ret, 1);
	return ret;
}

void ioM3SDReadSingleSector(u32 sector, void *buffer)
{
	// wait until data is ready
	// request should return 0 when ready to access
	while(ioM3SendCommand(IOM3_CMD_SD_SINGLE_READ_REQUEST(sector)));

	ioM3ReadCardData(IOM3_CMD_SD_READ_DATA, IOM3_CTRL_SD_READ, buffer, 128);
}

void ioM3SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
	u64 command = IOM3_CMD_SD_MULTI_READ_REQUEST(sector);

	for (int i = 0; i < num_sectors; i++)
	{
		// wait until data is ready
		// request should return 0 when ready to access
		while(ioM3SendCommand(command));
		ioM3ReadCardData(IOM3_CMD_SD_READ_DATA, IOM3_CTRL_SD_READ, buffer, 128);
		buffer = (u8 *)buffer + 0x200;
		// request next sector
		command = IOM3_CMD_SD_MULTI_READ_REQUEST_NEXT(sector);
	}

	// end multi-sector read
	ioM3ReadCardData(IOM3_CMD_SD_MULTI_RW_END, IOM3_CTRL_SD_MULTI_SECTOR_END, NULL, 0);
}

void ioM3SDWriteSingleSector(u32 sector, const void *buffer)
{
	ioM3WriteCardData(IOM3_CMD_SD_SINGLE_WRITE_START(sector), IOM3_CTRL_SD_WRITE, buffer, 128);

	// Wait until write finishes
	// status should return 0 when done
	while(ioM3SendCommand(IOM3_CMD_SD_SINGLE_WRITE_STAT(sector)));
}

void ioM3SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors)
{
	u64 command = IOM3_CMD_SD_MULTI_WRITE_START(sector);
	for (int i = 0; i < num_sectors; i++)
	{
		ioM3WriteCardData(command, IOM3_CTRL_SD_WRITE, buffer, 128);
		buffer = (u8 *)buffer + 0x200;
		while(ioM3SendCommand(IOM3_CMD_SD_MULTI_WRITE_STAT(sector)));
		command = IOM3_CMD_SD_MULTI_WRITE_START_NEXT(sector);
	}

	// end multi-sector write
	ioM3ReadCardData(IOM3_CMD_SD_MULTI_RW_END, IOM3_CTRL_SD_MULTI_SECTOR_END, NULL, 0);

	while(ioM3SendCommand(IOM3_CMD_SD_MULTI_WRITE_STAT(sector)));
}
