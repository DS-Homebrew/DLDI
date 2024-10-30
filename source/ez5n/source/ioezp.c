/*
	EZ-Flash Parallel
	Card IO routines

	Copyright (C) 2023-2024 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/card.h>
#include <nds/ndstypes.h>
#include "ioezp.h"
#include "libtwl_card.h"

static u32 ioEZP_ReadLengthCtrlValues[4] = {IOEZP_CTRL_READ_SD_512, IOEZP_CTRL_READ_SD_1024, IOEZP_CTRL_READ_SD_2048, IOEZP_CTRL_READ_SD_2048};

static inline void ioEZP_ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void ioEZP_WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuWriteUnaligned((u8 *)buffer, length);
	else
		card_romCpuWrite(buffer, length);
}

static inline u32 ioEZP_SendCommand(const u64 command, u32 latency)
{
	card_romSetCmd(command);
	card_romStartXfer((IOEZP_CTRL_SEND_CMD | MCCNT1_LATENCY1(latency)), false);
	card_romWaitDataReady();
	return card_romGetData();
}

u32 ioEZP_GetChipID(void)
{
	return ioEZP_SendCommand((((u64)CARD_CMD_DATA_CHIPID) << 56) | 1ull, 0x200);
}

void ioEZP_SDReadSectors(u32 sector, u32 num_sectors, void *buffer)
{
	u32 word_count;
	while(num_sectors > 0)
	{
		word_count = num_sectors >= 4 ? 4 : num_sectors;

		// wait until data is ready
		// request should return 0 when ready to access
		while(ioEZP_SendCommand(IOEZP_CMD_SD_READ_REQUEST(sector, word_count), 0xC8));


		ioEZP_ReadCardData(IOEZP_CMD_SD_READ_DATA, ioEZP_ReadLengthCtrlValues[word_count - 1], buffer, 128 * word_count);
		sector += word_count;
		num_sectors -= word_count;
		buffer = (u8*)buffer + (0x200 * word_count);
	}
}

void ioEZP_SDWriteSectors(u32 sector, u32 num_sectors, const void *buffer)
{
	u32 word_count;
	while(num_sectors > 0)
	{
		word_count = num_sectors >= 4 ? 4 : num_sectors;

		// Write to buffer
		for(u32 i=0; i < word_count; i++)
		{
			ioEZP_WriteCardData(IOEZP_CMD_SD_WRITE_BUFFER(i), (IOEZP_CTRL_WRITE_SD | MCCNT1_LEN_512), buffer, 128);
			buffer = (u8*)buffer + 0x200;
		}

		// Flush to disk
		// Should return 0 when done
		while(ioEZP_SendCommand(IOEZP_CMD_SD_WRITE_FLUSH(sector, word_count), 0x190));
		sector += word_count;
		num_sectors -= word_count;
	}
}
