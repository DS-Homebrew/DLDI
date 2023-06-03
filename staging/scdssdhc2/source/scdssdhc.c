/*
	SuperCard DSONE
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include <assert.h>
#include "scdssdhc.h"
#include "card.h"

static inline void SCDSReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
	card_romSetCmd(command);
	card_romStartXfer(flags, false);
	if ((u32)buffer & 3)
		card_romCpuReadUnaligned((u8 *)buffer, length);
	else
		card_romCpuRead(buffer, length);
}

static inline void SCDSWriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8 *)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

static inline void SCDSFlushResponse(void)
{
	while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
	SCDSSendCommand(SCDS_CMD_CARD_RESPONSE);
}

u32 SCDSSendCommand(const u64 command)
{
	u32 ret;
	SCDSReadCardData(command, SCDS_CTRL_READ_4B, &ret, 1);
	return ret;
}

void SCDSSDReadSingleSector(u32 sector, void *buffer)
{
	// instruct cart what to read
	SCDSSendCommand(SCDS_CMD_SD_READ_SINGLE_SECTOR_SEND(sector));

	// wait until data ready
	while(SCDSSendCommand(SCDS_CMD_SD_READ_REQUEST));

	// retrieve data
	SCDSReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDSSDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
	// instruct cart what to read
	SCDSSendCommand(SCDS_CMD_SD_READ_MULTI_SECTOR_SEND(sector));

	do
	{
        // wait until data ready
        while(SCDSSendCommand(SCDS_CMD_SD_READ_REQUEST));

		// retrieve data
        SCDSReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
		buffer = (u8 *)buffer + 0x200;
        if(--num_sectors)
        {
	        SCDSSendCommand(SCDS_CMD_SD_READ_REQUEST_NEXT);
        }
	} while(num_sectors);

	// end read
	SCDSSendCommand(SCDS_CMD_SD_RW_MULTI_SECTOR_END);
	SCDSFlushResponse();
}

void SCDSSDWriteSingleSector(u32 sector, const void *buffer)
{
	// instruct cart where to write
	SCDSSendCommand(SCDS_CMD_SD_WRITE_SINGLE_SECTOR_SEND(sector));
	SCDSFlushResponse();

    // write
    SCDSWriteCardData(SCDS_CMD_SD_WRITE_DATA, SCDS_CTRL_WRITE_512B, buffer, 128);

	// end write
	while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
	SCDSSendCommand(SCDS_CMD_SD_WRITE_END);
	while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
}

void SCDSSDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors)
{
	// instruct cart where to write
	SCDSSendCommand(SCDS_CMD_SD_WRITE_MULTI_SECTOR_SEND(sector));
	SCDSFlushResponse();

    do
    {
        // write
        SCDSWriteCardData(SCDS_CMD_SD_WRITE_DATA, SCDS_CTRL_WRITE_512B, buffer, 128);
        buffer = (u8 *)buffer + 0x200;
        // if this is the last chunk, let card know
        if(!--num_sectors)
        {
	        SCDSSendCommand(SCDS_CMD_SD_RW_MULTI_SECTOR_END);
	        SCDSFlushResponse();
        }

        // end write
        while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
        SCDSSendCommand(SCDS_CMD_SD_WRITE_END);
        while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
    } while (num_sectors);
}
