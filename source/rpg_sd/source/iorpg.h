/*
	Acekard RPG
	Card IO routines

	Copyright (C) 2023 lifehackerhansol

	SPDX-License-Identifier: Zlib
*/

#pragma once

#include <nds/ndstypes.h>
#include "libtwl_card.h"

#ifndef NULL
 #define NULL 0
#endif

// all RPG-based SDIO drivers use this as base, and add latency/size as needed
#define IORPG_CTRL_BASE      (MCCNT1_CMD_SCRAMBLE | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE | MCCNT1_LATENCY2(0) | MCCNT1_LATENCY1(0))
#define IORPG_CTRL_POLL      (IORPG_CTRL_BASE | MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_LEN_0)
#define IORPG_CTRL_READ_4B   (IORPG_CTRL_POLL | MCCNT1_LEN_4)
#define IORPG_CTRL_READ_512B (IORPG_CTRL_POLL | MCCNT1_LEN_512)
// by default all commands are using 4KB in the original driver; can this be changed?
#define IORPG_CTRL_READ_4KB  (IORPG_CTRL_POLL | MCCNT1_LEN_4096)
#define IORPG_CTRL_READ_16KB  (IORPG_CTRL_POLL | MCCNT1_LEN_16384)

// ret & 0x0F != provided state
// Note, on non-AKRPG carts, the state is in the upper 4 bits, so check 0xF0 instead
// TODO: what do these states mean?
#define IORPG_CMD_SD_READ_STATE        (0xC0ull << 56)

// command = 0xB700000000130000
// TODO: what does 0x13 mean?
#define IORPG_CMD_CARD_READ_DATA (0xB7ull << 56 | 0x13ull << 16)

// command = 0xB8B8B8B8B8B8B8B8
// This gets the chip ID... but why fill the entire buffer with it?
#define IORPG_CMD_CARD_WAIT_BUSY 0xB8B8B8B8B8B8B8B8ull

/*
	All AKRPG-based *SD* drivers use SDIO.
	Format:
	0xAABB00CC, 0xDDDDDDDD
	AA = IORPG_CMD_SDIO_BYTE // this is different depending on the flashcart
	BB = SDIO response type
	CC = SDIO command
	DDDDDDDD = SDIO parameter
*/

// the command identifier for SDIO differs based on the flashcart in question
#ifdef R4IDSN // carts from r4idsn.com
#define IORPG_CMD_SDIO_BYTE (0xABull << 56)
#else // all known other carts
#define IORPG_CMD_SDIO_BYTE (0xD5ull << 56)
#endif

enum IORPG_SDIO_PARAM_TYPES {
	IORPG_SDIO_NORESPONSE = 0ull,
	IORPG_SDIO_READ_RESPONSE = 1ull,
	IORPG_SDIO_READ_SINGLE_BLOCK = 3ull,
	IORPG_SDIO_READ_MULTI_BLOCK = 4ull,
	IORPG_SDIO_WRITE_SINGLE_BLOCK = 5ull,
	IORPG_SDIO_WRITE_MULTI_BLOCK = 6ull
};

#define IORPG_CMD_SET_SD_MODE_SDHC (0xC101000000000000ull)

static inline u64 IORPG_CMD_SDIO(u8 sdio, u8 param_type, u32 parameter)
{
	u32 command0 = (((u32)param_type << 16) | ((u32)sdio));
	return (IORPG_CMD_SDIO_BYTE | (((u64)command0) << 32) | (u64)parameter);
}

static inline u64 IORPG_CMD_SD_WRITE_DATA(const u32* buffer)
{
	u64 command;
	if((u32)buffer & 0x3)
	{
		command = (((u64)__builtin_bswap32(buffer[0]) << 32) | ((u64)__builtin_bswap32(buffer[1])));
	}
	else
	{
		u8* u8_buffer = (u8*)buffer;
		command = (
			((u64)u8_buffer[0] << 56) |
			((u64)u8_buffer[1] << 48) |
			((u64)u8_buffer[2] << 40) |
			((u64)u8_buffer[3] << 32) |
			((u64)u8_buffer[4] << 24) |
			((u64)u8_buffer[5] << 16) |
			((u64)u8_buffer[6] <<  8) |
			((u64)u8_buffer[7]      )
		);
	}
	return command;
}

// user API
u32 ioRPG_SendCommand(u64 command);
bool ioRPG_Initialize(void);
void ioRPG_SDReadSingleSector(u32 sector, void* buffer);
void ioRPG_SDReadMultiSector(u32 sector, u32 num_sectors, void* buffer);
void ioRPG_SDWriteSingleSector(u32 sector, const void* buffer);
void ioRPG_SDWriteMultiSector(u32 sector, u32 num_sectors, const void* buffer);
