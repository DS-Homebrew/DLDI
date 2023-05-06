/*
    M3 DS Real
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <nds/ndstypes.h>
#include "iom3.h"
#include "card.h"

void ioM3ReadCardData(u64 command, u32 flags, void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuReadUnaligned((u8 *)buffer, length);
    else
        card_romCpuRead(buffer, length);
}

// currently static because nothing else uses it except ioM3SDWrite*Sector
static inline void ioM3WriteCardData(u64 command, u32 flags, const void *buffer, u32 length)
{
    card_romSetCmd(command);
    card_romStartXfer(flags, false);
    if ((u32)buffer & 3)
        card_romCpuWriteUnaligned((u8 *)buffer, length);
    else
        card_romCpuWrite(buffer, length);
}

void ioM3SDReadSingleSector(u32 sector, void *buffer)
{
    u32 ret;
    // wait until data is ready
    // request should return 0 when ready to access
    do
    {
        ioM3ReadCardData(IOM3_CMD_SD_SINGLE_READ_REQUEST(sector), IOM3_CTRL_GENERAL, &ret, 1);
    } while (ret);

    ioM3ReadCardData(IOM3_CMD_SD_READ_DATA, IOM3_CTRL_SD_READ, buffer, 128);
}

void ioM3SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors)
{
    u32 ret;
    u64 command = IOM3_CMD_SD_MULTI_READ_REQUEST(sector);

    for (int i = 0; i < num_sectors; i++)
    {
        // wait until data is ready
        // request should return 0 when ready to access
        do
        {
            ioM3ReadCardData(command, IOM3_CTRL_GENERAL, &ret, 1);
        } while (ret);
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
    u32 ret;

    ioM3WriteCardData(IOM3_CMD_SD_SINGLE_WRITE_START(sector), IOM3_CTRL_SD_WRITE, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    do
    {
        ioM3ReadCardData(IOM3_CMD_SD_SINGLE_WRITE_STAT(sector), IOM3_CTRL_GENERAL, &ret, 1);
    } while (ret);
}

void ioM3SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors)
{
    u32 ret;
    u64 command = IOM3_CMD_SD_MULTI_WRITE_START(sector);
    for (int i = 0; i < num_sectors; i++)
    {
        ioM3WriteCardData(command, IOM3_CTRL_SD_WRITE, buffer, 128);
        buffer = (u8 *)buffer + 0x200;
        do {
            ioM3ReadCardData(IOM3_CMD_SD_MULTI_WRITE_STAT(sector), IOM3_CTRL_GENERAL, &ret, 1);
        } while (ret);
        command = IOM3_CMD_SD_MULTI_WRITE_START_NEXT(sector);
    }

    // end multi-sector write
    ioM3ReadCardData(IOM3_CMD_SD_MULTI_RW_END, IOM3_CTRL_SD_MULTI_SECTOR_END, NULL, 0);

    do {
        ioM3ReadCardData(IOM3_CMD_SD_MULTI_WRITE_STAT(sector), IOM3_CTRL_GENERAL, &ret, 1);
    } while (ret);

}
