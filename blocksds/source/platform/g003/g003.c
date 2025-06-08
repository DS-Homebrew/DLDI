/*
    GMP-Z003
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "g003.h"

void G003_SDReadSector(u32 sector, void* buffer) {
    // wait until data is ready
    // request should return 0 when ready to access
    while (cardExt_ReadData4Byte(G003_CMD_SD_READ_REQUEST(sector), G003_CTRL_READ_4B));

    cardExt_ReadData(G003_CMD_SD_READ_DATA, G003_CTRL_READ_512B, buffer, 128);
}

void G003_SDWriteSector(u32 sector, const void* buffer) {
    cardExt_WriteData(G003_CMD_SD_WRITE_DATA(sector), G003_CTRL_WRITE_512B, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    while (cardExt_ReadData4Byte(G003_CMD_SD_FLUSH_DATA(sector), G003_CTRL_READ_4B));
}
