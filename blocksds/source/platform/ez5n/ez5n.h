/*
    EZ-Flash Parallel
    Card IO routines

    Copyright (C) 2023-2024 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#ifndef NULL
#define NULL 0
#endif

// EZParallel defines
// EZParallel MCCNT1 flags
#define EZ5N_CTRL_BASE                                                                 \
    (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | \
     MCCNT1_READ_DATA_DESCRAMBLE)
#define EZ5N_CTRL_SEND_CMD (EZ5N_CTRL_BASE | MCCNT1_LATENCY2(24) | MCCNT1_LEN_4)
#define EZ5N_CTRL_READ_SD (EZ5N_CTRL_BASE | MCCNT1_LATENCY2(26) | MCCNT1_LATENCY1(0))
#define EZ5N_CTRL_WRITE_SD \
    (EZ5N_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LATENCY2(26) | MCCNT1_LATENCY1(0))

#define EZ5N_CTRL_READ_SD_512 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_512)
#define EZ5N_CTRL_READ_SD_1024 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_1024)
#define EZ5N_CTRL_READ_SD_2048 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_2048)

// EZParallel MCCMDs
#define EZ5N_CMD_SD_READ_DATA (0xBAull << 56)

// this reads 4 sectors???
static inline u64 EZ5N_CMD_SD_READ_REQUEST(u32 sector, u32 num_sectors) {
    return (0xB9ull << 56) | ((u64)(num_sectors & 0xFF) << 32) | ((u64)sector);
}

// Writing seems to support up to 0x800 bytes
// It appears to work from an internal buffer, and flushes afterwards.
static inline u64 EZ5N_CMD_SD_WRITE_BUFFER(u32 buffer_index) {
    return (0xBBull << 56) | (1ull << 32) | ((u64)(buffer_index & 0xFF));
}

static inline u64 EZ5N_CMD_SD_WRITE_FLUSH(u32 sector, u32 num_sectors) {
    return (0xBCull << 56) | ((u64)(num_sectors & 0xFF) << 32) | ((u64)sector);
}

// user API
u32 EZ5N_GetChipID();
void EZ5N_SDReadSectors(u32 sector, u32 num_sectors, void* buffer);
void EZ5N_SDWriteSectors(u32 sector, u32 num_sectors, const void* buffer);
