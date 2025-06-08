/*
    Stargate 3DS
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#include "sg3d.h"

void SG3D_SDReadSector(u32 sector, void* buffer) {
    // wait until data is ready
    // request should return 0 when ready to access
    while (cardExt_ReadData4Byte(SG3D_CMD_SD_READ_REQUEST(sector), SG3D_CTRL_READ_4B));

    cardExt_ReadData(SG3D_CMD_SD_READ_DATA, SG3D_CTRL_READ_512B, buffer, 128);
}

void SG3D_SDWriteSector(u32 sector, const void* buffer) {
    cardExt_WriteData(SG3D_CMD_SD_WRITE_START(sector), SG3D_CTRL_WRITE_512B, buffer, 128);

    // Wait until write finishes
    // status should return 0 when done
    while (cardExt_ReadData4Byte(SG3D_CMD_SD_WRITE_STAT(sector), SG3D_CTRL_READ_4B));
}
