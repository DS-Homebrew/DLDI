/*
    R4(DS) - Revolution for DS (v3)
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "r4tf.h"

void R4TF_SDReadSector(u32 sector, void* buffer) {
    // wait until data is ready
    // request should return 0 when ready to access
    while (cardExt_ReadData4Byte(R4TF_CMD_SD_READ_REQUEST(sector), R4TF_CTRL_READ_4B));

    // retrieve data
    cardExt_ReadData(R4TF_CMD_SD_READ_DATA, R4TF_CTRL_READ_512B, buffer, 128);
}

void R4TF_SDWriteSector(u32 sector, const void* buffer) {
    // dive straight into writing
    cardExt_WriteData(R4TF_CMD_SD_WRITE_START(sector), R4TF_CTRL_WRITE_512B, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    while (cardExt_ReadData4Byte(R4TF_CMD_SD_WRITE_STAT(sector), R4TF_CTRL_READ_4B));
}

// non-DLDI functions follow
#ifndef DLDI
void R4TF_SendFATEntry(u32 address) {
    while (cardExt_ReadData4Byte(R4TF_CMD_FAT_ENTRY_SEND(address), R4TF_CTRL_READ_4B));
}
#endif
