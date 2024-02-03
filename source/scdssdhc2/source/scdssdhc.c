/*
	SuperCard DSONE
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "scdssdhc.h"
#include "libtwl_card.h"

static inline void SCDS_ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void SCDS_WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8 *)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

static inline void SCDS_SDSendR0Command(u8 sdio, u32 parameter)
{
	SCDS_SendCommand(SCDS_CMD_SD_HOST_PARAM(parameter, sdio, SCDS_SD_HOST_NORESPONSE));
	while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
}

static inline u32 SCDS_SDSendR1Command(u8 sdio, u32 parameter)
{
	SCDS_SendCommand(SCDS_CMD_SD_HOST_PARAM(parameter, sdio, SCDS_SD_HOST_READ_4B));
	while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
	return __builtin_bswap32(SCDS_SendCommand(SCDS_CMD_SD_HOST_RESPONSE));
}

u32 SCDS_SendCommand(const u64 command)
{
	card_romSetCmd(command);
	card_romStartXfer(SCDS_CTRL_READ_4B, false);
	card_romWaitDataReady();
	return card_romGetData();
}

void SCDS_SDReadSingleSector(u32 sector, void *buffer)
{
	// instruct cart what to read
	SCDS_SendCommand(SCDS_CMD_SD_READ_SINGLE_BLOCK(sector));

	// wait until data ready
	while(SCDS_SendCommand(SCDS_CMD_FIFO_BUSY));

	// retrieve data
	SCDS_ReadCardData(SCDS_CMD_FIFO_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDS_SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
	// instruct cart what to read
	SCDS_SendCommand(SCDS_CMD_SD_READ_MULTI_BLOCK(sector));

	while(1)
	{
        // wait until data ready
        while(SCDS_SendCommand(SCDS_CMD_FIFO_BUSY));

		// retrieve data
        SCDS_ReadCardData(SCDS_CMD_FIFO_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
		buffer = (u8 *)buffer + 0x200;
        num_sectors--;
		if(num_sectors == 0)
			break;
	    SCDS_SendCommand(SCDS_CMD_SD_HOST_PARAM(0, 0, SCDS_SD_HOST_NEXT_DATABLOCK));
	};

	// end read
	SCDS_SDSendR1Command(12, 0);
}

void SCDS_SDWriteSingleSector(u32 sector, const void *buffer)
{
	// instruct cart where to write
	SCDS_SDSendR1Command(24, sector);

    // write
    SCDS_WriteCardData(SCDS_CMD_FIFO_WRITE_DATA, SCDS_CTRL_WRITE_512B, buffer, 128);
	while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));

	// end write
	SCDS_SendCommand(SCDS_CMD_SD_WRITE_END);
	while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
}

void SCDS_SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors)
{
	// instruct cart where to write
	SCDS_SDSendR1Command(25, sector);

    do
    {
        // end write
		// well, it's supposed to be end write. But the while loop starts with it, seemingly to simplify it
		// it doesn't seem to affect anything, so let's just do it the way it wants us to
        SCDS_SendCommand(SCDS_CMD_SD_WRITE_END);
        while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
        // write
        SCDS_WriteCardData(SCDS_CMD_FIFO_WRITE_DATA, SCDS_CTRL_WRITE_512B, buffer, 128);
        while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
        buffer = (u8 *)buffer + 0x200;
		num_sectors--;
    } while (num_sectors);

	// *really* end write
	SCDS_SDSendR1Command(12, 0);
    SCDS_SendCommand(SCDS_CMD_SD_WRITE_END);
    while(SCDS_SendCommand(SCDS_CMD_SD_HOST_BUSY));
}
