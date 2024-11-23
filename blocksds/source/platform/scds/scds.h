/*
    SuperCard DSONE
    Card IO routines

    Copyright (C) 2006 SuperCard
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#ifndef NULL
#define NULL 0
#endif

// SCDS defines
// SCDS MCCNT1 flags
#define SCDS_CTRL_BASE (MCCNT1_RESET_OFF | MCCNT1_LATENCY2(24))
#define SCDS_CTRL_READ_4B (SCDS_CTRL_BASE | MCCNT1_LEN_4)
#define SCDS_CTRL_READ_512B (SCDS_CTRL_BASE | MCCNT1_LEN_512)

// SCDS MCCMDs

// return 0 == idle
// return non-0 == not idle
#define SCDS_CMD_CARD_BUSY (0x38ull << 56)

// Others
#define SCDS_CMD_CARD_RESPONSE (0x3040ull << 48)
#define SCDS_CMD_SD_READ_REQUEST (0x34ull << 56)
#define SCDS_CMD_SD_WRITE_END (0x35ull << 56)
#define SCDS_CMD_SD_READ_DATA (0x36ull << 56)

// SDIO
// 33 aa bb bb bb bb xx cc
// aa = SDIO CMD & 0x40
// bbbbbbbb = argument
// cc = response type
// xx = unused

#define SCDS_CMD_SDIO_BASE ((0x33ull << 56) | (0x40ull << 40))

static inline u64 SCDS_CMD_SDIO(u8 response, u8 cmd, u32 argument) {
    return SCDS_CMD_SDIO_BASE | ((u64)cmd << 40) | ((u64)argument << 8) | (u64)response;
}

static inline u64 SCDS_CMD_SDIO_STOP_TRANSMISSION() {
    return SCDS_CMD_SDIO(0, 12, 0);
}

static inline u64 SCDS_CMD_SDIO_READ_SINGLE_BLOCK(u32 sector) {
    return SCDS_CMD_SDIO(1, 17, sector);
}

static inline u64 SCDS_CMD_SDIO_READ_MULTIPLE_BLOCK(u32 sector) {
    return SCDS_CMD_SDIO(1, 18, sector);
}

// writing only works single sector
static inline u64 SCDS_CMD_SDIO_WRITE_SINGLE_BLOCK(u32 sector) {
    return SCDS_CMD_SDIO(1, 24, sector);
}

// bytes == number of bytes currently sent
// for each 4 bytes, write next word from buffer, up to 512 bytes
static inline u64 SCDS_CMD_SD_WRITE_DATA(u32 bytes, u32 data) {
    return (0x37ull << 56) | ((u64)bytes << 40) | ((u64)(__builtin_bswap32(data)) << 8);
}

// user API
void SCDS_SDReadSingleSector(u32 sector, void* buffer);
void SCDS_SDReadMultiSector(u32 sector, void* buffer, u32 num_sectors);
void SCDS_SDWriteSector(u32 sector, const u32* buffer);
