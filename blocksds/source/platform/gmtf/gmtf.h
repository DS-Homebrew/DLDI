/*
    GMTF - Datel Games 'n' Music & Action Replay DS

    Copyright (C) 2023 lifehackerhansol
    Copyright (C) 2025 Edoardo Lolletti (edo9300)

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>
#include <libtwl/card/card.h>

#ifndef NULL
#define NULL 0
#endif

#define GMTF_SD_WRITE_OK 0x5

#define GMTF_SD_CMD_TIMEOUT_LEN 0xFFF
#define GMTF_SD_WRITE_TIMEOUT_LEN 0xFFFF

#define GMTF_SPI_READ_BYTE 0xFF
#define GMTF_SPI_START_DATA_TOKEN 0xFE
#define GMTF_SPI_MULTI_BLOCK_WRITE_TOKEN 0xFC
#define GMTF_SPI_END_MULTI_BLOCK_WRITE 0xFD

#define GMTF_CTRL_BASE (MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_LATENCY2(0x3F))

#define GMTF_CMD_F2_SPI_ENABLE 0xCC
#define GMTF_CMD_F2_SPI_DISABLE 0xC8

static inline u64 GMTF_CMD_F2(u32 param1, u8 param2) {
    return (0xF200000000000000ull | ((u64)param1 << 24) | ((u64)param2 << 16));
}

// User API

bool GMTF_SDInitialize(void);
bool GMTF_SDReadSingleSector(u32 sector, u8 * buffer);
bool GMTF_SDReadMultipleSector(u32 sector, u32 num_sectors, u8 * buffer);
bool GMTF_SDWriteSingleSector(u32 sector, const u8 * buffer);
bool GMTF_SDWriteMultipleSector(u32 sector, u32 num_sectors, const u8 * buffer);
