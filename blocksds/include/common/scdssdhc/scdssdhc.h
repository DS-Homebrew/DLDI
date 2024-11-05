/*
    SuperCard DSONE
    Card IO routines

    Copyright (C) 2006 SuperCard
    Copyright (C) 2023 lifehackerhansol

    SPDX-License-Identifier: Zlib
*/

#pragma once

#include <libtwl/card/card.h>
#include <nds/ndstypes.h>

#ifndef NULL
#define NULL 0
#endif

// libtwl workaround
// Certain DSTT clones need bytewise access to MCCMD
#define REG_SCDSSDHC_MCCMD ((vu8*)&REG_MCCMD0)

// SCDSSDHC defines
// SCDSSDHC ROMCTRL flags
#define SCDSSDHC_CTRL_BASE \
    (MCCNT1_ENABLE | MCCNT1_RESET_OFF | MCCNT1_LATENCY2(24) | MCCNT1_LATENCY1(0))
#define SCDSSDHC_CTRL_READ_4B (SCDSSDHC_CTRL_BASE | MCCNT1_LEN_4)
#define SCDSSDHC_CTRL_READ_512B (SCDSSDHC_CTRL_BASE | MCCNT1_LEN_512)
#define SCDSSDHC_CTRL_WRITE_4B (SCDSSDHC_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_4)
#define SCDSSDHC_CTRL_WRITE_512B (SCDSSDHC_CTRL_BASE | MCCNT1_DIR_WRITE | MCCNT1_LEN_512)

#define SCDSSDHC_CTRL_SD_LOW_CLK_LATENCY 0x1000

// SCDSSDHC CARD_COMMANDs

/*
    SRAM commands

    Command structure:
    AA BB BB BB BB 00 00 00
    AA = command
    BBBBBBBB = address
*/
#define SCDSSDHC_CMD_SRAM_READ_DATA (0x70)
#define SCDSSDHC_CMD_SRAM_WRITE_DATA (0x71)

// FIFO commands
#define SCDSSDHC_CMD_FIFO_BUSY (0x80)
#define SCDSSDHC_CMD_FIFO_READ_DATA (0x81)
#define SCDSSDHC_CMD_FIFO_WRITE_DATA (0x82)

/*
    SD host modes
    Used with 0x51 command, see below
*/
enum SCDSSDHC_SD_HOST_MODES {
    SCDSSDHC_SD_HOST_NORESPONSE = 0,
    SCDSSDHC_SD_HOST_READ_4B = 1,
    SCDSSDHC_SD_HOST_READ_4B_MULTI = 2,  // use mode 3 to continue this read
    SCDSSDHC_SD_HOST_NEXT_4B = 3,
    SCDSSDHC_SD_HOST_SEND_CLK = 4,
    SCDSSDHC_SD_HOST_SEND_STOP_CLK = 5,
    SCDSSDHC_SD_HOST_READ_DATABLOCK = 6,
    SCDSSDHC_SD_HOST_NEXT_DATABLOCK = 7,
    SCDSSDHC_SD_HOST_CMD17_READ_DATA = 8,     // Send SDIO CMD17 & read data
    SCDSSDHC_SD_HOST_CMD18_READ_DATA = 9,     // Send SDIO CMD18 & read data until stop
    SCDSSDHC_SD_HOST_COMMIT_FIFO_DATA = 0xA,  // commit data in FIFO to SD card
    SCDSSDHC_SD_HOST_CMD24_WRITE_DATA = 0xB,  // Send SDIO CMD24 & send data in SRAM buffer
    SCDSSDHC_SD_HOST_WAIT_BUSY = 0xC          // wait until data transfer ends
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
    CC = SD host mode, see SCDSSDHC_SD_HOST_MODES enum
*/
#define SCDSSDHC_CMD_SD_HOST_PARAM (0x51)

// SD host misc commands
// return 0 == idle
// return non-0 == not idle
#define SCDSSDHC_CMD_SD_HOST_BUSY (0x50)

// Gets response of SD_HOST commands, if the sent mode is 1 or 2
#define SCDSSDHC_CMD_SD_HOST_RESPONSE (0x52)
// Stops SD host data transfer
#define SCDSSDHC_CMD_SD_WRITE_END (0x56)

// Sends CMD17
#define SCDSSDHC_CMD_SD_READ_SINGLE_BLOCK (0x53)
// Sends CMD18
#define SCDSSDHC_CMD_SD_READ_MULTI_BLOCK (0x54)

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
#define SCDSSDHC_SD_HOST_REG_RESET BIT(0)
#define SCDSSDHC_SD_HOST_REG_400KHZ_CLK BIT(1)
#define SCDSSDHC_SD_HOST_REG_CLEAN_ROM_MODE BIT(2)
#define SCDSSDHC_SD_HOST_REG_SDHC BIT(3)

// Command to write to host register
#define SCDSSDHC_CMD_SD_HOST_SET_REGISTER (0x5F)

// user API
void SCDSSDHC_SDGetSDHCStatusFromSRAM(void);
bool SCDSSDHC_SDInitialize(void);
void SCDSSDHC_SDReadSingleSector(u32 sector, void* buffer);
void SCDSSDHC_SDReadMultiSector(u32 sector, void* buffer, u32 num_sectors);
void SCDSSDHC_SDWriteSingleSector(u32 sector, const void* buffer);
void SCDSSDHC_SDWriteMultiSector(u32 sector, const void* buffer, u32 num_sectors);
