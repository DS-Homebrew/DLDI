/*
	GMP-Z003
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "iom0.h"
#include "libtwl_card.h"

static inline void ioM0ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void ioM0WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

u32 ioM0SendCommand(const u64 command)
{
	card_romSetCmd(command);
	card_romStartXfer(IOM0_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void ioM0SDReadSector(u32 sector, void *buffer)
{
	// wait until data is ready
	// request should return 0 when ready to access
	while(ioM0SendCommand(IOM0_CMD_SD_READ_REQUEST(sector)));

	ioM0ReadCardData(IOM0_CMD_SD_READ_DATA, IOM0_CTRL_READ_512B, buffer, 128);
}

void ioM0SDWriteSector(u32 sector, const void *buffer)
{
	ioM0WriteCardData(IOM0_CMD_SD_WRITE_START(sector), IOM0_CTRL_WRITE_512B, buffer, 128);

	// Wait until write finishes
	// status should return 0 when done
	while(ioM0SendCommand(IOM0_CMD_SD_WRITE_STAT(sector)));
}
