/*
    M3 DS Real
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "m3ds.h"

void M3DS_SDReadSingleSector(u32 sector, void* buffer) {
    // wait until data is ready
    // request should return 0 when ready to access
    while (cardExt_ReadData4Byte(M3DS_CMD_SD_SINGLE_READ_REQUEST(sector), M3DS_CTRL_GENERAL))
        ;

    cardExt_ReadData(M3DS_CMD_SD_READ_DATA, M3DS_CTRL_SD_READ, buffer, 128);
}

void M3DS_SDReadMultiSector(u32 sector, void* buffer, u32 num_sectors) {
    u64 command = M3DS_CMD_SD_MULTI_READ_REQUEST(sector);

    for (int i = 0; i < num_sectors; i++) {
        // wait until data is ready
        // request should return 0 when ready to access
        while (cardExt_ReadData4Byte(command, M3DS_CTRL_GENERAL))
            ;
        cardExt_ReadData(M3DS_CMD_SD_READ_DATA, M3DS_CTRL_SD_READ, buffer, 128);
        buffer = (u8*)buffer + 0x200;
        // request next sector
        command = M3DS_CMD_SD_MULTI_READ_REQUEST_NEXT(sector);
    }

    // end multi-sector read
    cardExt_SendCommand(M3DS_CMD_SD_MULTI_RW_END, M3DS_CTRL_SD_MULTI_SECTOR_END);
}

void M3DS_SDWriteSingleSector(u32 sector, const void* buffer) {
    cardExt_WriteData(M3DS_CMD_SD_SINGLE_WRITE_START(sector), M3DS_CTRL_SD_WRITE, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    while (cardExt_ReadData4Byte(M3DS_CMD_SD_SINGLE_WRITE_STAT(sector), M3DS_CTRL_GENERAL))
        ;
}

void M3DS_SDWriteMultiSector(u32 sector, const void* buffer, u32 num_sectors) {
    u64 command = M3DS_CMD_SD_MULTI_WRITE_START(sector);
    for (int i = 0; i < num_sectors; i++) {
        cardExt_WriteData(command, M3DS_CTRL_SD_WRITE, buffer, 128);
        buffer = (u8*)buffer + 0x200;
        while (cardExt_ReadData4Byte(M3DS_CMD_SD_MULTI_WRITE_STAT(sector), M3DS_CTRL_GENERAL))
            ;
        command = M3DS_CMD_SD_MULTI_WRITE_START_NEXT(sector);
    }

    // end multi-sector write
    cardExt_SendCommand(M3DS_CMD_SD_MULTI_RW_END, M3DS_CTRL_SD_MULTI_SECTOR_END);

    while (cardExt_ReadData4Byte(M3DS_CMD_SD_MULTI_WRITE_STAT(sector), M3DS_CTRL_GENERAL))
        ;
}
