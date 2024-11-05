/*
    Ace3DS+
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

// Ace3DS+ defines
// Ace3DS+ MCCNT1 flags
#define ACEP_CTRL_BASE                                                                \
    (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | \
     MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0))
#define ACEP_CTRL_READ_4B (ACEP_CTRL_BASE | MCCNT1_LEN_4)
#define ACEP_CTRL_READ_512B (ACEP_CTRL_BASE | MCCNT1_LEN_512)
#define ACEP_CTRL_WRITE_512B (ACEP_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

// Ace3DS+ MCCMDs
#define ACEP_CMD_SD_READ_DATA (0xBAull << 56)

static inline u64 ACEP_CMD_SD_READ_REQUEST(u32 sector) {
    return (0xB9ull << 56) | ((u64)sector << 24);
}

static inline u64 ACEP_CMD_SD_WRITE_START(u32 sector) {
    return (0xBBull << 56) | ((u64)sector << 24);
}

static inline u64 ACEP_CMD_SD_WRITE_STAT(u32 sector) {
    u64 command = (0xBCull << 56) | ((u64)sector << 24);
    command |= ((command >> 56) ^ (command >> 48) ^ (command >> 40) ^ (command >> 32) ^
                (command >> 24)) &
               0xFF;
    return command;
}

// user API
void ACEP_SDReadSector(u32 sector, void* buffer);
void ACEP_SDWriteSector(u32 sector, const void* buffer);
