/*
    GMP-Z003
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#ifndef NULL
#define NULL 0
#endif

// GMP-Z003 defines
// GMP-Z003 MCCNT1 flags
#define G003_CTRL_BASE                                                 \
    (MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | \
     MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24))
#define G003_CTRL_READ_4B (G003_CTRL_BASE | MCCNT1_LEN_4)
#define G003_CTRL_READ_512B (G003_CTRL_BASE | MCCNT1_LEN_512)
#define G003_CTRL_WRITE_512B (G003_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

// GMP-Z003 MCCMDs
#define G003_CMD_CARD_INFO (0xB0ull << 56)
#define G003_CMD_SD_READ_DATA (0xCAull << 56)

static inline u64 G003_CMD_SD_READ_REQUEST(u32 sector) {
    return (0xC9ull << 56) | ((u64)(sector & 0xFFFFFF) << 32) | ((u64)sector >> 24 & 0xFF);
}

static inline u64 G003_CMD_SD_WRITE_DATA(u32 sector) {
    return (0xC5ull << 56) | ((u64)(sector & 0xFFFFFF) << 32) | ((u64)sector >> 24 & 0xFF);
}

static inline u64 G003_CMD_SD_FLUSH_DATA(u32 sector) {
    u64 command = (0xC6ull << 56) | ((u64)(sector & 0xFFFFFF) << 32) | ((u64)sector >> 24 & 0xFF);
    return command;
}

// user API
void G003_SDReadSector(u32 sector, void* buffer);
void G003_SDWriteSector(u32 sector, const void* buffer);
