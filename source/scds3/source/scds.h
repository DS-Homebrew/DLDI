/*
	SuperCard DSONE
	Card IO routines

	Copyright (C) 2006 SuperCard
	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>

#ifndef NULL
 #define NULL 0
#endif

// SCDS defines
// SCDS ROMCTRL flags
#define SCDS_CTRL_BASE      MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0)
#define SCDS_CTRL_READ_4B   SCDS_CTRL_BASE | MCCNT1_LEN_4
#define SCDS_CTRL_READ_512B SCDS_CTRL_BASE | MCCNT1_LEN_512

// SCDS CARD_COMMANDs

// return 0 == idle
// return non-0 == not idle
#define SCDS_CMD_CARD_BUSY       (0x38ull << 56)

// Others
#define SCDS_CMD_CARD_RESPONSE   (0x3040ull << 48)
#define SCDS_CMD_SD_READ_REQUEST (0x34ull << 56)
#define SCDS_CMD_SD_WRITE_END    (0x35ull << 56)
#define SCDS_CMD_SD_READ_DATA    (0x36ull << 56)

// 0x33 command has a whole lot of parameters. Will be split as needed
#define SCDS_CMD_CARD_PARAM                     (0x33ull << 56) | (0x40ull << 40)
#define SCDS_CMD_CARD_PARAM_R1                  SCDS_CMD_CARD_PARAM | 1ull

#define SCDS_CMD_SD_READ_MULTI_SECTOR_END      SCDS_CMD_CARD_PARAM | (12ull << 40)

static inline u64 SCDS_CMD_SD_READ_SINGLE_SECTOR_SEND(u32 sector)
{
	return SCDS_CMD_CARD_PARAM_R1 | (17ull << 40) | ((u64)sector << 8);
}

static inline u64 SCDS_CMD_SD_READ_MULTI_SECTOR_SEND(u32 sector)
{
	return SCDS_CMD_CARD_PARAM_R1 | (18ull << 40) | ((u64)sector << 8);
}

// writing only works single sector
static inline u64 SCDS_CMD_SD_WRITE_SECTOR_SEND(u32 sector)
{
	return SCDS_CMD_CARD_PARAM_R1 | (24ull << 40) | ((u64)sector << 8);
}

// bytes == number of bytes currently sent
// for each 4 bytes, write next word from buffer, up to 512 bytes
static inline u64 SCDS_CMD_SD_WRITE_DATA(u32 bytes, u32 data)
{
	return (0x37ull << 56) | ((u64)bytes << 40) | ((u64)(__builtin_bswap32(data)) << 8);
}

// user API
void SCDSSDReadSingleSector(u32 sector, void *buffer);
void SCDSSDReadMultiSector(u32 sector, void *buffer, u32 num_sectors);
void SCDSSDWriteSector(u32 sector, const u32 *buffer);
