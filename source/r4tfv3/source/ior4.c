/*
	R4(DS) - Revolution for DS (v3)
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "ior4.h"
#include "libtwl_card.h"

static inline void ioR4ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void ioR4WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

u32 ioR4SendCommand(u64 command)
{
	card_romSetCmd(command);
	card_romStartXfer(IOR4_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void ioR4SDReadSector(u32 sector, void *buffer)
{
	// wait until data is ready
	// request should return 0 when ready to access
	while(ioR4SendCommand(IOR4_CMD_SD_READ_REQUEST(sector)));

	// retrieve data
	ioR4ReadCardData(IOR4_CMD_SD_READ_DATA, IOR4_CTRL_READ_512B, buffer, 128);
}

void ioR4SDWriteSector(u32 sector, const void *buffer)
{
	// dive straight into writing
	ioR4WriteCardData(IOR4_CMD_SD_WRITE_START(sector), IOR4_CTRL_WRITE_512B, buffer, 128);

	// Wait until write finishes
	// status should return 0 when done
	while(ioR4SendCommand(IOR4_CMD_SD_WRITE_STAT(sector)));
}

// non-DLDI functions follow
#ifndef DLDI
void ioR4SendFATEntry(u32 address)
{
	while(ioR4SendCommand(IOR4_CMD_FAT_ENTRY_SEND(address)));
}
#endif
