/*
    EZ-Flash Parallel
    Card IO routines

    Copyright (C) 2023-2024 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#include <common/libtwl_ext.h>
#include <libtwl/card/card.h>
#include <nds/card.h>
#include <nds/ndstypes.h>

#include "ez5n.h"

static u32 EZ5N_ReadLengthCtrlValues[4] = {EZ5N_CTRL_READ_SD_512, EZ5N_CTRL_READ_SD_1024,
                                           EZ5N_CTRL_READ_SD_2048, EZ5N_CTRL_READ_SD_2048};

static inline u32 EZ5N_SendCommand(const u64 command, u32 latency) {
    return cardExt_ReadData4Byte(command, (EZ5N_CTRL_SEND_CMD | MCCNT1_LATENCY1(latency)));
}

u32 EZ5N_GetChipID(void) {
    return EZ5N_SendCommand((((u64)CARD_CMD_DATA_CHIPID) << 56) | 1ull, 0x200);
}

void EZ5N_SDReadSectors(u32 sector, u32 num_sectors, void* buffer) {
    u32 sector_count;
    while (num_sectors > 0) {
        sector_count = num_sectors >= 4 ? 4 : num_sectors;

        // wait until data is ready
        // request should return 0 when ready to access
        while (EZ5N_SendCommand(EZ5N_CMD_SD_READ_REQUEST(sector, sector_count), 0xC8))
            ;

        cardExt_ReadData(EZ5N_CMD_SD_READ_DATA, EZ5N_ReadLengthCtrlValues[sector_count - 1], buffer,
                         128 * sector_count);
        sector += sector_count;
        num_sectors -= sector_count;
        buffer = (u8*)buffer + (0x200 * sector_count);
    }
}

void EZ5N_SDWriteSectors(u32 sector, u32 num_sectors, const void* buffer) {
    u32 sector_count;
    while (num_sectors > 0) {
        sector_count = num_sectors >= 4 ? 4 : num_sectors;

        // Write to buffer
        for (u32 i = 0; i < sector_count; i++) {
            cardExt_WriteData(EZ5N_CMD_SD_WRITE_BUFFER(i), (EZ5N_CTRL_WRITE_SD | MCCNT1_LEN_512),
                               buffer, 128);
            buffer = (u8*)buffer + 0x200;
        }

        // Flush to disk
        // Should return 0 when done
        while (EZ5N_SendCommand(EZ5N_CMD_SD_WRITE_FLUSH(sector, sector_count), 0x190))
            ;
        sector += sector_count;
        num_sectors -= sector_count;
    }
}
