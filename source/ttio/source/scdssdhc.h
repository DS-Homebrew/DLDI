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
#define SCDS_CTRL_BASE             (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0))
// always used in 0x51 commands
#define SCDS_CTRL_READ_4B_DELAY    (SCDS_CTRL_BASE | MCCNT1_LEN_4 | MCCNT1_LATENCY1(16))
#define SCDS_CTRL_READ_4B          (SCDS_CTRL_BASE | MCCNT1_LEN_4)
#define SCDS_CTRL_READ_512B        (SCDS_CTRL_BASE | MCCNT1_LEN_512)
#define SCDS_CTRL_WRITE_4B         (SCDS_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_4)
#define SCDS_CTRL_WRITE_512B       (SCDS_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

// SCDS CARD_COMMANDs

// return 0 == idle
// return non-0 == not idle
#define SCDS_CMD_CARD_BUSY       (0x50ull << 56)

// Others
#define SCDS_CMD_CARD_RESPONSE   (0x52ull << 56)
#define SCDS_CMD_SD_WRITE_END    (0x56ull << 56)

// return 0 == SD
// return non-0 == SDHC
// not supported on DSTT
//#define SCDS_CMD_SD_IS_SDHC      ((0x70ull << 56) | (0x7F9E0ull << 24))

#define SCDS_CMD_SD_READ_REQUEST (0x80ull << 56)
#define SCDS_CMD_SD_READ_DATA    (0x81ull << 56)
#define SCDS_CMD_SD_WRITE_DATA   (0x82ull << 56)

// 0x51 command has a whole lot of parameters. Will be split as needed
#define SCDS_CMD_CARD_PARAM             (0x51ull << 56)
#define SCDS_CMD_CARD_PARAM_R1          (SCDS_CMD_CARD_PARAM | 0x100ull)
#define SCDS_CMD_SD_READ_REQUEST_NEXT   (SCDS_CMD_CARD_PARAM | (7ull << 8))
#define SCDS_CMD_SD_RW_MULTI_SECTOR_END (SCDS_CMD_CARD_PARAM_R1 | (12ull << 16))

static inline u64 SCDS_CMD_SD_READ_SINGLE_SECTOR_SEND(u32 sector)
{
	return (0x53ull << 56) | ((u64)sector << 24);
}

static inline u64 SCDS_CMD_SD_READ_MULTI_SECTOR_SEND(u32 sector)
{
	return (0x54ull << 56) | ((u64)sector << 24);
}

static inline u64 SCDS_CMD_SD_WRITE_SINGLE_SECTOR_SEND(u32 sector)
{
	return SCDS_CMD_CARD_PARAM_R1 | ((u64)sector << 24) | (24ull << 16);
}

static inline u64 SCDS_CMD_SD_WRITE_MULTI_SECTOR_SEND(u32 sector)
{
	return SCDS_CMD_CARD_PARAM_R1 | ((u64)sector << 24) | (25ull << 16);
}

// user API
u32 SCDS_SendCommand(const u64 command);
//void SCDS_SDReadSingleSector(u32 sector, void *buffer);
void SCDS_SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors);
void SCDS_SDWriteSingleSector(u32 sector, const void *buffer);
void SCDS_SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors);
