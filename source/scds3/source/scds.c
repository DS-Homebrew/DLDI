/*
    SuperCard DSONE
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include <assert.h>
#include "scds.h"
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

static inline u32 SCDSSendCommand(const u64 command)
{
    u32 ret;
    SCDSReadCardData(command, SCDS_CTRL_READ_4B, &ret, 1);
    return ret;
}

static inline void SCDSFlushResponse(void)
{
    SCDSSendCommand(SCDS_CMD_CARD_RESPONSE);
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
}

void SCDSSDReadSingleSector(u32 sector, void *buffer)
{
    // instruct cart what to read
    SCDSSendCommand(SCDS_CMD_SDIO_READ_SINGLE_BLOCK(sector));
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));

    // instruct cart to start reading
    SCDSSendCommand(SCDS_CMD_SD_READ_REQUEST);
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));

    // retrieve data
    SCDSReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDSSDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
    // instruct cart what to read
    SCDSSendCommand(SCDS_CMD_SDIO_READ_MULTIPLE_BLOCK(sector));
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));

    for (int i = 0; i < num_sectors; i++)
    {
        // instruct cart to start reading
        SCDSSendCommand(SCDS_CMD_SD_READ_REQUEST);
        while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
        // retrieve data
        SCDSReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
        buffer = (u8 *)buffer + 0x200;
    }

    // end read
    SCDSSendCommand(SCDS_CMD_SDIO_STOP_TRANSMISSION());
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
    SCDSFlushResponse();
}

void SCDSSDWriteSector(u32 sector, const u32 *buffer)
{
    // instruct cart where to write
    SCDSSendCommand(SCDS_CMD_SDIO_WRITE_SINGLE_BLOCK(sector));
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
    SCDSFlushResponse();

    // write
    for (u32 i = 0; i < 128; i++)
        SCDSSendCommand(SCDS_CMD_SD_WRITE_DATA(i << 2, buffer[i]));

    // end write
    SCDSSendCommand(SCDS_CMD_SD_WRITE_END);
    while(SCDSSendCommand(SCDS_CMD_CARD_BUSY));
}
