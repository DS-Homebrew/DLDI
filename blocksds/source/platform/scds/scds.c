/*
    SuperCard DSONE
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <common/sdio.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "scds.h"

static inline void SCDS_WaitBusy(void) {
    while (cardExt_RomReadData4Byte(SCDS_CMD_CARD_BUSY, SCDS_CTRL_READ_4B));
}

static inline void SCDS_SDSendSDIOCommand(u8 response_type, u8 cmd, u32 parameter) {
    cardExt_RomReadData4Byte(SCDS_CMD_SDIO(response_type, cmd, parameter), SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
}

static u32 SCDS_SDSendSDIOCommandR1(u8 cmd, u32 parameter) {
    u32 data = 0;
    SCDS_SDSendSDIOCommand(1, cmd, parameter);
    data = cardExt_RomReadData4Byte(SCDS_CMD_CARD_RESPONSE, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
    return data;
}

void SCDS_SDReadSingleSector(u32 sector, void* buffer) {
    // CMD17
    SCDS_SDSendSDIOCommand(1, SDIO_CMD17_READ_SINGLE_BLOCK, sector);

    // instruct cart to start reading
    cardExt_RomReadData4Byte(SCDS_CMD_SD_READ_REQUEST, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();

    // retrieve data
    cardExt_RomReadData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
}

void SCDS_SDReadMultiSector(u32 sector, void* buffer, u32 num_sectors) {
    // CMD18
    SCDS_SDSendSDIOCommand(1, SDIO_CMD18_READ_MULTIPLE_BLOCK, sector);

    for (int i = 0; i < num_sectors; i++) {
        // instruct cart to start reading
        cardExt_RomReadData4Byte(SCDS_CMD_SD_READ_REQUEST, SCDS_CTRL_READ_4B);
        SCDS_WaitBusy();
        // retrieve data
        cardExt_RomReadData(SCDS_CMD_SD_READ_DATA, SCDS_CTRL_READ_512B, buffer, 128);
        buffer = (u8*)buffer + 0x200;
    }

    // end read
    SCDS_SDSendSDIOCommandR1(SDIO_CMD12_STOP_TRANSMISSION, 0);
}

void SCDS_SDWriteSector(u32 sector, const void* buffer) {
    // CMD24
    SCDS_SDSendSDIOCommandR1(SDIO_CMD24_WRITE_SINGLE_BLOCK, sector);

    // write to buffer
    if ((u32)buffer & 3) {
        u8* u8_buffer = (u8*)buffer;
        for (u32 i = 0; i < 512; i += 4) {
            u32 data = u8_buffer[i + 0] | (u8_buffer[i + 1] << 8) | (u8_buffer[i + 2] << 16) |
                       (u8_buffer[i + 3] << 24);
            cardExt_RomReadData4Byte(SCDS_CMD_SD_WRITE_DATA(i, data), SCDS_CTRL_READ_4B);
        }
    } else {
        u32* u32_buffer = (u32*)buffer;
        for (u32 i = 0; i < 128; i++) {
            cardExt_RomReadData4Byte(SCDS_CMD_SD_WRITE_DATA(i << 2, u32_buffer[i]),
                                     SCDS_CTRL_READ_4B);
        }
    }

    // flush to disk
    cardExt_RomReadData4Byte(SCDS_CMD_SD_FLUSH_DATA, SCDS_CTRL_READ_4B);
    SCDS_WaitBusy();
}
