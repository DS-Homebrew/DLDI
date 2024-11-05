/*
    Stargate 3DS
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

// Stargate 3DS defines
// Stargate 3DS MCCNT1 flags
#define SG3D_CTRL_BASE                                                             \
    (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_LATENCY2(0) | \
     MCCNT1_LATENCY1(0))
#define SG3D_CTRL_READ_4B \
    (SG3D_CTRL_BASE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LEN_4)
#define SG3D_CTRL_READ_512B \
    (SG3D_CTRL_BASE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LEN_512)
#define SG3D_CTRL_WRITE_512B (SG3D_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

// Stargate 3DS MCCMDs
#define SG3D_CMD_SD_READ_DATA (0xBAull << 56)

static inline u64 SG3D_CMD_SD_READ_REQUEST(u32 sector) {
    return (0xB9ull << 56) | ((u64)sector << 24);
}

static inline u64 SG3D_CMD_SD_WRITE_START(u32 sector) {
    return (0xBBull << 56) | ((u64)sector << 24);
}

static inline u64 SG3D_CMD_SD_WRITE_STAT(u32 sector) {
    u64 command = (0xBCull << 56) | ((u64)sector << 24);
    return command;
}

// user API
void SG3D_SDReadSector(u32 sector, void* buffer);
void SG3D_SDWriteSector(u32 sector, const void* buffer);
