/*
    R4(DS) - Revolution for DS (v3)
    Card IO routines

    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>
#include "ior4.h"

#ifndef NULL
 #define NULL 0
#endif

// R4 defines
// R4 ROMCTRL flags
#define IOR4_CTRL_BASE     MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0)
#define IOR4_CTRL_GENERAL  IOR4_CTRL_BASE | MCCNT1_LEN_4
#define IOR4_CTRL_SD_READ  IOR4_CTRL_BASE | MCCNT1_LEN_512
#define IOR4_CTRL_SD_WRITE IOR4_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512

// R4 CARD_COMMANDs
#define IOR4_CMD_CARD_INFO         (0xB0ull << 56)
#define IOR4_CMD_SAVE_READ_REQUEST (0xB2ull << 56)
#define IOR4_CMD_SAVE_READ_DATA    (0xB3ull << 56)
#define IOR4_CMD_ROM_READ_REQUEST  (0xB6ull << 56)
#define IOR4_CMD_ROM_READ_DATA     (0xB7ull << 56)
#define IOR4_CMD_SD_READ_DATA      (0xBAull << 56)
#define IOR4_CMD_SAVE_WRITE_START  (0xBDull << 56)
#define IOR4_CMD_SAVE_WRITE_STAT   (0xBEull << 56)

static inline u64 IOR4_CMD_FAT_ENTRY_SEND(u32 sector)
{
    return (0xB4ull << 56) | ((u64)sector << 24);
}

static inline u64 IOR4_CMD_SD_READ_REQUEST(u32 sector)
{
    return (0xB9ull << 56) | ((u64)sector << 24);
}

static inline u64 IOR4_CMD_SD_WRITE_START(u32 sector)
{
    return (0xBBull << 56) | ((u64)sector << 24);
}

static inline u64 IOR4_CMD_SD_WRITE_STAT(u32 sector)
{
    return (0xBCull << 56) | ((u64)sector << 24);
}

// user API
void ioR4ReadCardData(u64 command, u32 flags, void* buffer, u32 length);
void ioR4SDReadSector(u32 sector, void* buffer);
void ioR4SDWriteSector(u32 sector, const void* buffer);

// non-DLDI functions follow
#ifndef DLDI
void ioR4SendFATEntry(u32 address);
#endif
