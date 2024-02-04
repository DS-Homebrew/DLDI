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
#define SCDS_CTRL_READ_4B          (SCDS_CTRL_BASE | MCCNT1_LEN_4)
#define SCDS_CTRL_READ_512B        (SCDS_CTRL_BASE | MCCNT1_LEN_512)
#define SCDS_CTRL_WRITE_4B         (SCDS_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_4)
#define SCDS_CTRL_WRITE_512B       (SCDS_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

#define SCDS_CTRL_SD_LOW_CLK_LATENCY 0x1000

// SCDS CARD_COMMANDs

/*
	SRAM commands

	Command structure:
	AA BB BB BB BB 00 00 00
	AA = command
	BBBBBBBB = address
*/

static inline u64 SCDS_CMD_SRAM_READ_DATA(u32 address)
{
	return (0x70ull << 56) | ((u64)address << 24);
}

static inline u64 SCDS_CMD_SRAM_WRITE_DATA(u32 address)
{
	return (0x71ull << 56) | ((u64)address << 24);
}

/*
	This address in SRAM *usually* contains a bool for SD or SDHC
	While not exactly reliable, the original driver did it, so here goes

	return 0 == SD
	return non-0 == SDHC
*/
static inline u64 SCDS_CMD_SD_IS_SDHC(void)
{
	return SCDS_CMD_SRAM_READ_DATA(0x7F9E0);
}

// FIFO commands
#define SCDS_CMD_FIFO_BUSY        (0x80ull << 56)
#define SCDS_CMD_FIFO_READ_DATA   (0x81ull << 56)
#define SCDS_CMD_FIFO_WRITE_DATA  (0x82ull << 56)

/*
	SD host modes
	Used with 0x51 command, see below
*/
enum SCDS_SD_HOST_MODES {
	SCDS_SD_HOST_NORESPONSE = 0ull,
	SCDS_SD_HOST_READ_4B = 1ull,
	SCDS_SD_HOST_READ_4B_MULTI = 2ull, // use mode 3 to continue this read
	SCDS_SD_HOST_NEXT_4B = 3ull,
	SCDS_SD_HOST_SEND_CLK = 4ull,
	SCDS_SD_HOST_SEND_STOP_CLK = 5ull,
	SCDS_SD_HOST_READ_DATABLOCK = 6ull,
	SCDS_SD_HOST_NEXT_DATABLOCK = 7ull,
	SCDS_SD_HOST_CMD17_READ_DATA = 8ull, // Send SDIO CMD17 & read data
	SCDS_SD_HOST_CMD18_READ_DATA = 9ull, // Send SDIO CMD18 & read data until stop
	SCDS_SD_HOST_COMMIT_FIFO_DATA = 0xAull, // commit data in FIFO to SD card
	SCDS_SD_HOST_CMD24_WRITE_DATA = 0xBull, // Send SDIO CMD24 & send data in SRAM buffer
	SCDS_SD_HOST_WAIT_BUSY = 0xCull // wait until data transfer ends
};

/*
	SD host related commands

	Note:
		While this is where the SDIO happens, it isn't always SDIO
		Thus, it can sometimes be 0

	Command structure:
	51 AA AA AA AA BB CC 00
	AAAAAAAA = SDIO parameter
	BB = SDIO command
	CC = SD host mode, see SCDS_SD_HOST_MODES enum
*/
static inline u64 SCDS_CMD_SD_HOST_PARAM(u8 sdio, u32 parameter, u8 mode)
{
	return (0x51ull << 56) | ((u64)parameter << 24) | ((u64)sdio << 16) | ((u64)mode << 8);
}

// SD host misc commands
// return 0 == idle
// return non-0 == not idle
#define SCDS_CMD_SD_HOST_BUSY     (0x50ull << 56)

// Gets response of SD_HOST commands, if the sent mode is 1 or 2
#define SCDS_CMD_SD_HOST_RESPONSE (0x52ull << 56)
// Stops SD host data transfer
#define SCDS_CMD_SD_WRITE_END     (0x56ull << 56)

// Sends CMD17
// This is effectively 0x51/0x50/0x52 rolled into one command
static inline u64 SCDS_CMD_SD_READ_SINGLE_BLOCK(u32 sector)
{
	return (0x53ull << 56) | ((u64)sector << 24);
}

// Sends CMD18
// This is effectively 0x51/0x50/0x52 rolled into one command
static inline u64 SCDS_CMD_SD_READ_MULTI_BLOCK(u32 sector)
{
	return (0x54ull << 56) | ((u64)sector << 24);
}

/*
	SD host control registers
	The 0x5F sets raw registers related to the SD host, which is a single u8.
	Bits:
	0: Reset
	1: Set 400k low clk
	2: Use 0xB7 as alternative of 0x5B for ROM reads
	3: Set SDHC mode
	4-5: 1
	6-7: 0
*/
#define SCDS_SD_HOST_REG_RESET BIT(0)
#define SCDS_SD_HOST_REG_400KHZ_CLK BIT(1)
#define SCDS_SD_HOST_REG_CLEAN_ROM_MODE BIT(2)
#define SCDS_SD_HOST_REG_SDHC BIT(3)

// This function will always set bits 4-5 to 1 and bits 6-7 to 0 for convenience
static inline u64 SCDS_CMD_SD_HOST_SET_REGISTER(u8 bits)
{
	return (0x5F30ull << 48) | ((u64)bits << 48);
}

// user API
u32 SCDS_SendCommand(const u64 command, u32 latency);
void SCDS_SDGetSDHCStatusFromSRAM(void);
bool SCDS_SDInitialize(void);
void SCDS_SDReadSingleSector(u32 sector, void *buffer);
void SCDS_SDReadMultiSector(u32 sector, void *buffer, u32 num_sectors);
void SCDS_SDWriteSingleSector(u32 sector, const void *buffer);
void SCDS_SDWriteMultiSector(u32 sector, const void *buffer, u32 num_sectors);
