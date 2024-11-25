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

// EZParallel MCCNT1 flags

// Common flags set by all commands
#define EZ5N_CTRL_BASE \
    (MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE)

// Flags when sending a command and retrieving a 4-byte response.
#define EZ5N_CTRL_SEND_CMD (EZ5N_CTRL_BASE | MCCNT1_LATENCY2(24) | MCCNT1_LEN_4)

// Flags when reading blocks of data.
#define EZ5N_CTRL_READ_SD (EZ5N_CTRL_BASE | MCCNT1_LATENCY2(26))
#define EZ5N_CTRL_READ_SD_512 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_512)
#define EZ5N_CTRL_READ_SD_1024 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_1024)
#define EZ5N_CTRL_READ_SD_2048 (EZ5N_CTRL_READ_SD | MCCNT1_LEN_2048)

// Flags when writing blocks of data.
// The original kernel did not set any LATENCY1; however, without it, intermittent
// corruption occurs, therefore the maximum latency was added.
#define EZ5N_CTRL_WRITE_SD \
    (EZ5N_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LATENCY2(26) | MCCNT1_LATENCY1(0x1FFF))

// EZParallel MCCMDs

// Retrieves the card hardware version.
#define EZ5N_CMD_CARD_VERSION 0x3E00000000000000ull

// Sends a FAT offset table to be used with hardware-backed retail cartridge emulation.
#define EZ5N_CMD_SD_SEND_FAT_ENTRY 0xB100000100000000ull

// Requests block reads from the SD card up to 4 sectors at a time.
static inline u64 EZ5N_CMD_SD_READ_REQUEST(u32 sector, u32 num_sectors) {
    return (0xB9ull << 56) | ((u64)(num_sectors & 0xFF) << 32) | ((u64)sector);
}

// Retrieve data blocks requested by EZ5N_CMD_SD_READ_REQUEST
#define EZ5N_CMD_SD_READ_DATA 0xBA00000000000000ull

// Sends data blocks to an index in the internal write buffer.
// Up to 4 indices are supported; this means that 4 sectors can be written at a time.
static inline u64 EZ5N_CMD_SD_WRITE_BUFFER(u32 buffer_index) {
    return (0xBBull << 56) | (1ull << 32) | ((u64)(buffer_index & 0xFF));
}

// Flushes the internal buffer to a given sector on the SD card.
static inline u64 EZ5N_CMD_SD_FLUSH_BUFFER(u32 sector, u32 num_sectors) {
    return (0xBCull << 56) | ((u64)(num_sectors & 0xFF) << 32) | ((u64)sector);
}

// user API
u32 EZ5N_CardReadChipID();
void EZ5N_SDReadSectors(u32 sector, u32 num_sectors, void* buffer);
void EZ5N_SDWriteSectors(u32 sector, u32 num_sectors, const void* buffer);

#ifndef DLDI
u32 EZ5N_CardReadHWVersion(void);

// TODO: what is the structure of this table?
void EZ5N_SendFATOffsetTable(const u32* table, u32 num_words);
#endif
