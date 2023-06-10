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

static inline void SCDS_ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuReadUnaligned((u8 *)buffer, length);
    else
        card_romCpuRead(buffer, length);
}

static inline u32 SCDS_SendCommand(const u64 command)
{
    u32 ret;
    SCDS_ReadCardData(command, SCDS_CTRL_READ_4B, &ret, 1);
    return ret;
}

static inline void SCDS_FlushResponse(void)
{
    SCDS_SendCommand(SCDS_CMD_CARD_RESPONSE);
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));
}

void SCDS_SDReadSingleSector(u32 sector, void *buffer)
{
    // instruct cart what to read
    SCDS_SendCommand(SCDS_CMD_SDIO_READ_SINGLE_BLOCK(sector));
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));

    // instruct cart to start reading
    SCDS_SendCommand(SCDS_CMD_SD_READ_REQUEST);
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));

    // retrieve data
    SCDS_ReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDS_SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
    // instruct cart what to read
    SCDS_SendCommand(SCDS_CMD_SDIO_READ_MULTIPLE_BLOCK(sector));
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));

    for (int i = 0; i < num_sectors; i++)
    {
        // instruct cart to start reading
        SCDS_SendCommand(SCDS_CMD_SD_READ_REQUEST);
        while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));
        // retrieve data
        SCDS_ReadCardData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
        buffer = (u8 *)buffer + 0x200;
    }

    // end read
    SCDS_SendCommand(SCDS_CMD_SDIO_STOP_TRANSMISSION());
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));
    SCDS_FlushResponse();
}

void SCDS_SDWriteSector(u32 sector, const u32 *buffer)
{
    // instruct cart where to write
    SCDS_SendCommand(SCDS_CMD_SDIO_WRITE_SINGLE_BLOCK(sector));
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));
    SCDS_FlushResponse();

    // write
    for (u32 i = 0; i < 128; i++)
        SCDS_SendCommand(SCDS_CMD_SD_WRITE_DATA(i << 2, buffer[i]));

    // end write
    SCDS_SendCommand(SCDS_CMD_SD_WRITE_END);
    while(SCDS_SendCommand(SCDS_CMD_CARD_BUSY));
}
