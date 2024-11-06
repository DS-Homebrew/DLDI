/*
    SuperCard DSONE
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "scds.h"

static inline void SCDS_WaitBusy(void) {
    while (cardExt_ReadData4Byte(SCDS_CMD_CARD_BUSY, SCDS_CTRL_READ_4B))
        ;
}

static inline void SCDS_FlushResponse(void) {
    cardExt_ReadData4Byte(SCDS_CMD_CARD_RESPONSE, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
}

void SCDS_SDReadSingleSector(u32 sector, void* buffer) {
    // instruct cart what to read
    cardExt_ReadData4Byte(SCDS_CMD_SDIO_READ_SINGLE_BLOCK(sector), SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();

    // instruct cart to start reading
    cardExt_ReadData4Byte(SCDS_CMD_SD_READ_REQUEST, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();

    // retrieve data
    cardExt_ReadData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDS_SDReadMultiSector(u32 sector, void* buffer, u32 num_sectors) {
    // instruct cart what to read
    cardExt_ReadData4Byte(SCDS_CMD_SDIO_READ_MULTIPLE_BLOCK(sector), SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();

    for (int i = 0; i < num_sectors; i++) {
        // instruct cart to start reading
        cardExt_ReadData4Byte(SCDS_CMD_SD_READ_REQUEST, SCDS_CTRL_READ_4B);
        SCDS_WaitBusy();
        // retrieve data
        cardExt_ReadData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
        buffer = (u8*)buffer + 0x200;
    }

    // end read
    cardExt_ReadData4Byte(SCDS_CMD_SDIO_STOP_TRANSMISSION(), SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
    SCDS_FlushResponse();
}

void SCDS_SDWriteSector(u32 sector, const u32* buffer) {
    // instruct cart where to write
    cardExt_ReadData4Byte(SCDS_CMD_SDIO_WRITE_SINGLE_BLOCK(sector), SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
    SCDS_FlushResponse();

    // write
    for (u32 i = 0; i < 128; i++)
        cardExt_ReadData4Byte(SCDS_CMD_SD_WRITE_DATA(i << 2, buffer[i]), SCDS_CTRL_READ_4B);

    // end write
    cardExt_ReadData4Byte(SCDS_CMD_SD_WRITE_END, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
}
